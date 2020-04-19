
#ifndef SOLNKLIST_H
#define SOLNKLIST_H

#include "LinkedList.h"
#include "SortedLinkedList.h"

// SortedObjectLinkedList //

// Interface IComparable is supported by derived classes which wish to use the SortedObjectLinkedList class
class IComparable {
public:
	__forceinline virtual int getPriority() = 0;
};

class SortedObjectLinkedList: public LinkedList<IComparable*> {
public:
	SortedObjectLinkedList();
	void  insert(IComparable* val);
	void  update(IComparable* val, int index);
	void  update(IComparable* val);
};

SortedObjectLinkedList::SortedObjectLinkedList() { LinkedList<IComparable*>(); }

void SortedObjectLinkedList::insert(IComparable* val)
{
	Node<IComparable*>* newNode=new Node<IComparable*>();
	newNode->val=val;

	if (size==0 || headptr->val->getPriority()>=val->getPriority()) {
		newNode->next=headptr;
		headptr=newNode;
		size++;
		return;
	}
	//at least 2 elements exist...
	Node<IComparable*>* prevNode=headptr;
	while (prevNode->next!=NULL && prevNode->next->val->getPriority()<val->getPriority()) {
		prevNode=prevNode->next;
	}
	newNode->next=prevNode->next;
	prevNode->next=newNode;
	size++;
}

void SortedObjectLinkedList::update(IComparable* val, int index)
{
	//remove and re-insert to update
	remove(index);
	insert(val);
}
void SortedObjectLinkedList::update(IComparable* val)
{
	int index=find(val);
	if (index==0) return;
	update(val,index);
}

#endif	//SOLNKLIST_H

