#pragma once
#ifndef LISTS_FUNCTIONS_H
#define LISTS_FUNCTIONS_H

void printListInt(doublyLinkedList_t *list);
void printListChar(doublyLinkedList_t *list);
doublyLinkedList_t *createDoublyLinkedList(int data_size);
void addToNthPosition(doublyLinkedList_t *list, int position, void *data);
node_t *removeNthPosition(doublyLinkedList_t *list, int position);
void addNewEmptyNode(doublyLinkedList_t *list, int position, int address);

#endif
