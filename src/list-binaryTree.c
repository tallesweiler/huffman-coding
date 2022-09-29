#include "../include/list-binaryTree.h"
#include <stdio.h>
#include <stdlib.h>

struct list
{
    BinaryTree *tree;
    struct list *next;
    struct list *previous;
};

List *constructor_list()
{
    return NULL;
}

void destructor_list(List *list)
{
    List *temp = list;
    List *temp_next = NULL;
    /*PROCURA O ULTIMO ELEMENTO DA list*/
    while (temp != NULL)
    {
        temp_next = temp->next;
        free(temp);
        temp = temp_next;
    }
}

List *insert_list(List *list, BinaryTree *tree)
{
    List *newList = malloc(sizeof(List));
    newList->tree = tree;
    newList->next = NULL;

    /*CASO A list ESTEJA VAZIA*/
    if (list == NULL)
    {
        list = newList;
        list->previous = NULL;
    }
    else
    {
        List *temp = list;

        /*PROCURA O ULTIMO ELEMENTO DA list*/
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        /*INSERE A NOVA list*/
        temp->next = newList;
        newList->previous = temp;
    }
    return list;
}

List *remove_list(List *list, BinaryTree *target)
{
    List *listTarget = find_list(list, target);
    if (listTarget != NULL)
    {
        /*CASO SEJA O UNICO DA list*/
        if (listTarget->previous == NULL && listTarget->next == NULL)
        {
            list = NULL;
        }
        /*CASO SEJA O PRIMEIRO DA list*/
        else if (listTarget->previous == NULL)
        {
            (listTarget->next)->previous = NULL;
            list = listTarget->next;
        }
        /*CASO SEJA O ULTIMO DA list*/
        else if (listTarget->next == NULL)
        {
            (listTarget->previous)->next = NULL;
        }
        else
        {
            (listTarget->previous)->next = listTarget->next;
            (listTarget->next)->previous = listTarget->previous;
        }
        free(listTarget);
    }
    return list;
}

List *find_list(List *list, BinaryTree *target)
{
    List *temp = list;
    while (temp != NULL && temp->tree != target)
    {
        temp = temp->next;
    }
    if (temp == NULL)
    {
        return NULL;
    }
    return temp;
}

void print_list(List *list)
{
    List *temp = list;
    while (temp != NULL)
    {
        printf("%c ", getChar_binaryTree(temp->tree));
        temp = temp->next;
    }
}

void ascendingOrder_list(List *list)
{
    List *temp = list;
    while (temp != NULL)
    {
        int lowerValue = getValue_binaryTree(temp->tree);
        List *lowerValueTree = temp;

        List *temp2 = temp->next;
        while (temp2 != NULL)
        {
            if (getValue_binaryTree(temp2->tree) < lowerValue)
            {
                lowerValue = getValue_binaryTree(temp2->tree);
                lowerValueTree = temp2;
            }
            temp2 = temp2->next;
        }
        if (temp != lowerValueTree)
        {
            BinaryTree *l = temp->tree;
            temp->tree = lowerValueTree->tree;
            lowerValueTree->tree = l;
        }

        temp = temp->next;
    }
}

int last_list(List *list)
{
    if (list->next == NULL && list->previous == NULL)
        return 1;
    else
        return 0;
}

List *getNext_list(List *list)
{
    return list->next;
}

List *getPrevious_list(List *list)
{
    return list->previous;
}

BinaryTree *getTree_list(List *list)
{
    return list->tree;
}