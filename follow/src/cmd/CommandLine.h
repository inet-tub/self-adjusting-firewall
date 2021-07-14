/*
 * CommandLine.h
 *
 *  Created on: Jul 14, 2021
 *      Author: vamsi
 */

#ifndef CMD_COMMANDLINE_H_
#define CMD_COMMANDLINE_H_

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

namespace simulator{

class CommandLine {

public:

	CommandLine();

	virtual ~CommandLine();

	void ParseArgs(int argc, char* argv[]);

	void Get(const string& key, int& var);

	void Get(const string& key, string& var);

	void Get(const string& key, unsigned int& var);

	void Get(const string& key, uint64_t& var);

	void Get(const string& key, bool& var);

	void Get(const string& key, double& var);

	void Get(const string& key, float& var);
private:
	unordered_map<string, string> ArgValueMap;
};

} // namespace simulator

#endif /* CMD_COMMANDLINE_H_ */
