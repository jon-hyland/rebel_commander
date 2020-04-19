
#ifndef ASTAR_H
#define ASTAR_H

#include "LinkedList.h"
#include "SortedLinkedList.h"
#include "SortedObjectLinkedList.h"

const int MAX_SUCCESSORS = 4;

class PathNode : public IComparable {
public:
	int f,g,h;
	PathNode* parent;
	PathNode* succ[MAX_SUCCESSORS];		//space for the generated successors of path node
	PathNode* neigh[MAX_SUCCESSORS];	//neighbors of this path node
	int x,y,id;	//location for relative distance, and id
	//methods
	PathNode() {
		parent=NULL;
		for(int i=0; i<MAX_SUCCESSORS; i++) {
			succ[i]=NULL;
			neigh[i]=NULL;
		}
	}
	PathNode(int newX,int newY,int newID) {
		x=newX; y=newY; id=newID;
		parent=NULL;
		for(int i=0; i<MAX_SUCCESSORS; i++) {
			succ[i]=NULL;
			neigh[i]=NULL;
		}
	}
	__forceinline int getPriority(){return(f);}
};

//some default cost estimation/calc functions
int EstimateCostDefault(PathNode* start, PathNode* dest);
int SuccessorCostDefault(PathNode* start, PathNode* dest);	//this assumes start --> dest is always = 1
//
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
__forceinline int FastDistance2D(int x, int y)
{
	// this function computes the distance from 0,0 to x,y with 3.5% error
	// first compute the absolute value of x,y
	x = abs(x);
	y = abs(y);
	// compute the minimum of x,y
	int mn = MIN(x,y);
	// return the distance
	return(x+y-(mn>>1)-(mn>>2)+(mn>>4));
}
int EstimateCostDefault(PathNode* start, PathNode* dest)
{
	return FastDistance2D(dest->x-start->x,dest->y-start->y);
}
int SuccessorCostDefault(PathNode* start, PathNode* dest)
{
	return 1;
}

class AStarBestPath {
private:
	//private data members
	SortedObjectLinkedList* open;
	SortedObjectLinkedList* closed;
	LinkedList<PathNode*>* bestPath;
	PathNode** graph;
	int graphSize;
	PathNode* _dest;	//ptr to destination path node
	//private methods
	void killGraph();
	void downPropagate(PathNode* parent);
	void buildPath(PathNode* destination);
	PathNode* getBestNode();
	int (*estimateCost)(PathNode*,PathNode*);
	int (*successorCost)(PathNode*,PathNode*);
public:
	AStarBestPath(PathNode** graphData, const int size);
	AStarBestPath(int* mapData, const int cols, const int rows);
	~AStarBestPath();
	LinkedList<PathNode*>* getBestPath();
	__forceinline void findBestPath(int start, int dest);
	void findBestPath(PathNode* start, PathNode* end);
	void setCostEstimationFunc(int (*estimateCostFunc)(PathNode*,PathNode*));
	void setCostCalculationFunc(int (*successorCostFunc)(PathNode*,PathNode*));
};

//-------------- CONSTRUCTORS --------------//
AStarBestPath::AStarBestPath(int* mapData, const int cols, const int rows)
{
	open = closed = NULL;
	bestPath = NULL;
	_dest = NULL;
	//
	int i,j,ind;
	int colsp=cols+2, rowsp=rows+2;
	int* mapDataPad=(int*)malloc(sizeof(int)*(rows+2)*(cols+2));
	graph=(PathNode**)malloc(sizeof(PathNode*)*rows*cols);
	graphSize=rows*cols;
	for (i=0; i<cols+2; i++)
		for (j=0; j<rows+2; j++) {
			if (i==0 || i==cols+1 ||
				j==0 || j==rows+1)
				 mapDataPad[j*colsp+i]=1;	//edges are blocked
			else
				 mapDataPad[j*colsp+i]=mapData[(j-1)*cols+(i-1)];
		}
	for (i=0; i<cols; i++)
		for (j=0; j<rows; j++)
			if (mapData[j*cols+i]==0)
				graph[j*cols+i]=new PathNode(i,j,i+j*cols);
			else
				graph[j*cols+i]=NULL;
	for (i=0; i<cols; i++)
		for (j=0; j<rows; j++) {
			if (mapData[j*cols+i]!=0) continue;	//no node present here
			ind=0;
			if (!mapDataPad[(j+1)*colsp+i])	//left
				graph[j*cols+i]->neigh[ind++]=graph[j*cols+(i-1)];
			if (ind==MAX_SUCCESSORS) continue;
			if (!mapDataPad[(j+1)*colsp+(i+2)])	//right
				graph[j*cols+i]->neigh[ind++]=graph[j*cols+(i+1)];
			if (ind==MAX_SUCCESSORS) continue;
			if (!mapDataPad[j*colsp+(i+1)])	//top
				graph[j*cols+i]->neigh[ind++]=graph[(j-1)*cols+i];
			if (ind==MAX_SUCCESSORS) continue;
			if (!mapDataPad[(j+2)*colsp+(i+1)])	//bot
				graph[j*cols+i]->neigh[ind++]=graph[(j+1)*cols+i];
			if (ind==MAX_SUCCESSORS) continue;
			/*if (!mapDataPad[j*colsp+i])	//top-left
				graph[j*cols+i]->neigh[ind++]=graph[(j-1)*cols+(i-1)];
			if (ind==MAX_SUCCESSORS) continue;
			if (!mapDataPad[j*colsp+(i+2)])	//top-right
				graph[j*cols+i]->neigh[ind++]=graph[(j-1)*cols+(i+1)];
			if (ind==MAX_SUCCESSORS) continue;
			if (!mapDataPad[(j+2)*colsp+i])	//bot-left
				graph[j*cols+i]->neigh[ind++]=graph[(j+1)*cols+(i-1)];
			if (ind==MAX_SUCCESSORS) continue;
			if (!mapDataPad[(j+2)*colsp+(i+2)])	//bot-right
				graph[j*cols+i]->neigh[ind++]=graph[(j+1)*cols+(i+1)];
			if (ind==MAX_SUCCESSORS) continue;*/
		}
	//clean up
	free(mapDataPad);
	//set default cost calculation func ptrs
	estimateCost=EstimateCostDefault;
	successorCost=SuccessorCostDefault;
}
AStarBestPath::AStarBestPath(PathNode** graphData, const int size)
{
	open = closed = NULL;
	bestPath = NULL;
	_dest = NULL;
	graph=graphData;
	graphSize=size;
	//set default cost calculation func ptrs
	estimateCost=EstimateCostDefault;
	successorCost=SuccessorCostDefault;
}
//--------------- DESTRUCTOR ---------------//
AStarBestPath::~AStarBestPath()
{
	if (open!=NULL) delete open;
	if (closed!=NULL) delete closed;
	if (bestPath!=NULL) delete bestPath;
	killGraph();
}
//------------------------------------------//

void AStarBestPath::killGraph() {
	for (int i=0; i<graphSize; i++)
		if (graph[i]!=NULL)
			delete graph[i];
	free(graph);
	graphSize=0;
}

LinkedList<PathNode*>* AStarBestPath::getBestPath()
{
	return bestPath;
}

void AStarBestPath::setCostEstimationFunc(int (*estimateCostFunc)(PathNode*,PathNode*))
{
	estimateCost=estimateCostFunc;
}
void AStarBestPath::setCostCalculationFunc(int (*successorCostFunc)(PathNode*,PathNode*))
{
	successorCost=successorCostFunc;
}

__forceinline void AStarBestPath::findBestPath(int start, int dest)
{
	findBestPath(graph[start],graph[dest]);
}
void AStarBestPath::findBestPath(PathNode* start, PathNode* dest)
{
	if (bestPath!=NULL) delete bestPath;
	bestPath = NULL;
	if (start==NULL || dest==NULL) return;
	if (open!=NULL) delete open;
	open = new SortedObjectLinkedList();
	if (closed!=NULL) delete closed;
	closed = new SortedObjectLinkedList();
	_dest=dest;		//setup global ptr to destination path node

	int index, newCost;

	//start by adding start node to OPEN set
	start->g = 0;
	start->h = estimateCost(start,_dest);	//est cost to dest
	start->f = start->g + start->h;
	open->insert(start);

	PathNode *bestNode, *succ;
	do {
		//get best node (removes from OPEN set and placed in CLOSED)
		bestNode=getBestNode();
		//check if bestNode is destination
		if (bestNode==_dest) {
			buildPath(bestNode);
			return;	//now path may be retrieved by calling getBestPath() method
		}
		for (int i=0; i<MAX_SUCCESSORS && (succ=bestNode->neigh[i])!=NULL; i++) {
			//get index of next free successor slot in bestNode
			// since 'succ' is always eventually added to 'bestNode', 'i' is index into succ[] array
			index=i;
			newCost=bestNode->g+successorCost(bestNode,succ);
			if ( (open->find(succ))>0 ) {
				bestNode->succ[index]=succ;
				if (succ->g>newCost) { //a better path is found to succ
					succ->g=newCost;
					succ->f=succ->g+succ->h;
					succ->parent=bestNode;
				}
			}
			else if ( (closed->find(succ))>0 ) {
				bestNode->succ[index]=succ;
				if (succ->g>newCost) { //a better path is found to succ
					succ->g=newCost;
					succ->f=succ->g+succ->h;
					succ->parent=bestNode;
					downPropagate(succ);
				}
			}
			else {	//not in OPEN nor CLOSED set
				succ->g = newCost;
				succ->h = estimateCost(succ,_dest);
				succ->f = succ->g+succ->h;
				succ->parent = bestNode;
				open->insert(succ);
				bestNode->succ[index]=succ;
			}
		}
	} while (open->length()>0);
}

void AStarBestPath::downPropagate(PathNode* parent)
{
	//stack for Depth-First traversal of parents' successors
	LinkedList<PathNode*>* stack = new LinkedList<PathNode*>();
	PathNode* succ;
	int index, newCost;
	
	//insert parents' immediate successors into stack only if they have g propagated to them
	index=0;
	while (index<MAX_SUCCESSORS && parent->succ[index]!=NULL) {
		succ=parent->succ[index];
		newCost=parent->g+successorCost(parent,succ);
		if (succ->parent==parent || newCost<succ->g) {
			succ->g=newCost;
			succ->f=succ->g+succ->h;
			succ->parent=parent;
			stack->push(succ);
		}
		index++;
	}
	//
	while (stack->length()>0) {
		parent=(PathNode*)stack->pop();
		//
		index=0;
		while (index<MAX_SUCCESSORS && parent->succ[index]!=NULL) {
			succ=parent->succ[index];
			newCost=parent->g+successorCost(parent,succ);
			if (succ->parent==parent || newCost<succ->g) {
				succ->g=newCost;
				succ->f=succ->g+succ->h;
				succ->parent=parent;
				stack->push(succ);
			}
			index++;
		}
	}
}

PathNode* AStarBestPath::getBestNode()
{
	if (open->length()==0)
		return NULL;
	//remove from OPEN and place in CLOSED
	PathNode* bestNode=(PathNode*)open->pop();
	closed->insert(bestNode);
	return bestNode;
}

void AStarBestPath::buildPath(PathNode* destination)
{
	if (destination==NULL) return;
	if (bestPath!=NULL) delete bestPath;
	bestPath = new LinkedList<PathNode*>();
	do {
		bestPath->push(destination);
		destination=destination->parent;
	} while (destination!=NULL);
}


#endif // ASTAR_H

