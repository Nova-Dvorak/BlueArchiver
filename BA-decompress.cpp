// Path: BA-decompress.cpp
#include<cstdio>
#include<cstring>
#include<vector>
#include<queue>
#include<cmath>

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

constexpr char HELP[]=R"(This program is part of BLUE ARCHIVER.
BLUE ARCHIVER is a file archive program using simple compression algorithms.
As an early version of the program, this version can only compress only one file a time.

parameters:
there are only 1 parameter:
    1. the input filename;




)";

struct Header
{
    char identifier[8];
    uint8 filename_length;
    uint64 size;
    uint8 key_length[256];
};

/*Huffman part*/
class HuffmanNode
{
public:



    HuffmanNode *son[2];
    uint8 value;
    HuffmanNode(uint8 value, uint32 treesize) : value(value), son{nullptr, nullptr} {}
    HuffmanNode(HuffmanNode *son[2]) : son{son[0], son[1]}, value(0) {}
    ~HuffmanNode()
    {
        if(son[0] != nullptr)
            delete son[0];
        if(son[1] != nullptr)
            delete son[1];
    }
};

void rebuildHuffmanTree(HuffmanNode *root, uint8 key_length[256], std::vector<bool> table[256])
{
    for(int i = 0; i < 256; i++)
    {
        if(key_length[i] != 0)
        {
            HuffmanNode *node = root;
            for(bool bit : table[i])
            {
                if(node->son[bit] == nullptr)
                    node->son[bit] = new HuffmanNode(0, 0);
                node = node->son[bit];
            }
            node->value = i;
        }
    }
}

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        fprintf(stderr, HELP);
        return -1;
    }



    /*Input Part*/

    //file extension checking
    char *extension=strrchr(argv[1], '.');
    if(extension==NULL || strcmp(extension, ".yuuka") != 0)
    {
        fprintf(stderr, "Error: This is not a yuuka file!\n");
        return -1;
    }

    //input file opening
    FILE *input=fopen(argv[1], "rb");
    if(input==NULL)
    {
        fprintf(stderr, "Error: Cannot open the input file.\n");
        return -1;
    }
/*
The file should go like this:
    Header of 280 bytes                                         which type is Header
    Filename of filename_length bytes (without null-terminator) which type is char[]
    Dictionary of ceil(sum of key_length / 8) bytes             which type is uint8[]
    Compressed data of ceil(size / 8) bytes                     which type is uint8[]                             
*/

    //header reading
    Header header{};
    fread(&header, sizeof(header), 1, input);
    if(memcmp(header.identifier, "BA+++++", 8) != 0)
    {
        fprintf(stderr, "Error: File identifier is not correct.\n");
        return -1;
    }

    //filename reading
    char filename[256];
    fread(filename, header.filename_length, 1, input);
    filename[header.filename_length] = '\0';

    //dictionary reading
    uint16 dictionary_length = 0;
    for(int i=0; i<256; i++)
        dictionary_length += header.key_length[i];
    uint16 dictionary_size = (uint16)ceil(dictionary_length / 8.0);
    uint8 *dictionary = new uint8[dictionary_size];
    fread(dictionary, dictionary_size, 1, input);

    //compressed data reading
    uint64 data_size = (uint64)ceil(header.size / 8.0);
    uint8 temp;
    std::vector<bool> code;
    for(uint64 i = 0; i < data_size - 1; i++)
    {
        fread(&temp, 1, 1, input);
        for(int j = 0; j < 8; j++)
            code.push_back((temp >> j) & 1);
    }
    fread(&temp, 1, 1, input);
    for(int j = 0; j < header.size % 8; j++)
        code.push_back((temp >> j) & 1);

    fclose(input);


    /*Decoding Part*/

    //building dictionary
    std::vector<bool> table[256];
    uint16 bit_count = 0;
    uint8 byte = 0;
    for(int i = 0; i < 256; i++)
    {
        for(int j = 0; j < header.key_length[i]; j++)
        {
            table[i].push_back((dictionary[bit_count / 8] >> (bit_count % 8)) & 1);
            bit_count++;
        }
    }

    //rebuilding huffman tree
    HuffmanNode *root = new HuffmanNode(0, 0);
    rebuildHuffmanTree(root, header.key_length, table);



    /*Output Part*/

    //output file opening
    FILE *output = fopen(filename, "rb");
    if(output != NULL)
    {
        fprintf(stderr, "Error: File %s already exists.\n", filename);
        fclose(output);
        return -1;
    }
    output = fopen(filename, "wb");
    if(output == NULL)
    {
        fprintf(stderr, "Error: Cannot open the output file.\n");
        return -1;
    }

    //decompressing
    HuffmanNode *node = root;
    for(bool bit : code)
    {
        node = node->son[bit];
        if(node->son[0] == nullptr && node->son[1] == nullptr)
        {
            fwrite(&node->value, 1, 1, output);
            node = root;
        }
    }

    fclose(output);
    return 0;
}