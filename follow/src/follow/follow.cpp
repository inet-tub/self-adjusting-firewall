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

}

follow::~follow() {
	struct listNode* node = head;
	struct listNode* item;
	while (node!=NULL){
		item=node;
		node = node->nextNode;
		delete item;
		item=NULL;
	}
}

int
follow::CreateClassifier(const vector<Rule>& ruleset){
    
    time_point<steady_clock> start, end;
    
    duration<double,std::milli> elapsed_milliseconds;
    
    start = steady_clock::now();

    for (const Rule& r : ruleset) {
        // We assume that the ruleset provided obeys dependencies in the correct order. This holds for classbench rulesets.
        
//        struct listNode* node = (struct listNode*) malloc(sizeof(struct listNode));
    	struct listNode* node = new listNode;
        node->rule = &r;
        node->prevNode = NULL;
        node->nextNode = NULL;
        node->numDependencies=0;

        if(tail != NULL){
            tail->nextNode = node;
            node->prevNode = tail;
        }
        if (head == NULL){
            head = node;
        }
        tail = node;
        rules.push_back(r);
        // this->InsertRule(r); // Or we can do this instead if the initial ruleset provided is messedup in the order of rules
    }

    struct listNode* h = head;
    while (h!=NULL){
    	struct listNode* h1 = head;
    	while (h1!=NULL){

    		if (h1->rule->IntersectsRule(*h->rule) && h1->rule->priority > h->rule->priority){
    			h->numDependencies+=1;
//    			h->dependencies.push_back(h1); // TODO what to do with the list of dependencies? Hard to keep track of positions in the list without which we resort to check and swap with each item.
    		}
    		h1=h1->nextNode;
    	}
//    	if (h->numDependencies)
//    		std::cout << "dependencies=" << h->dependencies.size()<< std::endl;
    	h=h->nextNode;
    }

    end = steady_clock::now();
    
    elapsed_milliseconds = end - start;
    
    this->initDelay = elapsed_milliseconds.count();
    
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
	return nodesTraversed;
}

unsigned int 
follow::AccessRule(const Packet& p){

	ASSERT(head!=NULL,"Empty list, Unexcepted when a packet is to be classified");
	struct listNode* node = head;
	bool match=false;
	uint32_t nodesTraversed=0;
	
	// Access
	/* start from head and search linearly through the list until the packet matches with a rule */
	while(!match){
        match=node->rule->MatchesPacket(p);
        nodesTraversed+=1;
        if (!match)
        	node=node->nextNode;
	}
	
	// Reconfiguration
	/* Nothing to do if the node is head. Otherwise, "follow" algorithm */
	if( node!=head ){
    	
    	struct listNode* temp = node;
    	bool dependencyFound=false;

    	/* starting from node which has been accessed before, search backwards towards the head to find a dependency and recurse */

    	while (temp->prevNode!=NULL){

	    	while (temp->prevNode != NULL && !dependencyFound){
	    		
	    		// TODO A better way to find dependencies. Checking whether the rule intersects another rule everytime is costly AF !
	    		dependencyFound = temp->prevNode->rule->IntersectsRule(*temp->rule);
	    		temp=temp->prevNode;

	    	} // at this point temp is the dependency or temp is at the head

	    	/* if a dependency is found then move the accessed node just behind the dependency */
	    	if (dependencyFound){
//	    		std::cout << "depFound" << std::endl;
	    		node->prevNode->nextNode = node->nextNode;
	    		node->nextNode->prevNode = node->prevNode;
	    		node->prevNode = temp;
	    		node->nextNode = temp->nextNode;
	    		temp->nextNode->prevNode = node;
	    		temp->nextNode = node;
	    		node=temp; /* This triggers the recursive more following the dependencies, until "temp" becomes the head. */
	    	}
	    	else { /* if no dependency is found, then we can move the node to the head */
	    		ASSERT(temp==head, "something went wrong in the follow algorithm!");
	    		node->prevNode->nextNode = node->nextNode;
	    		node->nextNode->prevNode = node->prevNode;
	    		node->prevNode = NULL;
	    		node->nextNode = head;
	    		head->prevNode = node;
	    		head = node;
	    		temp=head; /* resetting temp to head since head changed */
	    	}
	    	dependencyFound=false;
    	}
	}
	totalNodesTraversed+=nodesTraversed;
	totalAccess+=1;

//	std::cout << "nodes Traversed " << nodesTraversed << std::endl;
	return nodesTraversed;

}

unsigned int 
follow:: GetNumRules() const{
    return rules.size();
}

unsigned int 
follow::GetMemorysize() const{
	// TODO
	return 0;
}

double 
follow::GetInitDelay() const{
	return this->initDelay;
}


} /* namespace simulator */
