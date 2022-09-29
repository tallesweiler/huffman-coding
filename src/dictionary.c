#include "../include/dictionary.h"
#include "../include/list-binaryTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_WORDS 256

typedef struct word
{
    unsigned char c;
    bitmap *bits;
    bitmap *sizeBits;
} Word;

struct dictionary
{
    Word words[MAX_WORDS];
    bitmap *sizeWords;
    bitmap *sizeFile;
};

static void getText(int *ascii, char *file);
static void addBinCharPad(unsigned char c, bitmap *bitmap);
static void fillDictionary(BinaryTree *tree, Dictionary *dictionary);
static bitmap *fillSizeBits(unsigned int sizeBits, unsigned int bits);
static void fillSizeBits2(bitmap *bitmap, unsigned int sizeBits, unsigned int bits);
void fillBitmap(bitmap *bitmap, Dictionary *dictionary, char *type);

Dictionary *constructor_dictionary(char *file, char *type)
{
    Dictionary *dictionary = malloc(sizeof(Dictionary));

    int ascii[MAX_WORDS] = {0};
    getText(ascii, file);

    List *list = constructor_list();
    int i, lenght = 0;
    unsigned int numberChars = 0;
    for (i = 0; i < MAX_WORDS; i++)
    {
        dictionary->words[i].bits = NULL;
        dictionary->words[i].sizeBits = NULL;

        if (ascii[i] > 0)
        {
            list = insert_list(list, Constructor_binaryTreeLeaf(ascii[i], i));
            lenght++;
            numberChars = numberChars + ascii[i];
        }
    }

    while (last_list(list) == 0)
    {
        ascendingOrder_list(list);

        BinaryTree *first = getTree_list(list);
        BinaryTree *second = getTree_list(getNext_list(list));

        BinaryTree *newTree = Constructor_binaryTree(getValue_binaryTree(first) + getValue_binaryTree(second), first, second);

        list = insert_list(list, newTree);
        list = remove_list(list, first);
        list = remove_list(list, second);
    }
    fillBits_binaryTree(getTree_list(list));
    fillDictionary(getTree_list(list), dictionary);
    // print_binaryTree(getTree_list(list));
    Destructor_binaryTree(getTree_list(list));
    destructor_list(list);
    dictionary->sizeWords = fillSizeBits(lenght, 9);
    dictionary->sizeFile = fillSizeBits(numberChars, 32);

    return dictionary;
}

void destructor_dictionary(Dictionary *dictionary)
{
    bitmapLibera(dictionary->sizeFile);
    bitmapLibera(dictionary->sizeWords);

    int i;
    for (i = 0; i < MAX_WORDS; i++)
    {
        if (dictionary->words[i].bits != NULL)
        {
            bitmapLibera(dictionary->words[i].bits);
            bitmapLibera(dictionary->words[i].sizeBits);
        }
    }
    free(dictionary);
}

bitmap *getCharFromDictionary(Dictionary *dictionary, unsigned char c)
{
    bitmap *map = bitmapInit(bitmapGetLength(dictionary->words[(int)c].bits));
    int i;
    for (i = 0; i < bitmapGetLength(dictionary->words[(int)c].bits); i++)
    {
        bitmapAppendLeastSignificantBit(map, bitmapGetBit(dictionary->words[(int)c].bits, i));
    }
    return map;
}

static void getText(int *ascii, char *file)
{
    FILE *text = fopen(file, "rb");
    unsigned char c;
    while (fread(&c, sizeof(unsigned char), 1, text))
    {
        ascii[(unsigned int)c]++;
    }
    fclose(text);
}

static void fillDictionary(BinaryTree *tree, Dictionary *dictionary)
{
    if (tree != NULL)
    {
        if (getLeft_binaryTree(tree) == NULL && getRight_binaryTree(tree) == NULL && getChar_binaryTree(tree) >= 0)
        {
            bitmap *sizeBits = fillSizeBits(bitmapGetLength(getBits_binaryTree(tree)), 5);
            dictionary->words[(unsigned int)getChar_binaryTree(tree)].sizeBits = sizeBits;
            dictionary->words[(unsigned int)getChar_binaryTree(tree)].c = getChar_binaryTree(tree);
            dictionary->words[(unsigned int)getChar_binaryTree(tree)].bits = getBits_binaryTree(tree);
        }
        else
        {
            fillDictionary(getLeft_binaryTree(tree), dictionary);
            fillDictionary(getRight_binaryTree(tree), dictionary);
        }
    }
}

static bitmap *fillSizeBits(unsigned int sizeBits, unsigned int bits)
{
    bitmap *new = bitmapInit(bits);

    int i;
    for (i = bits - 1; i >= 0; i--)
    {
        if (sizeBits >= pow(2, i))
        {
            bitmapAppendLeastSignificantBit(new, 1);
            sizeBits = sizeBits - pow(2, i);
        }
        else
            bitmapAppendLeastSignificantBit(new, 0);
    }

    return new;
}

static void fillSizeBits2(bitmap *bitmap, unsigned int sizeBits, unsigned int bits)
{
    int i;
    for (i = bits - 1; i >= 0; i--)
    {
        if (sizeBits >= pow(2, i))
        {
            bitmapAppendLeastSignificantBit(bitmap, 1);
            sizeBits = sizeBits - pow(2, i);
        }
        else
            bitmapAppendLeastSignificantBit(bitmap, 0);
    }
}

void fillBitmap(bitmap *bitmap, Dictionary *dictionary, char *type)
{
    int i, j, count = 0, typeBytes = strlen(type);

    for (i = 0; i < MAX_WORDS; i++)
    {
        if (dictionary->words[i].bits != NULL)
        {
            count = count + 5 + bitmapGetLength(dictionary->words[i].bits) + 8;
        }
    }

    int sizeOfChars = 16 + 9 + count + (3 + typeBytes * 8) + 32;
    int ignore = 8 - sizeOfChars % 8;

    fillSizeBits2(bitmap, sizeOfChars + ignore, 16);

    for (j = 0; j < 9; j++)
    {
        bitmapAppendLeastSignificantBit(bitmap, bitmapGetBit(dictionary->sizeWords, j));
    }
    for (i = 0; i < MAX_WORDS; i++)
    {
        if (dictionary->words[i].bits != NULL)
        {
            for (j = 0; j < 5; j++)
            {
                bitmapAppendLeastSignificantBit(bitmap, bitmapGetBit(dictionary->words[i].sizeBits, j));
            }
            for (j = 0; j < bitmapGetLength(dictionary->words[i].bits); j++)
            {
                bitmapAppendLeastSignificantBit(bitmap, bitmapGetBit(dictionary->words[i].bits, j));
            }
            addBinCharPad(dictionary->words[i].c, bitmap);
        }
    }
    fillSizeBits2(bitmap, typeBytes, 3);
    for (i = 0; i < typeBytes; i++)
    {
        addBinCharPad(type[i], bitmap);
    }

    for (j = 0; j < 32; j++)
    {
        bitmapAppendLeastSignificantBit(bitmap, bitmapGetBit(dictionary->sizeFile, j));
    }
    for (i = 0; i < ignore; i++)
        bitmapAppendLeastSignificantBit(bitmap, 0);
}

static void addBinCharPad(unsigned char c, bitmap *bitmap)
{
    for (int i = 7; i >= 0; --i)
    {
        (c & (1 << i)) ? bitmapAppendLeastSignificantBit(bitmap, 1) : bitmapAppendLeastSignificantBit(bitmap, 0);
    }
}