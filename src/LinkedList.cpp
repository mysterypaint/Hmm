#include <cstdlib>
#include <iostream>

#include "LinkedList.hpp"

using namespace std;

List::List() {
	head = NULL;
	curr = NULL;
	temp = NULL;
}

void List::AddNode(int addData) {
	nodePtr n = new node;	// Create a new node pointer. n points to the new node
	n->next = NULL;
	n->data = addData;

	if (head != NULL) {		// If we have some list, intact already
			curr = head;	// Put the current pointer to the front

		while (curr->next != NULL) { // Keep setting the current pointer until it's at the very end of the list, at the last node
			curr = curr->next;
		}

		curr->next = n;		// Once it gets there, connect the last node to the one that we just created
	} else { 				// Otherwise, if we don't have a list...
		head = n;			// ...Make n be the front of the list
	}
}

void List::DeleteNode(int delData) {
	nodePtr delPtr = NULL;
	temp = head;
	curr = head;

	while (curr != NULL && curr->data != delData) {
		temp = curr;
		curr = curr->next;
	}
	if (curr == NULL) {
		printf("%d was not in the list\n", delData);
		delete delPtr;
	} else {
		delPtr = curr;
		curr = curr->next;
		temp->next = curr;
		if (delPtr == head) {
			head = head->next;
			temp = NULL;
		}
		delete delPtr;
		printf("The value %d was deleted\n", delData);
	}
}

void List::PrintList() {
	curr = head;
	while(curr != NULL) {
		printf("%d\n", curr->data);
		curr = curr->next;
	}
}

void List::PrintListSameLine() {
	curr = head;
	while(curr != NULL) {
		printf("%d, ", curr->data);
		curr = curr->next;
	}
}