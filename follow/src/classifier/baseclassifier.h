/*
 * baseclassifier.h
 *
 *  Created on: Jul 8, 2021
 *      Author: vamsi
 */

#ifndef CLASSIFIER_BASECLASSIFIER_H_
#define CLASSIFIER_BASECLASSIFIER_H_

#include "./../includes/external_includes.h"
#include <vector>
#include <list>
#include <chrono>
#include "./../cmd/CommandLine.h"

using namespace std;
using namespace std::chrono;
namespace simulator{

class BaseClassifier {
public:

	/* Constructor */
	BaseClassifier(){};

	/* Destructor */
	virtual ~BaseClassifier(){};

	/* Creates the datastructure maintained by the classifier. The function should also update the number of rules and the bytes consumed. */
	virtual int CreateClassifier(CommandLine* cmd) {return 0;}

	/* For inserting a rule. The function checks if the rule already exists, if not the rule is inserted in the datastructure. Num rules and bytes are updated  */
	virtual unsigned int InsertRule(const Rule& r) {return 0;};

	/* The function searches for the rule first and then deletes it.  Num rules and bytes are updated */
	virtual unsigned int DeleteRule(const Rule& r) {return 0;}

	/* Access is basically classifying a packet. */
	virtual unsigned int AccessRule(const Packet& p) {return 0;}

	/* self-explanatory functions. Nothing fancy here. */
	virtual unsigned int GetNumRules() {return 0;}

	virtual unsigned int GetMemorysize() {return 0;}

	/* Returns the time taken for creating the datastrucutre (tree/table/list) for the given ruleset. */
	virtual double GetInitDelay() const=0;

	double initDelay=0;

	unsigned int TotalRules=0;

	unsigned int TotalBytes=0;
};
} /* namespace simulator */ 

#endif /* CLASSIFIER_BASECLASSIFIER_H_ */
