/*
* Copyright (c) 2021 Peng Cheng Laboratory.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at:
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#pragma once
#ifndef  _TRIE_H
#define  _TRIE_H

#include<stdio.h>
#include<stdlib.h>
#include<queue>
#include <vector>

#include "hs.h"
#include "stdinc.h"

using namespace std;

namespace simulator{
class trie {

	struct range{
	  unsigned long long low;
	  unsigned long long high;
	};

	struct nodeItem {
		bool	isleaf;
		//pc_rule	*rulelist;
		int	nrules;
		range	field[MAXDIMENSIONS];
		int	*ruleid;
		unsigned int	ncuts[MAXDIMENSIONS];
		hs_node_t* rootnode;
		int*	child;
		int	layNo;
		int	flag;     //Cutting or Splitting
	};
public:

  	int	binth;
	int	pass;			// max trie level
	int	k;                      //dim. of small
	int dim[MAXDIMENSIONS];
	int	freelist;		// first nodeItem on free list
	unsigned int	threshold;

	int	Total_Rule_Size;	// number of rules stored
	int	Total_Array_Size;
	int	Leaf_Node_Count;
	int	NonLeaf_Node_Count;
	float	total_ficuts_memory_in_KB;
	float	total_hs_memory_in_KB;
	float	total_memory_in_KB;

	int speedUpFlag;//3:sa&da,1:sa, 2:da, others:for !5-tuple rules


	int	max_depth;
	int	numrules;

	vector<Rule*> rule;

	int	root;		// root of trie
	nodeItem *nodeSet;	// base of array of NodeItems

	double queryCount[1] = {0};

       queue<int> qNode;	//queue for node
       trie(int numrules1, int binth1, vector<Rule*> &rule1, vector<Rule*> &rule2, int threshold1, int k1);
	   ~trie();
       void  count_np_ficut1(nodeItem*);
	    void  count_np_ficut2(nodeItem*);


       void createtrie();
	   int trieLookup(const Packet& p);
	   double GetQueried()
	   {
		   return queryCount[0];
	   }

	   void ClearQueried(){
			queryCount[0]=0;
	   }
	   vector<Rule*> subset_node;

	   unsigned int get_pow(unsigned int n);
	   unsigned int get_nbits(unsigned int n);
};
} /* namepace simulator */
#endif
