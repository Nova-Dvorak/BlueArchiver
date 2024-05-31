#include<cstdio>
#include<cstring>
#include<unordered_map>
#include"BA-error.h"

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

constexpr char HELP[]=R"(This program is part of BLUE ARCHIVER.

BLUE ARCHIVER is a file archive program using simple compression algorithms, developed as a trial and exercise to enhance my coding skills.
It accepts a file with ".yuuka" extension.

parameters:
    there are only 1 parameter:

        1. the input filename;

)";

struct Header
{
    char identifier[8];
    uint8 filename_length;
    uint64 size, item_count;
};

/*LZ78 part*/
struct LZ78Item //Encoded data
{
    uint16 index;
    uint8 next_char;
    LZ78Item(uint16 i, uint8 c): index(i), next_char(c) {}
};


int main(int argc, char **argv)
{
    #define argc 2
    #define argv aaa
    if(argc != 2)
    {
        fprintf(stderr, HELP);
        return PARAM_ERROR;
    }
    char *aaa[] = {"", "test.yuuka"};
    //file extension checking
    char *extension=strrchr(argv[1], '.');
    if(extension==NULL || strcmp(extension, ".yuuka") != 0)
    {
        fprintf(stderr, "Error: This is not a yuuka file!\n");
        return FILE_ERROR;
    }



    /*I/O Part*/
    //input file opening
    FILE *input=fopen(argv[1], "rb");
    if(input==NULL)
    {
        fprintf(stderr, "Error: Cannot open the input file.\n");
        return FILE_ERROR | INPUT_ERROR;
    }

    //header reading
    Header header{};
    fread(&header, sizeof(header), 1, input);
    if(memcmp(header.identifier, "BAYuuka", 8) != 0)
    {
        fprintf(stderr, "Error: File identifier is not correct.\n");
        return CONTENT_ERROR | INPUT_ERROR;
    }
    fprintf(stderr, "Header content: filename_length=%x, item_count=%08llx\n", header.filename_length, header.item_count);

    //filename reading
    char filename[256];
    fread(filename, header.filename_length, 1, input);
    filename[header.filename_length] = '\0';
    fprintf(stderr, "Filename: %s\n", filename);

    //output file opening
    FILE *output = fopen(filename, "rb");
    if(output != NULL)
    {
        fclose(output);
        fprintf(stderr, "Error: File %s already exists, do you want to overwrite it?[Y/n]:", filename);
        char c;
        scanf("%c", &c);
        if(c != 'Y' && c != 'y')
        {
            fprintf(stderr, "Operation cancelled.\n");
            return 0;
        }
    }
    output = fopen(filename, "wb");
    if(output == NULL)
    {
        fprintf(stderr, "Error: Cannot open the output file.\n");
        return FILE_ERROR | OUTPUT_ERROR;
    }
    fprintf(stderr, "Output file %s opened.\n", filename);




    /*Decoding Part*/
    std::unordered_map<uint16, std::string> dict;
    fprintf(stderr, "Decoding & Writing...");
    for(uint64 i = 0; i < header.item_count; i++)
    {
        uint16 index;
        char next_char;
        fread(&index, sizeof(index), 1, input);
        fread(&next_char, sizeof(next_char), 1, input);
        if(index == 0)
        {
            fwrite(&next_char, 1, 1, output);
            dict.insert({dict.size() + 1, std::string(1, next_char)});
        }
        else
        {
            fwrite(dict[index].c_str(), dict[index].size(), 1, output);
            fwrite(&next_char, 1, 1, output);
            dict.insert({dict.size() + 1, dict[index] + next_char});
        }
    }
    fprintf(stderr, "Complete.\n");


    fclose(output);
    return 0;
}