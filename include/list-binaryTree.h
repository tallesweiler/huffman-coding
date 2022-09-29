#if !defined(LIST_BINARYTREE)
#define LIST_BINARYTREE
#include "../include/binaryTree.h"

typedef struct list List;

List *constructor_list();
void destructor_list(List *list);
List *insert_list(List *list, BinaryTree *tree);
List *remove_list(List *list, BinaryTree *target);
List *find_list(List *list, BinaryTree *target);
void print_list(List *list);
void ascendingOrder_list(List *list);
int last_list(List *list);
void print_binaryTree(BinaryTree *tree);
void printGraph_binaryTree(BinaryTree *tree);
List *getNext_list(List *list);
List *getPrevious_list(List *list);
BinaryTree *getTree_list(List *list);

#endif // LIST_BINARYTREE
