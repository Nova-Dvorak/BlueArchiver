#include<cstdio>
#include<cstring>
#include<string>
#include<unordered_map>
#include<vector>
#include"error.h"

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

struct Header
{
    const char identifier[8] = "BAYuuka";
    uint8 filename_length;
    uint64 size, item_count;
    Header(uint8 fl, uint64 s, uint64 ic): filename_length(fl), size(s), item_count(ic) {}
};
/*
The file should go like this:
    Header of 32 bytes                                          which type is Header
    Filename of filename_length bytes (without null-terminator) which type is char[]
    Compressed data of item_count * 3 bytes                     which type is LZ78Item[]

    Changes:
    * The dictionary is no longer stored in the file.
*/


/*LZ78 part*/
#define MAX_DICT_SIZE 65535 //limited by uint16, index starts from 1
struct LZ78Item //Encoded data
{
    uint16 index;
    char next_char;
    LZ78Item(uint16 i, char c): index(i), next_char(c) {}
};

void compress(std::vector<LZ78Item> &data, FILE *input, int inputsize)
{

    std::unordered_map<std::string, uint16> in_dict;
    std::string current;
    char c;
    uint16 last = 0;
    while(inputsize > 0)
    {
        if(inputsize == 1)
            printf("Last byte.\n"); //debug
        fread(&c, 1, 1, input);
        inputsize--;
        current += c;
        if(in_dict.find(current) == in_dict.end())
        {
            if(in_dict.size() < MAX_DICT_SIZE)
                in_dict.insert({current, in_dict.size() + 1});
            data.emplace_back(last, c);
            current.clear();
            last = 0;
        }
        else
            last = in_dict[current];
    }
    data.emplace_back(last, c);
}



int main(int argc, char **argv)
{
    if(argc != 3 && argc != 2)
    {
        fprintf(stderr, HELP1);
        return PARAM_ERROR;
    }


    /*input part*/

    //input file opening
    FILE *input = fopen(argv[1], "rb");
    if(input == nullptr)
    {
        fprintf(stderr, "Error: cannot open file %s\n", argv[1]);
        return INPUT_ERROR | FILE_ERROR;
    }
    fprintf(stderr, "File %s opened.\n", argv[1]);
    //checking file size
    fseek(input, 0, SEEK_END);
    int inputsize = ftell(input);
    fseek(input, 0, SEEK_SET);
    if(inputsize == 0)
    {
        fprintf(stderr, "Error: file %s is empty.\n", argv[1]);
        return INPUT_ERROR | CONTENT_ERROR;
    }
    fprintf(stderr, "File size: %08x bytes.\n", inputsize);



    /*compression part*/

    std::vector<LZ78Item> data;
    fprintf(stderr, "Compressing...");
    compress(data, input, inputsize);
    fprintf(stderr, "Complete.\n");
    fclose(input);



    /*output part*/

    //output file opening
    const char *filename = strrchr(argv[1], '\\');
    if(filename == nullptr) filename = strrchr(argv[1], '/');
    if(filename != nullptr)
        filename++; // Move past the '\\' character
    else
        filename = argv[1]; // If '\\' is not found, use the entire string as the filename
    char *outfile;
    if(argc == 3)
    {
        outfile = new char[strlen(argv[2]) + 7];
        sprintf(outfile, "%s.yuuka", argv[2]);
    }
    else {
        outfile = new char[strlen(filename) + 7];
        sprintf(outfile, "%s.yuuka", filename);
    }
    FILE *output = fopen(outfile, "wb");
    if(output == nullptr)
    {
        fprintf(stderr, "Error: cannot open file %s\n", outfile);
        return OUTPUT_ERROR | FILE_ERROR;
    }
    fprintf(stderr, "File %s opened.\n", outfile);

    //header completion

    Header header(strlen(filename), inputsize, data.size());
    fprintf(stderr, "Filename: %s\n", filename);
    fprintf(stderr, "Compressed size: %08x items.\n", data.size());


    //writing header, filename
    fprintf(stderr, "Writing header and filename...\n");
    fwrite(&header, sizeof(header), 1, output);
    fwrite(filename, 1, strlen(filename), output);
    //writing compressed data
    fprintf(stderr, "Writing compressed data...");

    for(auto i: data)
    {
        fwrite(&i.index, sizeof(i.index), 1, output);
        fwrite(&i.next_char, sizeof(i.next_char), 1, output);
    }
    fprintf(stderr, "Done.\n");
    
    //closing files
    fclose(output);
    return 0;
}