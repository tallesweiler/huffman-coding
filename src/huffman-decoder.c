#include "../include/huffman-decoder.h"
#include "../include/bitmap.h"
#include "../include/binaryTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static bitmap *addBinCharPad(unsigned char c);
static unsigned int getIntFromBits(bitmap *bitmap, unsigned int start, unsigned int bits);
static void addBits(bitmap *map, bitmap *overflow, bitmap *c);
static BinaryTree *writeText(FILE *file, bitmap *map, BinaryTree *tree_aux, BinaryTree *tree, int *leafs, int leafsMax);

void huffman_decoder(char *file)
{
    int i, j;
    char fileName[100];
    char fileTemp[100];
    strcpy(fileTemp, file);
    char *pt;
    pt = strtok(fileTemp, ".");
    strcpy(fileName, pt);
    pt = strtok(NULL, ".");

    bitmap *map = bitmapInit(pow(2, 20)); // at least 1mb to read
    bitmap *overflow = bitmapInit(50);
    FILE *file_comp = fopen(file, "rb");
    FILE *file_original = NULL;
    unsigned char c;
    bitmap *dictionarySize = bitmapInit(16);
    for (i = 0; i < 2; i++)
    {
        fread(&c, sizeof(unsigned char), 1, file_comp);
        bitmap *c_bits = addBinCharPad(c);
        addBits(dictionarySize, NULL, c_bits);
        bitmapLibera(c_bits);
    }
    int dictionarySizeInt = getIntFromBits(dictionarySize, 0, 16);
    bitmap *dictionary = bitmapInit(dictionarySizeInt - 16);
    // printf("--%d--\n", bitmapGetMaxSize(dictionary));
    for (i = 0; i < (dictionarySizeInt - 16) / 8; i++)
    {
        fread(&c, sizeof(unsigned char), 1, file_comp);
        bitmap *c_bits = addBinCharPad(c);
        addBits(dictionary, NULL, c_bits);
        bitmapLibera(c_bits);
    }
    BinaryTree *tree = Constructor_binaryTreeWithotBits(0, NULL, NULL);
    BinaryTree *tree_aux = tree;
    int treeSizeInt = getIntFromBits(dictionary, 0, 9);
    // printf("%d\n%d\n", dictionarySizeInt, treeSizeInt);
    int position = 9;
    /*
    for (i = 0; i < dictionarySizeInt - 16; i++)
    {
        printf("%d", bitmapGetBit(dictionary, i));
    }
    */
    for (i = 0; i < treeSizeInt; i++)
    {
        // printf("%d\n", position);
        int sizeBits = getIntFromBits(dictionary, position, 5);
        // printf("--%d--\n", sizeBits);
        position += 5;
        // printf("%d\n", position);
        tree_aux = tree;
        for (j = 0; j < sizeBits - 1; j++)
        {
            if (bitmapGetBit(dictionary, position) == 1)
            {
                if (getRight_binaryTree(tree_aux) == NULL)
                    setRight_binaryTree(tree_aux, Constructor_binaryTreeWithotBits(0, NULL, NULL));
                tree_aux = getRight_binaryTree(tree_aux);
            }
            else
            {
                if (getLeft_binaryTree(tree_aux) == NULL)
                    setLeft_binaryTree(tree_aux, Constructor_binaryTreeWithotBits(0, NULL, NULL));
                tree_aux = getLeft_binaryTree(tree_aux);
            }
            position++;
            // printf("%d\n", position);
        }
        if (bitmapGetBit(dictionary, position) == 1)
        {
            position++;
            setRight_binaryTree(tree_aux, Constructor_binaryTreeLeafWithoutBits(1, getIntFromBits(dictionary, position, 8)));
        }
        else
        {
            position++;
            // printf("-%d-\n", getIntFromBits(dictionary, position, 8));
            setLeft_binaryTree(tree_aux, Constructor_binaryTreeLeafWithoutBits(1, getIntFromBits(dictionary, position, 8)));
        }
        position += 8;
        // printf("%d\n", position);
    }
    // print_binaryTree(tree);

    strcat(fileName, ".");
    int sizeBitsType = getIntFromBits(dictionary, position, 3);
    position += 3;
    for (i = 0; i < sizeBitsType; i++)
    {
        char cc[2] = "a";
        cc[0] = getIntFromBits(dictionary, position, 8);
        strcat(fileName, cc);
        position += 8;
    }
    // printf("%s\n", fileName);
    file_original = fopen(fileName, "wb");

    int leafs = 0, leafsMax = getIntFromBits(dictionary, position, 32);
    // printf("%d\n", leafsMax);
    tree_aux = tree;
    while (fread(&c, sizeof(unsigned char), 1, file_comp) == 1)
    {
        bitmap *c_bits = addBinCharPad(c);
        addBits(map, overflow, c_bits);
        bitmapLibera(c_bits);

        if (bitmapGetLength(map) == bitmapGetMaxSize(map))
        {
            tree_aux = writeText(file_original, map, tree_aux, tree, &leafs, leafsMax);
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
    tree_aux = writeText(file_original, map, tree_aux, tree, &leafs, leafsMax);

    tree = Destructor_binaryTreeWithoutBits(tree);
    bitmapLibera(map);
    bitmapLibera(overflow);
    bitmapLibera(dictionary);
    bitmapLibera(dictionarySize);
    fclose(file_comp);
    fclose(file_original);
}
static bitmap *addBinCharPad(unsigned char c)
{
    bitmap *bitmap = bitmapInit(8);
    for (int i = 7; i >= 0; --i)
    {
        (c & (1 << i)) ? bitmapAppendLeastSignificantBit(bitmap, 1) : bitmapAppendLeastSignificantBit(bitmap, 0);
    }
    return bitmap;
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

static unsigned int getIntFromBits(bitmap *bitmap, unsigned int start, unsigned int bits)
{
    unsigned int count = 0;
    int i = 0, j = 0;
    for (i = bits - 1; i >= 0; i--)
    {
        if (bitmapGetBit(bitmap, j + start) == 1)
        {
            count = count + pow(2, i);
        }
        j++;
    }
    return count;
}

static BinaryTree *writeText(FILE *file, bitmap *map, BinaryTree *tree_aux, BinaryTree *tree, int *leafs, int leafsMax)
{
    int i;
    for (i = 0; i < bitmapGetLength(map); i++)
    {
        if (bitmapGetBit(map, i) == 1)
        {
            tree_aux = getRight_binaryTree(tree_aux);
        }
        else
        {
            tree_aux = getLeft_binaryTree(tree_aux);
        }
        if (getValue_binaryTree(tree_aux) == 1)
        {
            if ((*leafs) < leafsMax)
            {
                char c = getChar_binaryTree(tree_aux);
                fwrite(&c, sizeof(unsigned char), 1, file);
            }
            (*leafs) += 1;
            tree_aux = tree;
        }
    }
    return tree_aux;
}