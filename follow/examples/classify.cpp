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

#include "./../src/includes/external_includes.h"
#include "./../src/includes/Assert.h"
#include "./../src/cmd/CommandLine.h"

#include "./../src/follow/follow.h"
#include "./../src/cuts/cuts.h"

using namespace std;
using namespace simulator;

int main(int argc, char* argv[]) {


	string alg = "follow";
	CommandLine cmd;

	cmd.ParseArgs(argc, argv);
	cmd.Get("alg",alg);

	string rulesetFile = "/home/vamsi/src/phd/self-adjusting-firewall/classbench/db_generator/MyFilters10k";
	string traceFile = "/home/vamsi/src/phd/self-adjusting-firewall/classbench/db_generator/MyFilters10k_trace";
	//	string traceFile = "/home/vamsi/src/phd/self-adjusting-firewall/classbench/db_generator/mytrace";


	cmd.Get("ruleset",rulesetFile);
	cmd.Get("trace",traceFile);

	if (alg=="follow"){

		vector<Rule> rules = InputReader::ReadFilterFile(rulesetFile);
		vector <Packet> trace = InputReader::ReadPackets(traceFile);

		follow classifier;
		classifier.CreateClassifier(rules);
		std::cout << "InitDelayms " << classifier.GetInitDelay() << std::endl;

		int count = 0;
		for (auto p:trace){

			classifier.AccessRule(p);
//			if (count > 10){
//				break;
//			}
//			count++;
		}
		std::cout << "Average nodes traversed " << classifier.GetAvgNodesTraversed() << std::endl;
	}
	else{
		vector <Packet> trace = InputReader::ReadPackets(traceFile);
		cuts CutsClassifier;
		CutsClassifier.parseargs(&cmd);
		CutsClassifier.BuildClassifier();

//		CutsClassifier.CreateClassifier(rules);
//		std::cout << "InitDelayms " << CutsClassifier.GetInitDelay() << std::endl;

		for (auto p:trace){
			std::cout << "Nodes Accessed = " << CutsClassifier.AccessRule(p) << std::endl;
		}
//
//		std::cout << "Average nodes traversed " << CutsClassifier.GetAvgNodesTraversed() << std::endl;
	}

	return 0;
}


