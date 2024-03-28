#include "lists_structs.h"

#pragma once

#ifndef LISTS_FUNCTIONS_H
#define LISTS_FUNCTIONS_H

doublyLinkedList_t *createDoublyLinkedList(int dataSize);
void createNodeNthPosition(doublyLinkedList_t *list, int position, long address);
void addNodeToNthPosition(doublyLinkedList_t *list, node_t *node, int position);
node_t *remove_nth_position(doublyLinkedList_t *list, int position);
node_t *removeNodeByAddress(doublyLinkedList_t *list, long address);
node_t *removeNAddress(array_of_lists_t *arrayOfLists, long address, int *nodeSize);
node_t *removeNeighbourNode(node_t *node, array_of_lists_t *arrayOfLists, int *sizeOfBlock, int *sizeOfNeighbour);
void addNewEmptyNode(doublyLinkedList_t *list, int position, long address);
node_t *createEmptyNodeWithOrigin(int data_size, long address, long origin);
void sort_by_address(doublyLinkedList_t *list);
void bubbleSortArrayOfListsBySize(array_of_lists_t *array_of_lists);
doublyLinkedList_t *getListWithSize(array_of_lists_t *arrayOfLists, int size);
//int isNodeComplete(node_t *node, array_of_lists_t *arrayOfLists);
int getSizeOfBlockByAddress(array_of_lists_t *arrayOfLists, long address);
node_t *getNodeByAddress(array_of_lists_t *arrayOfLists, long address);
int getSizeOfPartialBlockByAddress(array_of_lists_t *arrayOfLists, long address);
char *getPartialBlockDataByAddress(array_of_lists_t *arrayOfLists, long address);
long getMinAddress(array_of_lists_t *arrayOfLists);

#endif
