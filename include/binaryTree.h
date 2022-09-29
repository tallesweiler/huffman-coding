#if !defined(BINARY_TREE)
#define BINARY_TREE

#include "../include/bitmap.h"

typedef struct binaryTree BinaryTree;

BinaryTree *Constructor_binaryTreeLeaf(int value, unsigned char c);
BinaryTree *Constructor_binaryTree(int value, BinaryTree *left, BinaryTree *right);
BinaryTree *Constructor_binaryTreeLeafWithoutBits(int value, unsigned char c);
BinaryTree *Constructor_binaryTreeWithotBits(int value, BinaryTree *left, BinaryTree *right);
BinaryTree *Destructor_binaryTree(BinaryTree *tree);
BinaryTree *Destructor_binaryTreeWithoutBits(BinaryTree *tree);
int getValue_binaryTree(BinaryTree *tree);
unsigned char getChar_binaryTree(BinaryTree *tree);
bitmap *getBits_binaryTree(BinaryTree *tree);
BinaryTree *getLeft_binaryTree(BinaryTree *tree);
BinaryTree *getRight_binaryTree(BinaryTree *tree);
void setLeft_binaryTree(BinaryTree *tree, BinaryTree *left);
void setRight_binaryTree(BinaryTree *tree, BinaryTree *right);
void fillBits_binaryTree(BinaryTree *tree);
void print_binaryTree(BinaryTree *tree);

#endif // BINARY_TREE
