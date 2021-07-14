/*
 * Tuplemerge.cpp
 *
 *  Created on: Jul 8, 2021
 *      Author: vamsi
 */

#include "Tuplemerge.h"
#include <vector>
#include <unordered_map>
#include <sstream>

namespace simulator{
Tuplemerge::Tuplemerge(int limit):
	limit(limit)
{
	std::unordered_map<string,string> collisionLimit;
	stringstream ss;
	ss << limit;
	collisionLimit.insert(std::pair<string, string>("TM.Limit.Collide", ss.str()));
	classifier = new TupleMergeOnline(collisionLimit);

}

Tuplemerge::~Tuplemerge() {
	delete classifier;
}

unsigned int Tuplemerge::GetNumRules() const {
	return classifier->rules.size();
}


unsigned int Tuplemerge::GetMemorysize() const {
	return classifier->MemSizeBytes();
}

// In milliseconds
double Tuplemerge::GetInitDelay() const {
	return this->initDelay;
}


int Tuplemerge::CreateClassifier(const vector<Rule>& rules){
	time_point<steady_clock> start, end;
		duration<double,std::milli> elapsed_milliseconds;
		start = steady_clock::now();
		classifier->ConstructClassifier(rules);
		end = steady_clock::now();
		elapsed_milliseconds = end - start;
		this->initDelay = elapsed_milliseconds.count();
	return 0;
}

unsigned int Tuplemerge::InsertRule(const Rule& r){

	classifier->InsertRule(r);
	// TODO: They don't check if the rule exists or not. Just keep inserting.
	return 0;
}

unsigned int Tuplemerge::DeleteRule(const Rule& r){
	// TODO: In tuplemerge they are taking "index" which is easily obtained from ruleset list. The ruleset list is maintained separately and is not from the datastructure (tables). Shouldn't we first search for the rule?
	return 0;
}

unsigned int Tuplemerge::AccessRule(const Packet& p){
	classifier->ClassifyAPacket(p);
	return 0;
}

}
