#if !defined(DICTIONARY)
#define DICTIONARY

#include <stdio.h>
#include "../include/bitmap.h"

typedef struct dictionary Dictionary;

Dictionary *constructor_dictionary(char *file, char *type);
void destructor_dictionary(Dictionary *dictionary);
void printDictionary(Dictionary *dictionary);
void fillBitmap(bitmap *bitmap, Dictionary *dictionary, char *type);
bitmap *getCharFromDictionary(Dictionary *dictionary, unsigned char c);

#endif // DICTIONARY
