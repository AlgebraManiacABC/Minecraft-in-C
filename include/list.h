/********************************************************************************
 * linked_list.h
 * 
 ********************************************************************************/

#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <stdbool.h>
#include <stddef.h>

typedef struct Node_s Node_t;
typedef struct List_s List_t;

/**
 * Creates a new (empty) Linked List
 * @returns the new `List_t *`
 */
List_t * createList();

/**
 * Gets the list's node count
 * @param list the list to query
 * @returns the number of nodes in the provided list
 */
size_t getListLength(List_t * list);

/**
 * Checks if the list is empty
 * @param list the list to query
 * @returns True if the list is empty, else False
 */
bool listIsEmpty(List_t * list);

/**
 * Destroys all nodes of and
 * frees all memory associated with a list
 * @param list the list to destroy
 */
void destroyList(List_t * list);

/**
 * Inserts a node at the head of the list,
 *  filling it with the specified data
 * @param list the list to change
 * @param data the data to insert
 * @returns EXIT_SUCCESS or EXIT_FAILURE
 */
int insertNodeAtHead(List_t * list, void * data);

/**
 * Inserts a node at the end of the list,
 *  filling it with the specified data
 * @param list the list to change
 * @param data the data insert
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int insertNodeAtTail(List_t * list, void * data);

/**
 * Gets the data from the node at the given index
 * @param list the list from which to get data
 * @param index the index of the data node
 * @returns the data at the given index
 */
void * getNodeAtIndex(List_t * list, size_t index);

/**
 * Gets the data from the head node of the list
 * @param list the list from which to get data
 * @return the data at the head node
 */
void * getNodeAtHead(List_t * list);

/**
 * Gets the data from the tail (final) node of the list
 * @param list the list from which to get data
 * @return the data at the tail node
 */
void * getNodeAtTail(List_t * list);

/**
 * Remove a node by index, returning its data
 * @param list the list to change
 * @param index the index of the node to delete
 * @return the data from the deleted node
 */
void * removeNodeAtIndex(List_t * list, size_t index);

/**
 * Remove the head node of a list, returning its data
 * @param list the list to change
 * @return the data from the deleted node
 */
void * removeNodeAtHead(List_t * list);

/**
 * Remove the tail (final) node of a list, returning its data
 * @param list the list to change
 * @return the data from the deleted node
 */
void * removeNodeAtTail(List_t * list);

/**
 * Searches through the list for a node with matching data,
 *  then removes it.
 * @param list the list to change
 * @param data a pointer to compare with each node
 * @returns EXIT_SUCCESS on node removal, else EXIT_FAILURE (see getError)
 */
int removeNodeMatchingData(List_t * list, void * data);

/**
 * Create a new node with data at the end of a list
 * @param list the list to change
 * @param data the data to append
 */
void pushObject(List_t * list, void * data);

/**
 * Destroy the last node at the end of a list, returning its data
 * @param list the list to change
 * @returns the data extracted from the node
 */
void * popObject(List_t * list);

#endif