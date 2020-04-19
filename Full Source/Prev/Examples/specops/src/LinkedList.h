
#ifndef LNKLIST_H
#define LNKLIST_H

//#include <stdlib.h>

template <class nodetype> struct Node {
	nodetype val;
	Node<nodetype> *next;
};

template <class ltype> class LinkedList {
private:
	Node<ltype>* lastptr;	// <--- ONLY used in this (LinkedList) class' version of insert() method not used elsewhere
protected:
	int size;
	Node<ltype>* headptr;
public:
	LinkedList();
	~LinkedList();
	void  push(ltype val);
	ltype pop();
	void  insert(ltype val);
	ltype get(int index);
	int	  find(ltype val);	//returns FIRST index of entry with 'val' value, else returns 0 if 'val' not found
	ltype remove(int index);
	void  update(ltype val, int index);
	int   length();	//size of list
};

//implementation

template <class ltype>
LinkedList<ltype>::LinkedList() { size=0; headptr=NULL; lastptr=NULL; }
template <class ltype>
LinkedList<ltype>::~LinkedList()
{
	int i=0;
	do {
		i++;
		remove(1);
	} while (i<size);
}

template <class ltype>
void LinkedList<ltype>::push(ltype val)
{
	Node<ltype>* tmpNode=new Node<ltype>;
	tmpNode->val=val;
	tmpNode->next=headptr;
	headptr=tmpNode;
	if (size==0) lastptr=tmpNode;
	size++;
}

template <class ltype>
ltype LinkedList<ltype>::pop()
{
	return remove(1);
}

template <class ltype>
void LinkedList<ltype>::insert(ltype val)
{
	if (size==0) {
		headptr=new Node<ltype>;
		headptr->val=val;
		lastptr=headptr;
	}
	else {
		lastptr->next=new Node<ltype>;
		lastptr=lastptr->next;
		lastptr->val=val;
	}
	size++;
}

template <class ltype>
ltype LinkedList<ltype>::get(int index)
{
	if (index>0 && index<=size) {
		if (index==1) {
			return headptr->val;
		}
		else {
			int i=0;
			Node<ltype> *ptr=headptr;
			while (i<index-1) {
				i++;
				ptr=ptr->next;
			};
			return ptr->val;
		}
	}
	return NULL;
}

template <class ltype>
int LinkedList<ltype>::find(ltype val)
{
	Node<ltype>* ptr=headptr;
	int index=0;
	while (ptr!=NULL) {
		index++;
		if (ptr->val==val) return index;
		ptr=ptr->next;
	}
	return 0;	//not found
}

template <class ltype>
ltype LinkedList<ltype>::remove(int index)
{
	int i=0;
	ltype val;
	Node<ltype> *ptr=headptr, *ptr2=NULL;

	if (index>0 && index<=size) {
		if (index==1) {
			headptr=headptr->next;
			val=ptr->val;
			delete ptr;
		}
		else {
			while (i<index-2) {
				i++;
				ptr=ptr->next;
			};
			ptr2=ptr->next;
			ptr->next=ptr2->next;
			val=ptr2->val;
			delete ptr2;
		}
		size--;
		return val;
	}
	return NULL;
}

template <class ltype>
void LinkedList<ltype>::update(ltype val, int index)
{
	int i=0;
	Node<ltype> *ptr=headptr;

	if (index>0 && index<=size) {
		while (i<index-1) {
			i++;
			ptr=ptr->next;
		};
		ptr->val=val;
	}
}

template <class ltype>
int LinkedList<ltype>::length() { return size; }

#endif //LNKLIST_H

