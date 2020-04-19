
#ifndef SLNKLIST_H
#define SLNKLIST_H

#include "LinkedList.h"

// SortedLinkedList //

template <class ltype> class SortedLinkedList:public LinkedList<ltype> {
public:
	SortedLinkedList();
	void  insert(ltype val);
	void  update(ltype val, int index);
};

// implementation

template <class ltype>
SortedLinkedList<ltype>::SortedLinkedList() { LinkedList<ltype>(); }

template <class ltype>
void SortedLinkedList<ltype>::insert(ltype val)
{
	Node<ltype>* newNode=new Node<ltype>();
	newNode->val=val;

	if (size==0 || headptr->val>=val) {
		newNode->next=headptr;
		headptr=newNode;
		size++;
		return;
	}
	//at least 2 elements exist...
	Node<ltype>* prevNode=headptr;
	while (prevNode->next!=NULL && prevNode->next->val<val) {
		prevNode=prevNode->next;
	}
	newNode->next=prevNode->next;
	prevNode->next=newNode;
	size++;
}

template <class ltype>
void SortedLinkedList<ltype>::update(ltype val, int index)
{
	//remove and re-insert to update position
	remove(index);
	insert(val);
}


#endif	//SLNKLIST_H
