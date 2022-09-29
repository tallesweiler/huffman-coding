#include "../include/huffman-encoder.h"
#include "../include/binaryTree.h"
#include "../include/list-binaryTree.h"
#include "../include/dictionary.h"
#include "../include/bitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static void addBits(bitmap *map, bitmap *overflow, bitmap *c);

void huffman_encoder(char *file)
{
    char fileType[10], fileName[100];

    char fileTemp[100];
    strcpy(fileTemp, file);
    char *pt;
    pt = strtok(fileTemp, ".");
    strcpy(fileName, pt);
    strcat(fileName, ".comp");
    pt = strtok(NULL, ".");
    strcpy(fileType, pt);

    Dictionary *dictionary = constructor_dictionary(file, fileType);
    bitmap *map = bitmapInit(pow(2, 20)); // at least 1mb to write
    bitmap *overflow = bitmapInit(50);

    fillBitmap(map, dictionary, fileType);
    /*
    for (int i = 0; i < bitmapGetLength(map); i++)
    {
        printf("%d", bitmapGetBit(map, i));
    }
    */

    unsigned char c;
    FILE *file_original = fopen(file, "rb");
    FILE *file_comp = fopen(fileName, "wb");

    while (fread(&c, sizeof(unsigned char), 1, file_original) == 1)
    {
        bitmap *c_bits = getCharFromDictionary(dictionary, c);
        addBits(map, overflow, c_bits);
        bitmapLibera(c_bits);

        if (bitmapGetLength(map) == bitmapGetMaxSize(map))
        {
            fwrite(bitmapGetContents(map), 1, bitmapGetLength(map) / 8, file_comp);
            bitmapLibera(map);
            map = bitmapInit(pow(2, 20));
            int i;
            for (i = 0; i < bitmapGetLength(overflow); i++)
            {
                bitmapAppendLeastSignificantBit(map, bitmapGetBit(overflow, i));
            }
            bitmapLibera(overflow);
            overflow = bitmapInit(50);
        }
    }

    int ignore = 8 - bitmapGetLength(map) % 8;
    int i;
    for (i = 0; i < ignore; i++)
        bitmapAppendLeastSignificantBit(map, 0);
    fwrite(bitmapGetContents(map), 1, bitmapGetLength(map) / 8, file_comp);
    bitmapLibera(map);
    bitmapLibera(overflow);

    fclose(file_original);
    fclose(file_comp);
    destructor_dictionary(dictionary);
}

static void addBits(bitmap *map, bitmap *overflow, bitmap *c)
{
    int i;
    for (i = 0; i < bitmapGetLength(c); i++)
    {
        if (bitmapGetLength(map) < bitmapGetMaxSize(map))
            bitmapAppendLeastSignificantBit(map, bitmapGetBit(c, i));
        else
            bitmapAppendLeastSignificantBit(overflow, bitmapGetBit(c, i));
    }
}