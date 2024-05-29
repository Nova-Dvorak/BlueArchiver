# Introduction

**BLUE ARCHIVER** is a file archive program using simple compression algorithms, developed as a trial and exercise to enhance my coding skills.

This version is created for my \"creation and practice\" course assignment.

As an early version of the program, now it can only compress/decompress one file at a time, using simple Huffman coding, to generate a file with ".yuuka" extension name.

I split the compressor and decompressor into two separate programs, both written in one single .cpp file.

I hope that I can add more functions into this project, making it a useful tool. Also, I may develop this project to learn how to develop programs with multi files. What's more, writing readme in English may be helpful in English learning.

# How to use

Compile it with C++11 or higher version.

(I probably didn't use any compiler-specific or system-specific functions, so it's likely to be cross-platformed)

Use the program with command line.

---

The compressor has 2 parameters:

1. the input file name
2. the output file name (the file extension "yuuka" will be appended later)

The input file should be in the same folder as the compressor.



The decompressor has 1 parameters:

​	the input filename (file extension "yuuka" included)

Wrong parameter number would activate the help page.

### Attention

The compressor will directly cover the file with the output file name.

The decompressor cannot work when the file has exist.

Extreme situations which generate Huffman code with 256-bit length aren't tested, and will probably lead to a crash.

~~Situations with 0 length is not tested, either.~~ This might not be a problem.

# What's more

Actually the code is mostly written by copilot(in this version), even including some of my comments.

It just popped out loads of codes when I typed my first words, and the codes are just all right when I looked through them.

It was an amazing experience. But sometimes I think it should go quiet. The grey words were like naughty kids shouting at me, telling me how to code.

I'm worrying about being replaced by AIs.

Also being worried about the copyright issue.

If possible, I want to keep the copyright for now.