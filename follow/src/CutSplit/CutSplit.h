/*
 * CutSplit.h
 *
 *  Created on: Aug 1, 2021
 *      Author: vamsi
 */

#ifndef CUTSPLIT_CUTSPLIT_H_
#define CUTSPLIT_CUTSPLIT_H_


#include "./../includes/ElementaryClasses.h"
#include "./../IO/InputReader.h"
#include "./../classifier/baseclassifier.h"
#include "./../cmd/CommandLine.h"
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <list>
#include "trie.h"
#include <sys/time.h>

namespace simulator {

class CutSplit: public BaseClassifier {

public:
	CutSplit();
	virtual ~CutSplit();

//	int loadrule1(FILE *fp,pc_rule *rule);

	void count_length1(int number_rule,vector<Rule*>& rule,field_length *field_length_ruleset);

	void partition_V3(vector<Rule*>& rule, vector<vector<Rule*>>& subset,int number_rule,field_length *field_length_ruleset,int threshold_value[2]);

//	void CutSplitTrace(FILE* fpt,int trials);

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
		// std::cout << "nodes " << mem_acc[0] << " totalAccess " << totalAccess << std::endl;
		return mem_acc[0]/totalAccess;
	}

	std::vector<Rule> rules;

private:

	    uint64_t totalNodesTraversed; // May overflow
	    uint64_t totalAccess;

	    uint64_t numRules;
	    uint64_t totalMemoryBytes;

		int total_query;
		int bucketSize;
		int threshold;
//		pc_rule *rule;
		vector<Rule*> rule;
		int number_rule;
		vector<vector<Rule*>> subset_4;
		int num_subset_4[4];
		int threshold_value[2];

		trie* T_sa;
		trie* T_sa_da;
		trie* T_da;
		hs_node_t* big_node;
		hstrie* T;

		double* mem_acc;
};

} /* namespace simulator */

#endif /* CUTSPLIT_CUTSPLIT_H_ */
