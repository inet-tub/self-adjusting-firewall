/*
 * follow.cpp
 *
 *  Created on: Jul 8, 2021
 *      Author: vamsi
 */



#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <chrono>
#include <string>
#include <sstream>
#include <fstream>
#include<bits/stdc++.h>

#include "./../src/includes/ElementaryClasses.h"
#include "./../src/IO/InputReader.h"

#include "./../src/includes/Assert.h"
#include "./../src/cmd/CommandLine.h"

#include "./../src/follow/follow.h"
#include "./../src/cuts/cuts.h"

#include "./../src/CutSplit/CutSplit.h"

using namespace std;
using namespace simulator;

uint32_t dumb_Hashing(Packet& pkt)
{
	uint64_t hash = 0;
	for (uint32_t i = 0; i < 5; i++)
		hash += pkt[i];
	return hash;
}


int main(int argc, char* argv[]) {


	string alg = "ef";
	uint32_t threads = 1;
	CommandLine cmd;

	cmd.ParseArgs(argc, argv);
	cmd.Get("alg", alg);
	cmd.Get("threads", threads);
	string rulesetFile = "/home/vamsi/src/phd/codebase/self-adjusting-firewall/classbench/db_generator/MyFilters10k";
	string traceFile = "/home/vamsi/src/phd/codebase/self-adjusting-firewall/classbench/db_generator/MyFilters10k_trace";
//		string traceFile = "/home/vamsi/src/phd/self-adjusting-firewall/classbench/db_generator/mytrace";

	string output = "./output.dat";
	cmd.Get("outFile", output);
	ofstream outputFile ;
	outputFile.open(output, std::ios_base::app);

	string Ps = "Follow";

	double PsInit = 0;
	int PsNumClass = 0;
	double PsTotalClassTime = 0;
	double PsAvgClassTime = 0;
	int PsNumMisClass = 0;
	double PsThroughput = 0;



	int trials = 1;


	cmd.Get("ruleset", rulesetFile);
	cmd.Get("trace", traceFile);

//	vector<Rule> rules = InputReader::ReadFilterFile(rulesetFile);
	vector <Packet> trace = InputReader::ReadPackets(traceFile);
	int number_rule = 0 ;
	int number_pkt = trace.size();

	if (alg == "follow") {

		follow classifier[threads];
		for (uint32_t i = 0; i < threads; i++) {
			classifier[i].setReconfigure(true);
			classifier[i].setFastReconfigure(false);
			classifier[i].CreateClassifier(&cmd);
		}
		std::cout << "InitDelayms " << classifier[0].GetInitDelay() << std::endl;
		std::cout << "TotalMemoryBytes " <<classifier[0].GetMemorysize() << std::endl;

		for (auto p : trace) {

			uint32_t thread = dumb_Hashing(p)%threads;

			classifier[thread].AccessRule(p);
		}
		double maxAvgTraversed = 0;
		double avgReqPerCost = 0;
		for (uint32_t i=0; i<threads; i++){
			if (maxAvgTraversed < classifier[i].GetAvgNodesTraversed())
				maxAvgTraversed = classifier[i].GetAvgNodesTraversed();
			avgReqPerCost += 1.0/classifier[i].GetAvgNodesTraversed();
		}
		std::cout << "AverageTraversed " << classifier[0].GetAvgNodesTraversed() << std::endl;
		std::cout << "AverageCostPerReq " << 1.0/avgReqPerCost << " AverageReqPerCost " << avgReqPerCost << " MaxAverageCostPerReq " << maxAvgTraversed << " MaxAverageReqPerCost " << 1.0/maxAvgTraversed  << std::endl;
	}
	else if (alg == "followfast") {
		follow classifier[threads];
		for (uint32_t i=0; i<threads ; i++){
			classifier[i].setReconfigure(true);
			classifier[i].setFastReconfigure(true);
			classifier[i].CreateClassifier(&cmd);
		}
		std::cout << "InitDelayms " << classifier[0].GetInitDelay() << std::endl;
		std::cout << "TotalMemoryBytes " << classifier[0].GetMemorysize() << std::endl;
		for (auto p : trace) {
			uint32_t thread = dumb_Hashing(p)%threads;
			classifier[thread].AccessRule(p);
		}
		std::cout << "AverageTraversed " << classifier[0].GetAvgNodesTraversed() << std::endl;
		double maxAvgTraversed = 0;
		double avgReqPerCost = 0;
		for (uint32_t i=0; i<threads; i++){
			if (maxAvgTraversed < classifier[i].GetAvgNodesTraversed())
				maxAvgTraversed = classifier[i].GetAvgNodesTraversed();
			avgReqPerCost += 1.0/classifier[i].GetAvgNodesTraversed();
		}
		std::cout << "AverageCostPerReq " << 1.0/avgReqPerCost << " AverageReqPerCost " << avgReqPerCost << " MaxAverageCostPerReq " << maxAvgTraversed << " MaxAverageReqPerCost " << 1.0/maxAvgTraversed  << std::endl;
	}
	else if (alg == "cuts") {
		cuts CutsClassifier[threads];
		for (uint32_t i=0; i< threads ; i++){
			CutsClassifier[i].CreateClassifier(&cmd);
		}
		std::cout << "InitDelayms " << CutsClassifier[0].GetInitDelay() << std::endl;
		std::cout << "TotalMemoryBytes " << CutsClassifier[0].GetMemorysize() << std::endl;

		for (auto p : trace) {
			uint32_t thread = dumb_Hashing(p)%threads;
			CutsClassifier[thread].AccessRule(p);
		}
		std::cout << "AverageTraversed " << CutsClassifier[0].GetAvgNodesTraversed() << std::endl;
		double maxAvgTraversed = 0;
		double avgReqPerCost = 0;
		for (uint32_t i=0; i<threads; i++){
			if (maxAvgTraversed < CutsClassifier[i].GetAvgNodesTraversed())
				maxAvgTraversed = CutsClassifier[i].GetAvgNodesTraversed();
			avgReqPerCost += 1.0/CutsClassifier[i].GetAvgNodesTraversed();
		}
		std::cout << "AverageCostPerReq " << 1.0/avgReqPerCost << " AverageReqPerCost " << avgReqPerCost << " MaxAverageCostPerReq " << maxAvgTraversed << " MaxAverageReqPerCost " << 1.0/maxAvgTraversed  << std::endl;
	}
	else if (alg == "list") {
		follow classifier[threads];
		for (uint32_t i=0; i< threads; i++){
			classifier[i].setReconfigure(false);
			classifier[i].setFastReconfigure(false);
			classifier[i].CreateClassifier(&cmd);			
		}
		std::cout << "InitDelayms " << classifier[0].GetInitDelay() << std::endl;
		std::cout << "TotalMemoryBytes " << classifier[0].GetMemorysize() << std::endl;

		for (auto p : trace) {
			uint32_t thread = dumb_Hashing(p)%threads;
			classifier[thread].AccessRule(p);
		}
		std::cout << "AverageTraversed " << classifier[0].GetAvgNodesTraversed() << std::endl;
		double maxAvgTraversed = 0;
		double avgReqPerCost = 0;
		for (uint32_t i=0; i<threads; i++){
			if (maxAvgTraversed < classifier[i].GetAvgNodesTraversed())
				maxAvgTraversed = classifier[i].GetAvgNodesTraversed();
			avgReqPerCost += 1.0/classifier[i].GetAvgNodesTraversed();
		}
		std::cout << "AverageCostPerReq " << 1.0/avgReqPerCost << " AverageReqPerCost " << avgReqPerCost << " MaxAverageCostPerReq " << maxAvgTraversed << " MaxAverageReqPerCost " << 1.0/maxAvgTraversed  << std::endl;
	}
	else if (alg == "cutsplit") {
		CutSplit cutsplit[threads];
		for (uint32_t i=0; i<threads; i++){
			cutsplit[i].CreateClassifier(&cmd);		
		}
		std::cout << "InitDelayms " << cutsplit[0].GetInitDelay() << std::endl;
		std::cout << "TotalMemoryBytes " << cutsplit[0].GetMemorysize() << std::endl;

		uint32_t miss = 0;
		for (auto p : trace) {
			uint32_t thread = dumb_Hashing(p)%threads;
			miss += cutsplit[thread].AccessRule(p);
		}
		std::cout << "AverageTraversed " << cutsplit[0].GetAvgNodesTraversed() << std::endl;
		std::cout << "Missed " << miss << std::endl;
		double maxAvgTraversed = 0;
		double avgReqPerCost = 0;
		for (uint32_t i=0; i<threads; i++){
			if (maxAvgTraversed < cutsplit[i].GetAvgNodesTraversed())
				maxAvgTraversed = cutsplit[i].GetAvgNodesTraversed();
			avgReqPerCost += 1.0/cutsplit[i].GetAvgNodesTraversed();
		}
		std::cout << "AverageCostPerReq " << 1.0/avgReqPerCost << " AverageReqPerCost " << avgReqPerCost << " MaxAverageCostPerReq " << maxAvgTraversed << " MaxAverageReqPerCost " << 1.0/maxAvgTraversed  << std::endl;

	}

	return 0;
}


