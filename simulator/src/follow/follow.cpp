/*
 * follow.cpp
 *
 *  Created on: Jul 13, 2021
 *      Author: vamsi
 */

#include "./../includes/Assert.h"
#include <stdint.h>
#include "follow.h"


using namespace std;

namespace simulator{
follow::follow() {
	head=NULL;
	tail=NULL;
	totalNodesTraversed=0;
	totalAccess=0;
	reconfigure=true;
	fastReconfiguration=true;
	numRules=0;
	totalMemoryBytes=0;
}

follow::~follow() {
	struct listNode* node = head;
	struct listNode* item;
	while (node!=NULL){
		item=node;
		node = node->nextNode;
		item->dependencies.clear();
		item->nextNode=NULL;
		item->prevNode=NULL;
		item->rule=NULL;
		delete item;
		item=NULL;
	}
}

/* Check whether "dependency" is a dependency of r.  */
bool
follow::depends(const Rule& dependency, const Rule& r) {
	int dims=0;
	for (int i = 0; i < 5; i++) {
		if ( (r.range[i][HighDim] < dependency.range[i][LowDim]) || (r.range[i][LowDim] > dependency.range[i][HighDim]) ) {
			return false;
		}
	}
	return true;
	if (dependency.priority > r.priority)
		return true;
	else
		return false;
}

int
follow::CreateClassifier(CommandLine* cmd){

	string f = "/home/vamsi/src/phd/codebase/self-adjusting-firewall/classbench/db_generator/MyFilters10k";
	cmd->Get("ruleset", f);
	rules= InputReader::ReadFilterFile(f);
	numRules=rules.size();

	time_point<steady_clock> start, end;
	duration<double,std::milli> elapsed_milliseconds;
	start = steady_clock::now();

	uint32_t index = 0;
	for (const Rule& r : rules) {
		/*We assume that the ruleset provided obeys dependencies in the correct order. This holds for classbench rulesets.*/

		// struct listNode* node = (struct listNode*) malloc(sizeof(struct listNode));
		struct listNode* node = new listNode;
		node->rule = &r;
		node->prevNode = NULL;
		node->nextNode = NULL;
		node->dependencies.clear();
		node->index = index;
		index++;

		if(tail != NULL){
			tail->nextNode = node;
			node->prevNode = tail;
		}
		if (head == NULL){
			head = node;
		}
		tail = node;
	}
	if (fastReconfiguration){
		totalMemoryBytes+= rules.size()*(POINTER_SIZE + INT_SIZE);
	}
	else{
		totalMemoryBytes+= rules.size()*(POINTER_SIZE);
	}

	if (fastReconfiguration){
		// list of dependencies at each node.
		struct listNode* h = head;
		while (h!=NULL){
			struct listNode* h1 = h->prevNode;
			while (h1!=NULL){
				if (depends(*h1->rule,*h->rule)){
					h->dependencies.push_back(h1);
				}
				h1=h1->prevNode;
			}
			totalMemoryBytes+= POINTER_SIZE*h->dependencies.size();
			h=h->nextNode;
		}
	}

	end = steady_clock::now();
	elapsed_milliseconds = end - start;
	initDelay = elapsed_milliseconds.count();

	return 0;
}

unsigned int
follow::InsertRule(const Rule& r){
	// TODO
	return 0;
}

unsigned int
follow::DeleteRule(const Rule& r){

	ASSERT(head!=NULL,"Empty list, Unexcepted when a packet is to be classified");
	struct listNode* node = head;
	uint32_t match=0;
	uint32_t nodesTraversed=0;
	// Find the rule and then delete
	while(!match || node!=NULL){
		if (node->rule->priority == r.priority){ // priority numbers are unique, its enough to check this
			match=1;
			nodesTraversed+=1;
			node->prevNode->nextNode= node->nextNode;
			node->nextNode->prevNode = node->prevNode;
		}
		else{
			node=node->nextNode;
			nodesTraversed+=1;
		}
	}
	if (!match){
		//TODO something is wrong. Could be that a rule is asked to be deleted when it does not exist.
	}
	else{
		numRules--;
	}
	return nodesTraversed;
}

void
follow::mtf(listNode*& first, listNode*& second){

	/* When the second node moves forwards, second's previous and second's next nodes must be connected together */
	/* Second node moves exactly behind first */

	ASSERT(second!=NULL,"trying to move a NULL pointer in the list!");

	if (first==second){
		return;
	}
	else if (second->prevNode==first){
		return;
	}
	else if (second==head){
		head=second;
		return;
	}
	else if (second==tail){
		listNode* secPrev = second->prevNode;
		listNode* secNext = NULL;

		listNode* firstNext;

		if (first!=NULL){
			firstNext = first->nextNode;
			second->prevNode = first;
			second->nextNode = firstNext;
			first->nextNode = second;
			firstNext->prevNode = second;
		}
		else{
			firstNext = head;
			second->prevNode = NULL;
			second->nextNode = firstNext;
			head->prevNode = second;
			head = second;
		}

		secPrev->nextNode = NULL; // because second is tail and now secPrev becomes tail.
		tail = secPrev;
	}
	else if (first==head){
		listNode* secPrev = second->prevNode;
		listNode* secNext = second->nextNode;

		listNode* firstNext = first->nextNode;

		second->prevNode = first;
		second->nextNode = firstNext;
		first->nextNode = second;
		firstNext->prevNode = second;

		secPrev->nextNode = secNext;
		secNext->prevNode = secPrev;

	}
	else{
		listNode* secPrev = second->prevNode;
		listNode* secNext = second->nextNode;

		listNode* firstNext = NULL;

		if (first!=NULL){
			firstNext = first->nextNode;
			second->prevNode = first;
			second->nextNode = firstNext;
			first->nextNode = second;
			firstNext->prevNode = second;
		}
		else{
			firstNext = head;
			second->prevNode = NULL;
			second->nextNode = firstNext;
			head->prevNode = second;
			head = second;
		}

		secPrev->nextNode = secNext;
		secNext->prevNode = secPrev;
	}
}



listNode*
follow::findDependency(listNode *& node, uint32_t * nodesTraversed){

	listNode* temp = NULL;

	if (!fastReconfiguration){
		temp = node->prevNode;
		bool dependencyFound = false;
		int count = 0;
		while (temp!= NULL && !dependencyFound){
			dependencyFound = depends(*temp->rule,*node->rule);
			*nodesTraversed = *nodesTraversed+1; // because we performed a match action above
			if (!dependencyFound){
				temp=temp->prevNode;
			}
			count++;
		} // at this point temp is the dependency or temp is NULL
	}
	else{
		uint32_t max = 0;
		temp = NULL;
		for (auto dep : node->dependencies){
			if (dep->index >= max){
				temp=dep;
				max=dep->index;
			}
		}
	}
	return temp;
}


unsigned int 
follow::AccessRule(const Packet& p){

	ASSERT(head!=NULL,"Empty list, Unexcepted when a packet is to be classified");
	listNode* node = head;
	bool match=false;
	uint32_t nodesTraversed=0;

	// Access
	/* start from head and search linearly through the list until the packet matches with a rule */

	uint32_t index = 0;
	while(!match && node!=NULL){
		match=node->rule->MatchesPacket(p);
		nodesTraversed++;
		node->index=index;
		index++;
		if (!match){
			node=node->nextNode;
		}
	}

	if(!match){
		std::cout << "rule not found "  << std::endl;
	}

	// Reconfiguration
	/* Nothing to do if the node is head. Otherwise, "follow" algorithm */
	struct listNode* dnode= node;

	if( dnode!=NULL && dnode!=head && reconfigure==true){

		struct listNode* temp = dnode->prevNode;

		/* starting from node which has been accessed before, search backwards towards the head to find a dependency and recurse */

		int count=0;
		while (temp!=NULL){

			temp = findDependency(dnode, &nodesTraversed);

			/* if a dependency is found then move the accessed node just behind the dependency */
			/* node moves behind temp */
			mtf(temp,dnode);

			dnode=temp;

		}

	}
	totalNodesTraversed+=nodesTraversed;
	totalAccess+=1;

	return nodesTraversed;

}

unsigned int 
follow:: GetNumRules() const{
	return numRules;
}

uint64_t
follow::GetMemorysize() const{
	return totalMemoryBytes;
}

double 
follow::GetInitDelay() const{
	return initDelay;
}


} /* namespace simulator */
