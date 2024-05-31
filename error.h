/*
version pre-0.2

    *Version pre-0.1 is served with single file compression and decompression, applying huffman coding on each byte.
    *Plans:
    *    /Add more information to stderr output.
    *    /Separate different error codes.
    *    Change the parameters
    *        enable default output filename
    *        enable dictionary size control
    *    Design a file I/O interface (Actually it is essential for supporting big files.)
    *    Add error handling
    *    Separate the code into different files. (This might be done later, when the project gets more complex.)
*/
#define FILE_ERROR 2
#define CONTENT_ERROR 4
#define INPUT_ERROR 8
#define OUTPUT_ERROR 16
#define PARAM_ERROR 32

constexpr char HELP1[] = R"(This program is part of BLUE ARCHIVER.

BLUE ARCHIVER is a file archive program using simple compression algorithms, developed as a trial and exercise to enhance my coding skills.
It generates a file with ".yuuka" extension.

    parameters:
    there are only 2 parameters (one of which is optional):

        1. the input filename;
        2. the output filename. (optional) (the file extension "yuuka" will be appended later)

If the output filename
At last, I want to marry Hayase Yuuka.
I love Shiromi Iori, Sunaōkami Shiroko and Kuromi Serika too.
And Tsukiyuki Miyako, who hates adults like me.
)",
HELP2[] = R"(This program is part of BLUE ARCHIVER.

BLUE ARCHIVER is a file archive program using simple compression algorithms, developed as a trial and exercise to enhance my coding skills.
It accepts a file with ".yuuka" extension.

parameters:
    there are only 2 parameters (one of which is optional):

        1. the input filename;
        2. the output filename. (optional)

)";