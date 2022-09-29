#include "../include/binaryTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BITS 32

static void printTree(BinaryTree *tree);

struct binaryTree
{
    int value;
    unsigned char c;
    bitmap *bits;
    struct binaryTree *right;
    struct binaryTree *left;
};

BinaryTree *Constructor_binaryTreeLeaf(int value, unsigned char c)
{
    BinaryTree *new = malloc(sizeof(BinaryTree));
    new->value = value;
    new->c = c;
    new->bits = bitmapInit(MAX_BITS);
    new->right = NULL;
    new->left = NULL;
    return new;
}
BinaryTree *Constructor_binaryTree(int value, BinaryTree *left, BinaryTree *right)
{
    BinaryTree *new = malloc(sizeof(BinaryTree));
    new->value = value;
    new->c = '-';
    new->bits = bitmapInit(MAX_BITS);
    new->right = right;
    new->left = left;
    return new;
}
BinaryTree *Constructor_binaryTreeLeafWithoutBits(int value, unsigned char c)
{
    BinaryTree *new = malloc(sizeof(BinaryTree));
    new->value = value;
    new->c = c;
    new->bits = NULL;
    new->right = NULL;
    new->left = NULL;
    return new;
}
BinaryTree *Constructor_binaryTreeWithotBits(int value, BinaryTree *left, BinaryTree *right)
{
    BinaryTree *new = malloc(sizeof(BinaryTree));
    new->value = value;
    new->c = '-';
    new->bits = NULL;
    new->right = right;
    new->left = left;
    return new;
}
BinaryTree *Destructor_binaryTreeWithoutBits(BinaryTree *tree)
{
    if (tree != NULL)
    {
        if (tree->left != NULL)
            Destructor_binaryTreeWithoutBits(tree->left);
        if (tree->right != NULL)
            Destructor_binaryTreeWithoutBits(tree->right);
        free(tree);
    }
    return NULL;
}
BinaryTree *Destructor_binaryTree(BinaryTree *tree)
{
    if (tree != NULL)
    {
        if (tree->left != NULL || tree->right) // not leaf
            bitmapLibera(tree->bits);
        if (tree->left != NULL)
            Destructor_binaryTree(tree->left);
        if (tree->right != NULL)
            Destructor_binaryTree(tree->right);
        free(tree);
    }
    return NULL;
}

// imprime as informações dos nós da árvore
void print_binaryTree(BinaryTree *tree)
{
    if (tree != NULL)
    {
        if (tree->right == NULL && tree->left == NULL)
        {
            printf("--%.3d--(%d)\n", tree->c, bitmapGetLength(tree->bits));
        }

        if (tree->left != NULL)
        {
            print_binaryTree(tree->left);
        }
        if (tree->right != NULL)
        {
            print_binaryTree(tree->right);
        }
    }
}

void printGraph_binaryTree(BinaryTree *tree)
{
    printf("\nstrict graph{\n");
    printTree(tree);
    printf("}\n");
    return;
}

void fillBits_binaryTree(BinaryTree *tree)
{
    if (tree != NULL)
    {
        if (tree->left != NULL)
        {
            int i;
            for (i = 0; i < bitmapGetLength(tree->bits); i++)
            {
                bitmapAppendLeastSignificantBit(tree->left->bits, bitmapGetBit(tree->bits, i));
            }
            bitmapAppendLeastSignificantBit(tree->left->bits, 0);
            fillBits_binaryTree(tree->left);
        }
        if (tree->right != NULL)
        {
            int i;
            for (i = 0; i < bitmapGetLength(tree->bits); i++)
            {
                bitmapAppendLeastSignificantBit(tree->right->bits, bitmapGetBit(tree->bits, i));
            }
            bitmapAppendLeastSignificantBit(tree->right->bits, 1);
            fillBits_binaryTree(tree->right);
        }
    }
}

static void printTree(BinaryTree *tree)
{

    if (tree != NULL)
    {
        if (tree->left != NULL)
        {
            printf("arv(%c) -- arv(%c)\n", tree->c, tree->left->c);
        }
        if (tree->right != NULL)
        {
            printf("arv(%c) -- arv(%c)\n", tree->c, tree->right->c);
        }
        printTree(tree->left);
        printTree(tree->right);
    }
}

int getValue_binaryTree(BinaryTree *tree)
{
    return tree->value;
}

unsigned char getChar_binaryTree(BinaryTree *tree)
{
    return tree->c;
}

bitmap *getBits_binaryTree(BinaryTree *tree)
{
    return tree->bits;
}

BinaryTree *getLeft_binaryTree(BinaryTree *tree)
{
    return tree->left;
}

BinaryTree *getRight_binaryTree(BinaryTree *tree)
{
    return tree->right;
}

void setLeft_binaryTree(BinaryTree *tree, BinaryTree *left)
{
    tree->left = left;
}
void setRight_binaryTree(BinaryTree *tree, BinaryTree *right)
{
    tree->right = right;
}