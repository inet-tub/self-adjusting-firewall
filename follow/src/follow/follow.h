/*
 * follow.h
 *
 *  Created on: Jul 13, 2021
 *      Author: vamsi
 */

#ifndef FOLLOW_FOLLOW_H_
#define FOLLOW_FOLLOW_H_

#include "./../../tuplemerge/ElementaryClasses.h"
#include "./../classifier/baseclassifier.h"
#include "./../cmd/CommandLine.h"

#include <unordered_map>

# define POINTER_SIZE 4
# define INT_SIZE 4
# define HEADER_SIZE 4

# define MAX_DEPENDENCIES 100

using namespace std;
using namespace std::chrono;

namespace simulator{

struct listNode{
	Rule* rule;
	listNode* nextNode;
	listNode* prevNode;
	uint32_t index; // This is the index of this node in the doubly linked list starting from head at 0.
	list<listNode*> dependencies;
};

//class dagNode{
//public:
//	dagNode(listNode* item){
//		listitem=item;
//		OutNeighbours.push_back(item);
//	}
//	virtual ~dagNode();
//
//	void AddDependency(listNode* dep){
//		OutNeighbours.push_back(dep);
//	}
//
//private:
//	listNode* listitem;
//	std::vector<listNode*> OutNeighbours;
//
//};


class follow : public BaseClassifier {
public:
	follow();
	virtual ~follow();

    int CreateClassifier(CommandLine* cmd);

    /* For inserting a rule. The function checks if the rule already exists, if not the rule is inserted in the datastructure. Num rules and bytes are updated  */
    unsigned int InsertRule(const Rule& r);

    /* The function searches for the rule first and then deletes it.  Num rules and bytes are updated */
    unsigned int DeleteRule(const Rule& r);

    /* Access is basically classifying a packet. */
    unsigned int AccessRule(const Packet& p);

    /* self-explanatory functions. Nothing fancy here. */
    unsigned int GetNumRules() const;

    uint64_t GetMemorysize() const;

    /* Returns the time taken for creating the datastrucutre (tree/table/list) for the given ruleset. */
    double GetInitDelay() const;

    double GetAvgNodesTraversed(){
    	return double(totalNodesTraversed)/totalAccess;
    }
    void setReconfigure(bool set){
    	reconfigure=set;
    }

    void setFastReconfigure(bool set){
    	fastReconfiguration=set;
    }

    listNode* findDependency(listNode*& node, uint32_t * nodesTraversed);
    bool depends(const Rule& dependency, const Rule& rule);

    void mtf(listNode*& first, listNode*& second);



    
    std::vector<Rule> rules;

private:
    struct listNode* head;
    struct listNode* tail;
    uint64_t totalNodesTraversed; // May overflow
    uint64_t totalAccess;
    bool reconfigure;
    bool fastReconfiguration;

    uint64_t numRules;
    uint64_t totalMemoryBytes;
};

} /* namespace simulator */

#endif /* FOLLOW_FOLLOW_H_ */
