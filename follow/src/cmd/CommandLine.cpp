/*
 * CommandLine.cpp
 *
 *  Created on: Jul 14, 2021
 *      Author: vamsi
 */

#include "CommandLine.h"
#include <sstream>
#include "./../includes/Assert.h"

using namespace std;

namespace simulator{

CommandLine::CommandLine() {

}

CommandLine::~CommandLine() {
	ArgValueMap.clear();
}


void
CommandLine::ParseArgs(int argc, char* argv[]) {

	for (int i = 1; i < argc; i++) {

		string arg = argv[i];

		std::string throwError = "invalid argument found ";
		throwError.append(arg);
		ASSERT(arg.find("=")!=std::string::npos ,throwError);

		string doublehyphen = "%"; //temp change
		string hyphen = "%"; //temp change
		std::string par = arg.substr(0, 2);
		if (par==doublehyphen){
			arg = arg.substr(2);
		}else{
			std::string par = arg.substr(0, 1);
			ASSERT(par==hyphen,throwError);
			arg = arg.substr(1);
		}

		vector<string> ArgValue;
		string param = arg.substr(0, arg.find("="));
		string value = arg.substr(arg.find("=")+1);
		ArgValueMap[param] = value;

	}

}



void
CommandLine::Get(const string& key, int& var) {
	if (ArgValueMap.find(key) != ArgValueMap.end())
		var = std::stoi(ArgValueMap.at(key));
}

void
CommandLine::Get(const string& key, string& var) {
	if (ArgValueMap.find(key) != ArgValueMap.end())
		var = ArgValueMap.at(key);
}

void
CommandLine::Get(const string& key, unsigned int& var) {
	if (ArgValueMap.find(key) != ArgValueMap.end())
		var = std::stoul(ArgValueMap.at(key));
}

void
CommandLine::Get(const string& key, uint64_t& var) {
	if (ArgValueMap.find(key) != ArgValueMap.end())
		var = std::stoull(ArgValueMap.at(key));
}

void
CommandLine::Get(const string& key, bool& var) {
	if (ArgValueMap.find(key) != ArgValueMap.end()){
		string bin = ArgValueMap.at(key);

		if (bin == "true" || "True"){
			var = true;
		}
		else if (bin == "false"|| "False"){
			var = false;
		}
		else{
			ASSERT(0,"Invalid arguments for bool type. Use true or True or false or False");
		}
	}
}

void
CommandLine::Get(const string& key, double& var) {
	if (ArgValueMap.find(key) != ArgValueMap.end())
		var = std::stod(ArgValueMap.at(key));
}

void
CommandLine::Get(const string& key, float& var) {
	if (ArgValueMap.find(key) != ArgValueMap.end())
		var = std::stof(ArgValueMap.at(key));
}



} // namespace simulator
