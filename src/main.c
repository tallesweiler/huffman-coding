#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/huffman-encoder.h"
#include "../include/huffman-decoder.h"

void start_encoder(char *file);

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("ERROR: UNSPECIFIED FILE OR MODE\n");
        return 1;
    }

    if (!strcmp(argv[1], "-encoder"))
    {
        huffman_encoder(argv[2]);
    }
    else if (!strcmp(argv[1], "-decoder"))
    {
        huffman_decoder(argv[2]);
    }
    else
    {
        printf("ERROR!\n");
        return 1;
    }
    return 0;
}