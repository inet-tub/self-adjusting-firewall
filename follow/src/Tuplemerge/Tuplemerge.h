/*
 * Tuplemerge.h
 *
 *  Created on: Jul 8, 2021
 *      Author: vamsi
 */

#ifndef TUPLEMERGE_TUPLEMERGE_H_
#define TUPLEMERGE_TUPLEMERGE_H_

#include "./../../tuplemerge/TupleMerge/TupleMergeOnline.h"
#include "./../classifier/baseclassifier.h"
#include "./../includes/external_includes.h"
#include "./../cmd/CommandLine.h"

namespace simulator{

class Tuplemerge : public BaseClassifier {
public:
	Tuplemerge (int limit = 10);
	virtual ~Tuplemerge();
	virtual int CreateClassifier(CommandLine* cmd);

	/* For inserting a rule. The function checks if the rule already exists, if not the rule is inserted in the datastructure. Num rules and bytes are updated  */
	virtual unsigned int InsertRule(const Rule& r);

	/* The function searches for the rule first and then deletes it.  Num rules and bytes are updated */
	virtual unsigned int DeleteRule(const Rule& r);

	/* Access is basically classifying a packet. */
	virtual unsigned int AccessRule(const Packet& p);

	/* self-explanatory functions. Nothing fancy here. */
	virtual unsigned int GetNumRules() const;

	virtual unsigned int GetMemorysize() const;

	/* Returns the time taken for creating the datastrucutre (tree/table/list) for the given ruleset. */
	virtual double GetInitDelay() const;

	std::vector<Rule> rules;

private:
	TupleMergeOnline* classifier;
	int limit;

};

} /* namespace simulator */

#endif /* TUPLEMERGE_TUPLEMERGE_H_ */
