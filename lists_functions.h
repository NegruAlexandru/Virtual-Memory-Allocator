#pragma once
#ifndef LISTS_FUNCTIONS_H
#define LISTS_FUNCTIONS_H

void printListInt(doublyLinkedList_t *list);
void printListChar(doublyLinkedList_t *list);
doublyLinkedList_t *createDoublyLinkedList(int dataSize);
void addToNthPosition(doublyLinkedList_t *list, int position, void *data, long address);
node_t *removeNthPosition(doublyLinkedList_t *list, int position);
node_t *removeNodeByAddress(doublyLinkedList_t *list, long address);
void addNewEmptyNode(doublyLinkedList_t *list, int position, long address);
node_t *createEmptyNode(int data_size, long address);
void bubbleSortListByAddress(doublyLinkedList_t *list);
void bubbleSortArrayOfListsBySize(doublyLinkedList_t **array, int size);

#endif
