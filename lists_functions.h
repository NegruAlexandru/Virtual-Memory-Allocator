#pragma once
#ifndef LISTS_FUNCTIONS_H
#define LISTS_FUNCTIONS_H

void printListInt(doublyLinkedList_t *list);
void printListChar(doublyLinkedList_t *list);
doublyLinkedList_t *createDoublyLinkedList(int dataSize);
void createNodeNthPosition(doublyLinkedList_t *list, int position, void *data, long address);
void addNodeToNthPosition(doublyLinkedList_t *list, node_t *node, int position);
node_t *removeNthPosition(doublyLinkedList_t *list, int position);
node_t *removeNodeByAddress(doublyLinkedList_t *list, long address);
node_t *removeNAddress(arrayOfLists_t *arrayOfLists, long address, int *nodeSize);
node_t *removeNeighbourNode(node_t *node, arrayOfLists_t *arrayOfLists, int *sizeOfBlock, int *sizeOfNeighbour);
void addNewEmptyNode(doublyLinkedList_t *list, int position, long address);
node_t *createEmptyNodeWithOrigin(int data_size, long address, long origin);
void bubbleSortListByAddress(doublyLinkedList_t *list);
void bubbleSortArrayOfListsBySize(doublyLinkedList_t **array, int size);
doublyLinkedList_t *getListWithSize(arrayOfLists_t *arrayOfLists, int size);
//int isNodeComplete(node_t *node, arrayOfLists_t *arrayOfLists);
int getSizeOfBlockByAddress(arrayOfLists_t *arrayOfLists, long address);
node_t *getNodeByAddress(arrayOfLists_t *arrayOfLists, long address);
int getSizeOfPartialBlockByAddress(arrayOfLists_t *arrayOfLists, long address);
char *getPartialBlockDataByAddress(arrayOfLists_t *arrayOfLists, long address);

#endif
