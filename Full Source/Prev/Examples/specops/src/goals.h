
//adds an internal condition to a goal
void AddCondition(LPGOAL goal, LPCONDITION condition)
{
	if (goal->internConditions==NULL)
		goal->internConditions=new LinkedList<LPCONDITION>();
	goal->internConditions->insert(condition);
}

// Checks if player satisfies the condition
// preconditions: condition must not be satisfied YET
// postconditions: returns true if player satisfied condition
int CheckCondition(LPCONDITION condition, LPPLAYER player)
{
	int dx,dy,len,i,allSatisfied;
	LPCONDITION cond;

	switch (condition->type) {
	case CONTYPE_TOUCH:
		dx = (player->x-condition->data[3]);
		dy = (player->y-condition->data[4]);
		if (player->mappos.x==condition->data[0] &&
			player->mappos.y==condition->data[1] &&
			player->mappos.z==condition->data[2] &&
			dx*dx+dy*dy<condition->data[5]) {
				condition->satisfied=1;
		}
		break;
	case CONTYPE_TIMER:
		if (condition->data[0]>0)
			condition->data[0]-=1;
		else
			condition->satisfied=1;
		break;
	case CONTYPE_LIST_CONTAINS_ITEM:
		if (ListContainsItem(condition->itemcol,condition->item))
			condition->satisfied=1;
		break;
	case CONTYPE_TARGET_DESTROYED:
		if ((condition->item->type & ID_TARGET) &&
			condition->item->attr[1]<=0)
			condition->satisfied=1;
		break;
	case CONTYPE_CONDITIONS_SATISFIED:
		len=condition->conditionList->length();
		allSatisfied=1;
		for (i=1; i<=len; i++) {
			cond=condition->conditionList->get(i);
			allSatisfied = allSatisfied && cond->satisfied;
		}
		condition->satisfied=allSatisfied;
		break;
	case CONTYPE_NPC_KILLED:
		for (i=0; i<MAX_NUM_NPC; i++) {
			if (npcs[i]==condition->npc) break;
		}
		if (i==MAX_NUM_NPC)
			condition->satisfied = 1;
		break;
	default: break;
	}
	return (condition->satisfied);
}

// executes a result (that is a consequence of a satisfied condition)
void ExecuteResults(LPRESULT result, LPPLAYER player)
{
	if (result==NULL) return;
	switch (result->type) {
	case RESTYPE_ENERGY:
		player->energy+=result->data[0];
		break;
	case RESTYPE_DESTROY_OBJECT:
		if (ListContainsItem((LPOBJECTCOL)result->itemcol,(LPOBJECT)result->item))
			ListDelete((LPOBJECTCOL)result->itemcol,(LPOBJECT)result->item);
		break;
	case RESTYPE_MAKE_DISABLED_INVISIBLE:
		result->item->enabled=result->item->visible=0;
		break;
	case RESTYPE_MAKE_ENABLED_VISIBLE:
		result->item->enabled=result->item->visible=1;
		break;
	case RESTYPE_SHOW_MESSAGE:
		currentMsg=CreateMessage(result->messages,
								 result->data[0],
								 result->data[1],result->data[2],
								 result->data[3],result->data[4]);
		ShowMessages();
		break;
	case RESTYPE_EXPLOSION:
		AddItem(result->area,ID_DAMAGE_EXPL,0,result->data[0],result->data[1],NULL,0);
		break;
	case RESTYPE_ENDGAME:
		gameState=GS_EXIT;
		break;
	}
	if (result->next!=NULL)
		ExecuteResults(result->next, player);
}

// checks all "reachable" conditions in condition tree that player may satisfy
void CheckConditions(LPCONDITION start, LPPLAYER player)
{
	int condInd=0,numConditions;
	LPCONDITION cond;

	if (start==NULL)
		return;
	if (!start->satisfied) {	//end of this branch of conditions
		if (CheckCondition(start,player))
			ExecuteResults(start->result,player);
		return;
	}
	numConditions=start->numNextConditions;
	while (condInd<numConditions) {		//check other branches of conditions
		cond=start->nextConditions[condInd];
		CheckConditions(cond,player);
		condInd++;
	};
}

void CheckGoalConditions(LPGOAL goal, LPPLAYER player)
{
	CheckConditions(goal->conditionTree,player);
}

//checks all internal conditions (used by game engine)
void CheckInternalConditions(LPGOAL goal, LPPLAYER player)
{
	if (goal->internConditions==NULL) return;
	//
	LPCONDITION cond;
	LPRESULT nextres,res;
	int len=goal->internConditions->length();
	for (int i=len; i>0; i--) {
		cond=goal->internConditions->get(i);
		if (CheckCondition(cond,player)) {
			//remove from list
			goal->internConditions->remove(i);
			//execute results
			ExecuteResults(cond->result,player);
			//free stuff
			// free results list
			res=cond->result;
			if (res!=NULL) {
				do {
					nextres=res->next;
					FREE(res->data);
					FREE(res->messages);
					free(res);
					res=nextres;
				} while (res!=NULL);
			}
			// free condition stuff
			FREE(cond->data);
			DEL(cond->conditionList);
			free(cond);
		}
	}
}



