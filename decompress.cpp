#include<cstdio>
#include<cstring>
#include<unordered_map>
#include"error.h"

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;


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
uint8* decompress(FILE* input, FILE* output, const Header& header)
{
    std::unordered_map<uint16, std::string> dict;
    fprintf(stderr, "Decoding...");
    uint8 *data = new uint8[header.size], *p = data;
    for(uint64 i = 0; i < header.item_count; i++)
    {
        if(i == header.item_count - 1)
            fprintf(stderr, "Last item.\n");
        uint16 index;
        char next_char;
        fread(&index, sizeof(index), 1, input);
        fread(&next_char, sizeof(next_char), 1, input);
        if(index == 0)
            dict.insert({dict.size() + 1, std::string(1, next_char)});
        else
        {
            if(index > dict.size())
            {
                fprintf(stderr, "Error: Index out of range.\n");
                return nullptr;
            }
            memcpy(p, dict[index].c_str(), dict[index].size());
            p += dict[index].size();
            dict.insert({dict.size() + 1, dict[index] + next_char});
        }
        *p++ = next_char;
    }
    fprintf(stderr, "Done.\n");
    return data;
}


int main(int argc, char **argv)
{
    if(argc != 2 && argc != 3)
    {
        fprintf(stderr, HELP2);
        return PARAM_ERROR;
    }
    //file extension checking
    char *extension=strrchr(argv[1], '.'), *inputname;
    if(extension==NULL || strcmp(extension, ".yuuka") != 0)
    {
        inputname = new char[strlen(argv[1]) + 7];
        strcpy(inputname, argv[1]);
        strcat(inputname, ".yuuka");
    }
    else
        inputname = argv[1];



    /*I/O Part*/
    //input file opening
    FILE *input=fopen(inputname, "rb");
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
    char filename[header.filename_length + 1];
    fread(filename, header.filename_length, 1, input);
    filename[header.filename_length] = '\0';
    fprintf(stderr, "Filename: %s\n", filename);

    //output file opening
    FILE *output;
    char *outputname;
    if(argc == 3)
        outputname = argv[2];
    else
        outputname = filename;
    output = fopen(outputname, "rb");
    if(output != NULL)
    {
        fclose(output);
        fprintf(stderr, "Error: File %s already exists, do you want to overwrite it?[Y/n]:", outputname);
        char c;
        scanf("%c", &c);
        if(c != 'Y' && c != 'y')
        {
            fprintf(stderr, "Operation cancelled.\n");
            return 0;
        }
    }
    output = fopen(outputname, "wb");
    if(output == NULL)
    {
        fprintf(stderr, "Error: Cannot open the output file.\n");
        return FILE_ERROR | OUTPUT_ERROR;
    }
    fprintf(stderr, "Output file %s opened.\n", outputname);




    /*Decoding Part*/
    uint8 *data = decompress(input, output, header);
    if(data == nullptr)
    {
        fclose(output);
        return CONTENT_ERROR | INPUT_ERROR;
    }

    //writing data
    fprintf(stderr, "Writing data...");
    fwrite(data, 1, header.size, output);
    fprintf(stderr, "Done.\n");

    fclose(output);
    return 0;
}
