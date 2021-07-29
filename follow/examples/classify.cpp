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


	string alg = "ef";
	CommandLine cmd;

	cmd.ParseArgs(argc, argv);
	cmd.Get("alg",alg);

	string rulesetFile = "/home/vamsi/src/phd/self-adjusting-firewall/classbench/db_generator/MyFilters10k";
	string traceFile = "/home/vamsi/src/phd/self-adjusting-firewall/classbench/db_generator/MyFilters10k_trace";
//		string traceFile = "/home/vamsi/src/phd/self-adjusting-firewall/classbench/db_generator/mytrace";



	string output = "./output.dat";
	cmd.Get("outFile",output);
	ofstream outputFile ;
	outputFile.open(output,std::ios_base::app);

    string Ps = "Follow";

    double PsInit = 0;
    int PsNumClass = 0;
    double PsTotalClassTime = 0;
    double PsAvgClassTime = 0;
    int PsNumMisClass = 0;
    double PsThroughput = 0;



int trials=1;


	cmd.Get("ruleset",rulesetFile);
	cmd.Get("trace",traceFile);

//	vector<Rule> rules = InputReader::ReadFilterFile(rulesetFile);
	vector <Packet> trace = InputReader::ReadPackets(traceFile);
	int number_rule= 0 ;
	int number_pkt=trace.size();

	if (alg=="follow"){
			follow classifier;
			classifier.setReconfigure(true);
			classifier.setFastReconfigure(false);
			classifier.CreateClassifier(&cmd);
			std::cout << "InitDelayms " << classifier.GetInitDelay() << std::endl;
			std::cout << "TotalMemoryBytes " <<classifier.GetMemorysize() << std::endl;

			for (auto p:trace){

				classifier.AccessRule(p);
			}
		std::cout << "AverageTraversed " <<classifier.GetAvgNodesTraversed()<< std::endl;
	}
	else if (alg=="followfast"){
				follow classifier;
				classifier.setReconfigure(true);
				classifier.setFastReconfigure(true);
				classifier.CreateClassifier(&cmd);
				 std::cout << "InitDelayms " << classifier.GetInitDelay() << std::endl;
				 std::cout << "TotalMemoryBytes " <<classifier.GetMemorysize() << std::endl;
				for (auto p:trace){

					classifier.AccessRule(p);
				}
			std::cout << "AverageTraversed " <<classifier.GetAvgNodesTraversed()<< std::endl;
		}
	else if (alg=="cuts"){
		cuts CutsClassifier;
		CutsClassifier.CreateClassifier(&cmd);

		std::cout << "InitDelayms " << CutsClassifier.GetInitDelay() << std::endl;
		std::cout << "TotalMemoryBytes " <<CutsClassifier.GetMemorysize() << std::endl;
		for (auto p:trace){
			CutsClassifier.AccessRule(p);
		}
		std::cout << "AverageTraversed " << CutsClassifier.GetAvgNodesTraversed() << std::endl;
	}
	else if (alg=="list"){
		follow classifier;
		classifier.CreateClassifier(&cmd);
		classifier.setReconfigure(false);

		std::cout << "InitDelayms " << classifier.GetInitDelay() << std::endl;
		std::cout << "TotalMemoryBytes " <<classifier.GetMemorysize() << std::endl;

		for (auto p:trace){

			classifier.AccessRule(p);
		}
	std::cout << "AverageTraversed " <<classifier.GetAvgNodesTraversed()<< std::endl;
	}

	return 0;
}


