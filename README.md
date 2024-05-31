# Introduction

**BLUE ARCHIVER** is a file archive program using simple compression algorithms, developed as a trial and exercise to enhance my coding skills.

This version is created for my \"creation and practice\" course assignment.

### function

As an early version of the program, now it can only compress/decompress one file at a time, to generate a file with ".yuuka" extension name.

It uses **LZ78** algorithm now.

### In the future

I hope that I can add more functions into this project, making it a useful tool. Also, I may develop this project to learn how to develop programs with multi files. What's more, writing readme in English may be helpful in English learning.

# How to use

Compile it with C++14 or higher version.

(I probably didn't use any compiler-specific or system-specific functions, so it's likely to be cross-platformed)

Use the program with command line.

---

Both the compressor and decompressor have 2 parameters, with the second one optional:

1. the input file name
2. the output file name (optional)

For the compressor, if the second parameter is missing, the output file name would be "the input file name with extension".yuuka

For the decompressor, now you can specify the output file name using the second parameter, or it will use the file name written in the .yuuka file.

Also, .yuuka is not needed in decompressor's input file name anymore. 

Wrong parameter number would activate the help page.

### Attention

The compressor will directly cover the file with the output file name. (Will be fixed soon.)

# What's more

## About the code

Actually the code is mostly written by copilot(in this version), even including some of my comments.

It just popped out loads of codes when I typed my first words, and the codes are just all right when I looked through them.

It was an amazing experience. But sometimes I think it should go quiet. The grey words were like naughty kids shouting at me, telling me how to code.

Actually, it also helped me to find out that most of my words or codes have just little meaning. I might have to learn much more to become a smart person.

## About the title

I just want to play a joke on the game which I am crazy about right now, Blue Archive.
Since both its simplified-Chinese community and its development team are under a tough situation, I feel a bit depressed... But it's still a lot of fun to enjoy the game, as well as the user-generated contents.

By the way, Hayase Yuuka is such an excellent candidate for my mother. Although like the ".yuuka" files, she might be a little heavier than she looks like...