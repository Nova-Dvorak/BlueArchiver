#include<cstdio>
#include<cstring>
#include<vector>
#include<queue>

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

constexpr char HELP[]=R"(This program is part of BLUE ARCHIVER.
BLUE ARCHIVER is a file archive program using simple compression algorithms.
As an early version of the program, this version can only compress only one file a time.

parameters:
there are only 2 parameters:
    1. the input filename;
    2. the output filename. (the file extension "yuuka" will be appended later)

And at last, yuri is good for our health. So please support yuri content creation in Blue Archive community. Thank you very much.

)";
struct Header
{
    const char identifier[8] = "BA+++++";
    uint8 filename_length;
    uint64 size;
    uint8 key_length[256];
};
/*
The file should go like this:
    Header of 280 bytes                                         which type is Header
    Filename of filename_length bytes (without null-terminator) which type is char[]
    Dictionary of ceil(sum of key_length / 8) bytes             which type is uint8[]
    Compressed data of ceil(size / 8) bytes                     which type is uint8[]                             
*/







/*Huffman part*/
class HuffmanNode
{
public:
    class Huff_comp
    {
    public:
        bool operator() (HuffmanNode* a, HuffmanNode* b) const
        { return a->frequency > b->frequency; }
    };



    HuffmanNode *son[2];
    uint8 value;
    uint32 frequency;
    HuffmanNode(uint8 value, uint32 frequency) : value(value), frequency(frequency), son{nullptr, nullptr} {}
    HuffmanNode(HuffmanNode *son[2]) : son{son[0], son[1]}, value(0), frequency(son[0]->frequency + son[1]->frequency) {}
    ~HuffmanNode()
    {
        if(son[0] != nullptr)
            delete son[0];
        if(son[1] != nullptr)
            delete son[1];
    }
};


HuffmanNode *build_tree(uint32 *frequency)
{
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, HuffmanNode::Huff_comp> pq;
    for(uint32 i = 0; i < 256; i++)
    {
        if(frequency[i] != 0)
            pq.push(new HuffmanNode(i, frequency[i]));
    }
    while(pq.size() > 1)
    {
        HuffmanNode *son[2];
        son[0] = pq.top();
        pq.pop();
        son[1] = pq.top();
        pq.pop();
        pq.push(new HuffmanNode(son));
    }
    return pq.top();
}

void build_table(HuffmanNode *now, std::vector<bool> table[256], std::vector<bool> path)
{
    if(now->son[0] == nullptr && now->son[1] == nullptr)
    {
        table[now->value] = path;
        return;
    }
    path.push_back(false);
    build_table(now->son[0], table, path);
    path.pop_back();
    path.push_back(true);
    build_table(now->son[1], table, path);
}





int main(int argc, char **argv)
{
    if(argc != 3)
    {
        fprintf(stderr, HELP);
        return -1;
    }





    /*input part*/

    //input file opening
    FILE *input = fopen(argv[1], "rb");
    if(input == nullptr)
    {
        fprintf(stderr, "Error: cannot open file %s\n", argv[1]);
        return -1;
    }

    Header header;
    
    //checking file size
    fseek(input, 0, SEEK_END);
    int inputsize = ftell(input);
    fseek(input, 0, SEEK_SET);





    /*encoding part*/

    //counting frequency
    uint32 frequency[256] = {0};
    for(uint32 i = 0; i < inputsize; i++)
    {
        uint8 c;
        fread(&c, 1, 1, input);
        frequency[c]++;
    }


    //building huffman tree
    HuffmanNode *root = build_tree(frequency);
    
    //building huffman code table
    std::vector<bool> table[256];
    build_table(root, table, std::vector<bool>());

    //encoding
    fseek(input, 0, SEEK_SET);
    std::vector<bool> code;
    for(uint32 i = 0; i < inputsize; i++)
    {
        uint8 c;
        fread(&c, 1, 1, input);
        for(bool bit : table[c])
            code.push_back(bit);
    }
    fclose(input);




    /*output part*/

    //output file opening
    char *outfile = new char[strlen(argv[2]) + 6];
    sprintf(outfile, "%s.yuuka", argv[2]);
    FILE *output = fopen(outfile, "wb");
    if(output == nullptr)
    {
        fprintf(stderr, "Error: cannot open file %s\n", outfile);
        return -1;
    }

    //header completion
    header.size = code.size();
    header.filename_length = strlen(argv[1]);
    for(uint32 i = 0; i < 256; i++)
        header.key_length[i] = table[i].size();
    

    //writing header, filename
    fwrite(&header, sizeof(header), 1, output);
    fwrite(argv[1], 1, strlen(argv[1]), output);

    //writing dictionary
    static uint8 byte = 0;
    static uint8 bit_count = 0;
    for(uint32 i = 0; i < 256; i++)
    {
        for(bool bit : table[i])
        {
            byte |= bit << bit_count;
            bit_count++;
            if(bit_count == 8)
            {
                fwrite(&byte, 1, 1, output);
                byte = 0;
                bit_count = 0;
            }
        }
    }
    if(bit_count != 0)
        fwrite(&byte, 1, 1, output);
    

    //writing compressed data
    byte = 0;
    bit_count = 0;
    for(bool bit : code)
    {
        byte |= bit << bit_count;
        bit_count++;
        if(bit_count == 8)
        {
            fwrite(&byte, 1, 1, output);
            byte = 0;
            bit_count = 0;
        }
    }
    if(bit_count != 0)
        fwrite(&byte, 1, 1, output);
    
    //closing files
    fclose(output);
    return 0;
}
