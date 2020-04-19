

int ListInit(LPOBJECTCOL* lpItemcol)
{
	*lpItemcol=(LPOBJECTCOL)malloc(sizeof(OBJECTCOL));
	(*lpItemcol)->size=0;
	(*lpItemcol)->sizeAllocd=0;
	(*lpItemcol)->list=NULL;
	return SUCCESS;
}

int ListAdd(LPOBJECTCOL itemcol, LPOBJECT item)
{
	if (itemcol==NULL) return FAILURE;
	//check if the new size overflows list size (if so, reallocate a large list arr)
	int newsize=++itemcol->size;
	if (newsize > itemcol->sizeAllocd) {
		itemcol->sizeAllocd+=LIST_INC_SIZE;
		OBJECT** newlist;
		if ((newlist=(OBJECT**)malloc(sizeof(LPOBJECT)*itemcol->sizeAllocd))==NULL)
			return FAILURE;
		for (int i=0; i<newsize-1; i++) {
			newlist[i]=itemcol->list[i];
		}
		free(itemcol->list);	//kill old list
		itemcol->list=newlist;	//assign to new list
	}
	//add pItem to list
	itemcol->list[newsize-1]=item;
	return SUCCESS;
}

//preconditions: need to check if item is in list
int ListDelete(LPOBJECTCOL itemcol, LPOBJECT item)
{
	if (itemcol==NULL) return FAILURE;
	int i;
	OBJECT** newlist;
	if ((newlist=(OBJECT**)malloc(sizeof(LPOBJECT)*itemcol->sizeAllocd))==NULL)
		return FAILURE;
	//find item in itemcol
	int ind=0; //newlist index
	for (i=0; i<itemcol->size; i++) {
		if (item!=itemcol->list[i])
			newlist[ind++]=itemcol->list[i];
	}
	free(itemcol->list);	//kill old list
	itemcol->list=newlist;	//assign to new list
	itemcol->size--;		//decr size
	free(item);				//kill item
	return SUCCESS;
}
//preconditions: need to check if item is in list
LPOBJECT ListRemove(LPOBJECTCOL itemcol, LPOBJECT item)
{
	if (itemcol==NULL) return FAILURE;
	int i;
	OBJECT** newlist;
	if ((newlist=(OBJECT**)malloc(sizeof(LPOBJECT)*itemcol->sizeAllocd))==NULL)
		return NULL;
	//find item in itemcol
	int ind=0; //newlist index
	for (i=0; i<itemcol->size; i++) {
		if (item!=itemcol->list[i])
			newlist[ind++]=itemcol->list[i];
	}
	free(itemcol->list);	//kill old list
	itemcol->list=newlist;	//assign to new list
	itemcol->size--;		//decr size
	return (item);			//return item
}

//checks if a particular item belongs to the specified list
INLINE int ListContainsItem(LPOBJECTCOL itemcol, LPOBJECT item)
{
	int i;
	for (i=0; i<itemcol->size; i++) {
		if (itemcol->list[i]==item)
			return 1;
	}
	return 0;
}
//checks if at least one occurence of an item equal to the specified type exists in the list
int ListItemTypeExists(LPOBJECTCOL itemcol, int type)
{
	int i;
	for (i=0; i<itemcol->size; i++) {
		if (itemcol->list[i]->type==type)
			return 1;
	}
	return 0;
}

//checks if an occurence of an item with the specified uid exists in the list
// and returns the first one found (there should only be one), or NULL if does not exist
LPOBJECT ListItemUIDExists(LPOBJECTCOL itemcol, int uid)
{
	int i;
	for (i=0; i<itemcol->size; i++) {
		if (itemcol->list[i]->uid==uid)
			return (itemcol->list[i]);
	}
	return NULL;
}
//gets next "class" of item type specified, starting from item passed in (NULL if start at beginning)
LPOBJECT ListEnumItemClass(LPOBJECTCOL itemcol, int typeClass, LPOBJECT startItem)
{
	int i,startInd,visitStart;
	int size=itemcol->size;
	if (size==0) return NULL;
	if (startItem==NULL || !ListContainsItem(itemcol,startItem)) {
		startInd=0;
	}
	else {
		for (i=0; i<itemcol->size; i++) {
			if (itemcol->list[i]==startItem) {
				//start at next item
				startInd=(i+1)%size; break;
			}
		}
	}
	//startInd is guaranteed to pt to an item as a starting position in itemcol list
	//now return first occurrence of typeClass items encountered
	visitStart=0;
	for (i=startInd; 1; i=(i+1)%size) {
		if (visitStart && i==startInd) break;
		if (i==startInd) visitStart=1;
		if (itemcol->list[i]->type & typeClass) {
			return (itemcol->list[i]);
		}
	}
	return NULL;	//at this pt startInd was visited again and therefore no item of typeClass exists in list at all
}

//gets next item type specified, starting from item passed in (NULL if start at beginning)
LPOBJECT ListEnumItemType(LPOBJECTCOL itemcol, int type, LPOBJECT startItem)
{
	int i,startInd,visitStart;
	int size=itemcol->size;
	if (size==0) return NULL;
	if (startItem==NULL || !ListContainsItem(itemcol,startItem)) {
		startInd=0;
	}
	else {
		for (i=0; i<itemcol->size; i++) {
			if (itemcol->list[i]==startItem) {
				//start at next item
				startInd=(i+1)%size; break;
			}
		}
	}
	//startInd is guaranteed to pt to an item as a starting position in itemcol list
	//now return first occurrence of type items encountered
	visitStart=0;
	for (i=startInd; 1; i=(i+1)%size) {
		if (visitStart && i==startInd) break;
		if (i==startInd) visitStart=1;
		if (itemcol->list[i]->type==type) {
			return (itemcol->list[i]);
		}
	}
	return NULL;	//at this pt startInd was visited again and therefore no item of type exists in list at all
}



