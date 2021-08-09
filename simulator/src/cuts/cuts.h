/*
 * cuts.h
 *
 *  Created on: Jul 19, 2021
 *      Author: vamsi
 */

#ifndef CUTS_CUTS_H_
#define CUTS_CUTS_H_

#define MAXDIMENSIONS 5
#define MAX_ALLOWED_LEVELS 200

#define PTR_SIZE 4
#define HEADER_SIZE 4
#define BOUNDARY_SIZE 26
#define INTERVAL_SIZE 2

#define INTERNAL_NODE_SIZE (HEADER_SIZE + BOUNDARY_SIZE + PTR_SIZE)
#define LEAF_NODE_SIZE HEADER_SIZE

#define MAX_MEMBINS 4

#define TOO_MUCH 16

#define NUM_JUNK 5

# include "./../classifier/baseclassifier.h"
#include "./../cmd/CommandLine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <list>
#include <map>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <cmath>

using namespace std;

namespace simulator{

struct range{
  unsigned long long low;
  unsigned long long high;
};

struct pc_rule{
  int priority;
  struct range field[MAXDIMENSIONS];
  int siplen, diplen;
  unsigned sip[4], dip[4];
};

struct node
{
  int depth;
  int problematic;
  int node_has_rule=0;
  pc_rule boundary;
  list <pc_rule*> classifier;
  list <node *> children;
  list <node *> nextGen;
  int cuts[MAXDIMENSIONS];
  // this is used only if this node is a result
  // of cutting in 2D
  int Row;
  int Column;
  int Index;

  bool is_compressed;
};

struct TreeStat
{
  // independent vars
  int Id;
  int No_Rules;
  int Max_Depth;
  int Max_Levels;
  int Max_Cuts;
  int Rules_at_the_Leaf;
  int Rules_along_path;
  unsigned long long Total_Rule_Size;
  unsigned long long Total_Rules_Moved_Up;
  unsigned long long Total_Array_Size;
  unsigned long long Node_Count;
  unsigned long long Problematic_Node_Count;
  unsigned long long NonLeaf_Node_Count;
  unsigned long long Compressed_NonLeaf_Node_Count;
  unsigned long long Uncompressed_NonLeaf_Node_Count;
  map <unsigned,unsigned long long> interval_per_node;
  map <unsigned,unsigned long long> cuts_per_node;
  // dependent vars
  unsigned long long ruleptr_memory;
  unsigned long long array_memory;
  unsigned long long leaf_node_memory;
  unsigned long long compressed_int_node_memory;
  unsigned long long uncompressed_int_node_memory;
  unsigned long long total_memory;
  unsigned long long total_memory_in_KB;
};

struct MemBin
{
  int Max_Depth;
  int Max_Levels;
  unsigned long long total_memory;
  list<TreeStat*> Trees;
};

class cuts : public BaseClassifier {

public:
	cuts();
	virtual ~cuts();

    int BuildClassifier();

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
    	return double(totalNodesTraversed)/totalAccess;
    }

    void parseargs(CommandLine* cmd);

    int loadrule(FILE *fp);

    void IP2Range(unsigned ip1,unsigned ip2,unsigned ip3,unsigned ip4,unsigned iplen,pc_rule *rule,int index);

    void LoadRulePtr(list <pc_rule> &rule_list,list <pc_rule*> &ruleptr_list,int start,int end);


//    bool mycomparison(Rule* first,Rule* second);

//    bool myequal(Rule* first,Rule* second);

//    bool mystatsort(TreeStat* first,TreeStat* second);

//    bool mymemsort(MemBin* first,MemBin* second);

    int CheckIPBounds(range fld);

    int CheckPortBounds(range fld);

    int CheckProtoBounds(range fld);

    void ClearMem(node *curr_node);

    bool IsPowerOfTwo(int x);

    pc_rule get_bound(node *curr_node,int *offset);

    bool is_present(pc_rule boundary,pc_rule *rule);

    void modifyrule(pc_rule boundary,pc_rule *rule);

    bool is_equal(pc_rule rule1,pc_rule rule2, pc_rule boundary);

    void remove_redund(node *curr_node);

    void calc_dimensions_to_cut(node *curr_node,int *select_dim);

    void cp_node(node* src,node* dest);

    void calc_num_cuts_1D(node *root,int dim);

    void LinearizeChildren(int RowSize);

    void SortChildren();

    void calc_num_cuts_2D(node *root,int *dim);

    void calc_cuts(node *curr_node);

    void createBoundary(node *a,node *b,node *c);

    int LogicalMerge(node* a,node* b,int Max);

    bool NodeCompress(list <node*> &nodelist);

    void InitStats(int No_Rules);

    void NodeStats(node *curr_node);

    void InterValHist(map <unsigned,unsigned long long> interval_per_node);

    void CutsHist(map <unsigned,unsigned long long> cuts_per_node);

    void PrintStatRecord(TreeStat *p_record);

    void PrintStats();

    void RecordTreeStats();

    void moveRulesUp(node* curr_node);

    int samerules(node * r1, node * r2);

    list<node*> nodeMerging(node * curr_node);

    void regionCompaction(node * curr_node);

    void create_tree(list <pc_rule*> p_classifier);

    void binRules();

    void MergeTrees();

    void BinPack(int bins,list <TreeStat*> Statistics);

    int ComputeCutoffs();

    static bool mymemsort(MemBin* first,MemBin* second);

    static bool mystatsort(TreeStat* first,TreeStat* second);

    static bool mycomparison(pc_rule* first,pc_rule* second);

    static bool myequal(pc_rule* first,pc_rule* second);

    int AccessTrees(const Packet& p);

    int AccessTree(node* item, const Packet& p);

    int AccessList(list<pc_rule*> &roots, const Packet& p);

    bool inline MatchesPacket(const Packet& p, pc_rule * r) ;

    bool inline IntersectsRule(pc_rule *r1, pc_rule *r2);

    void PrintTrees();

private:
    uint64_t totalNodesTraversed=0; // May overflow
    uint64_t totalAccess=0;
    int Num_Junk;

    unsigned long long Percents[NUM_JUNK] =
    {
      83,
      10,
      4,
      2,
      1
    };

    unsigned long long Cutoffs[NUM_JUNK];

    int bucketSize = 16;
    double spfac = 8.0;
    FILE *fpr;
    int hypercuts = 1;
    int compressionON = 1;
    int binningON = 1;
    int mergingON = 1;
    int num_intervals = 7;
    double bin = 0.5;
    double IPbin = 0.05;
    int thirtyone = 0;
    int Num_Rules_Moved_Up = 0;

    int numTrees = 0;

    // tree related
    list <pc_rule> classifier;
    list <pc_rule*> p_classifier;
    int numrules=0;
    node *root;
    list <node*> childlist;
    list <node*> nextGenlist;

    int rulelists[31];
    list<pc_rule*> bigrules[5];
    list<pc_rule*> kindabigrules[10];
    list<pc_rule*> mediumrules[10];
    list<pc_rule*> littlerules[5];
    list<pc_rule*> smallrules;

    int Num_Partitions;
    int Avg_Degree;
    int Max_Degree;
    unsigned long long Max_WorklistSize;
    // Statistics
    // live records
    int Max_Depth;
    int Max_Levels;
    int Max_Cuts;
    int Rules_at_the_Leaf;
    int Rules_along_path;
    unsigned long long Total_Rule_Size;
    unsigned long long Total_Rules_Moved_Up;
    unsigned long long Total_Array_Size;
    unsigned long long Node_Count;
    unsigned long long Problematic_Node_Count;
    unsigned long long NonLeaf_Node_Count;
    unsigned long long Compressed_NonLeaf_Node_Count;
    unsigned long long Uncompressed_NonLeaf_Node_Count;
    map <unsigned,unsigned long long> interval_per_node;
    map <unsigned,unsigned long long> cuts_per_node;
    // accumulated records
    int treecount = 0;
    TreeStat* p_record;
    list <TreeStat*> Statistics;

    list<node* > allRoots;

    bool accessFound = false;

    uint64_t totalMemoryBytes;
	
};



} /* namespace simulator */



#endif /* CUTS_CUTS_H_ */
