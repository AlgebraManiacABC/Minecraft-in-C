/********************************************************************************
 * linked_list.c
 * Goal is to create a circularly-linked list:
 *   o<->o<->o
 *  /         \
 * o           o <- head
 *  \         /
 *   o<->o<->o
 ********************************************************************************/

#include <stdlib.h>
#include "../include/list.h"

struct Node_s
{
    Node_t * prev;
    Node_t * next;
    void * data;
};

struct List_s
{
    Node_t * head;
    size_t length;
};

List_t * createList()
{
    // calloc sets everything to NULL or 0
    List_t * list = calloc(1, sizeof(List_t));
    return list;
}

Node_t * createNode(void * data)
{
    // calloc sets next/pref to NULL
    Node_t * node = calloc(1, sizeof(Node_t));
    if (!node)
    {
        return NULL;
    }
    node->data = data;
    return node;
}

size_t getListLength(List_t * list)
{
    if (!list) return 0;
    return list->length;
}

bool listIsEmpty(List_t * list)
{
    if (!list) return true;
    return list->head == NULL;
}

void destroyList(List_t * list)
{
    if (!list) return;
    while (list->head) removeNodeAtHead(list);
    free(list);
}

int insertNodeAtHead(List_t * list, void * data)
{
    if (!list)
    {
        return EXIT_FAILURE;
    }
    Node_t * node = createNode(data);
    if (!node) return EXIT_FAILURE;
    if (!list->head)
    {
        list->head = node;
        node->next = node;
        node->prev = node;
    }
    else
    {
        list->head->prev->next = node;
        list->head->prev = node;
        list->head = node;
    }
    list->length++;
    return EXIT_SUCCESS;
}

int insertNodeAtTail(List_t * list, void * data)
{
    if (!list)
    {
        return EXIT_FAILURE;
    }
    Node_t * node = createNode(data);
    if (!node) return EXIT_FAILURE;
    if (!list->head)
    {
        list->head = node;
        node->next = node;
        node->prev = node;
    }
    else
    {
        Node_t * penultimate = list->head->prev;
        penultimate->next = node;
        node->prev = penultimate;
        node->next = list->head;
        list->head->prev = node;
    }
    list->length++;
    return EXIT_SUCCESS;
}

// void * getNodeAtIndex(List_t * list, size_t index);

// void * getNodeAtHead(List_t * list);

// void * getNodeAtTail(List_t * list);

// void * removeNodeAtIndex(List_t * list, size_t index);

void * removeNodeAtHead(List_t * list)
{
    if (!list || !list->length) return NULL;
    void * data = list->head->data;
    list->head->prev->next = list->head->next;
    list->head->next->prev = list->head->prev;
    Node_t * temp = NULL;
    if (list->length > 1) temp = list->head->next;
    free(list->head);
    list->head = temp;
    list->length--;
    return data;
}

// void * removeNodeAtTail(List_t * list);

int removeNodeMatchingData(List_t * list, void * data)
{
    if (!list || !list->length) return EXIT_FAILURE;
    Node_t * search = list->head;
    for (int i=0; i<list->length; i++)
    {
        if (search->data == data)
        {
            search->prev->next = search->next;
            search->next->prev = search->prev;
            if (search == list->head) list->head = search->next;
            list->length--;
            free(search);
            if (list->length == 0) list->head = NULL;
            return EXIT_SUCCESS;
        }
        search = search->next;
    }
    return EXIT_FAILURE;
}

// void pushObject(List_t * list, void * data);

// void * popObject(List_t * list);