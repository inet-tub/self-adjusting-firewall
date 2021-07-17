/*
 * cuts.cpp
 *
 *  Created on: Jul 16, 2021
 *      Author: vamsi
 */

// Copyright to be added later
// Originally from Efficuts authors. Majority of the code in this file is from the original implementation.
// Modified by Vamsi

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



#include "cuts.h"
#include "./../includes/Assert.h"
#include <stdint.h>


namespace simulator{


bool mycomparison(Rule* first,Rule* second)
{
  return (first->priority < second->priority);
}

bool myequal(Rule* first,Rule* second)
{
  return (first->priority == second->priority);
}

bool mystatsort(TreeStat* first,TreeStat* second)
{
  if (first->Max_Depth > second->Max_Depth)
  {
    return true;
  }
  else
  {
    if (first->Max_Depth == second->Max_Depth)
    {
      if (first->total_memory > second->total_memory)
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    else
    {
      return false;
    }
  }
}

bool mymemsort(MemBin* first,MemBin* second)
{
  if (first->Max_Depth < second->Max_Depth)
  {
    return true;
  }
  else
  {
    if (first->Max_Depth == second->Max_Depth)
    {
      if (first->total_memory < second->total_memory)
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    else
    {
      return false;
    }
  }
}

int cuts::CheckIPBounds(range fld)
{
  if (fld.low > 0xFFFFFFFF)
  {
    printf("Error: IPRange is buggy!(%llu)\n",fld.low);
    return 1;
  }
  if (fld.high > 0xFFFFFFFF)
  {
    printf("Error: IPRange is buggy!(%llu)\n",fld.high);
    return 1;
  }
  if (fld.low > fld.high)
  {
    printf("Error: IPRange is buggy!(%llu - %llu)\n",fld.low,fld.high);
    return 1;
  }
  return 0;
}

int cuts::CheckPortBounds(range fld)
{
  if (fld.low > 0xFFFF)
  {
    printf("Error: PortRange is buggy!(%llu)\n",fld.low);
    return 1;
  }
  if (fld.high > 0xFFFF)
  {
    printf("Error: PortRange is buggy!(%llu)\n",fld.high);
    return 1;
  }
  if (fld.low > fld.high)
  {
    printf("Error: PortRange is buggy!(%llu - %llu)\n",fld.low,fld.high);
    return 1;
  }
  return 0;
}

int cuts::CheckProtoBounds(range fld)
{
  if (fld.low > 0xFF)
  {
    printf("Error: ProtoRange is buggy!(%llu)\n",fld.low);
    return 1;
  }
  if (fld.high > 0xFF)
  {
    printf("Error: ProtoRange is buggy!(%llu)\n",fld.high);
    return 1;
  }
  if (fld.low > fld.high)
  {
    printf("Error: ProtoRange is buggy!(%llu - %llu)\n",fld.low,fld.high);
    return 1;
  }
  return 0;
}

void cuts::parseargs(CommandLine* cmd) {
  int	c;
  bool	ok = 1;

  bool help=false;

  cmd->Get("bucketSize",bucketSize);
  cmd->Get("spfac",spfac);
  cmd->Get("num_intervals",num_intervals);
  cmd->Get("hypercuts",hypercuts);
  cmd->Get("Num_Rules_Moved_Up",Num_Rules_Moved_Up);
  cmd->Get("compressionON",compressionON);
  cmd->Get("binningON",binningON);
  cmd->Get("mergingON",mergingON);
  cmd->Get("bin",bin);
  cmd->Get("IPbin",IPbin);
  cmd->Get("thirtyone",thirtyone);
  cmd->Get("cutsHelp",help);

  if (help){
	  printf("valid options:\n "
			  "--bucketSize bucketSize \n"
			  "--spfac spfac\n"
			  "--num_intervals num_intervals\n"
			  "--hypercuts (0 for hicuts| 1 for hypercuts)\n"
			  "-u pushup\n"
			  "-c (0|1)\n"
			  "--bin bin\n"
			  "--IPbin IPbin\n"
			  "--cutsHelp\n"
			  );
	  exit(1);
  }


  if(bucketSize <= 0){
    printf("bucketSize should be > 0\n");
    ok = 0;
  }

  if(spfac < 0){
    printf("space factor should be >= 0\n");
    ok = 0;
  }

  if (compressionON > 1) {
    printf("c can be only 0 - no compress, 1 - linear\n");
    ok = 0;
  }

  if (binningON > 2) {
    printf("g can be only 0 - no binning, 1 - binning 2 - static\n");
    ok = 0;
  }

  if (hypercuts > 1) {
    printf("m can be only 0 - hicut, 1 - hypercut\n");
    ok = 0;
  }

  if(num_intervals < 0){
    printf("num_intervals should be >= 0\n");
    ok = 0;
  }

  if(fpr == NULL){
    printf("can't open ruleset file\n");
    ok = 0;
  }

  if (bin < 0.0 || bin > 1.0)
  {
    printf("bin should be [0,1]\n");
    ok = 0;
  }
  if (IPbin < 0.0 || IPbin > 1.0)
  {
    printf("IP bin should be [0,1]\n");
    ok = 0;
  }
  if (!ok) {
	  fprintf(stderr,"valid options:\n "
	  			  "--bucketSize bucketSize \n"
	  			  "--spfac spfac\n"
	  			  "--num_intervals num_intervals\n"
	  			  "--hypercuts (0 for hicuts| 1 for hypercuts)\n"
	  			  "-u pushup\n"
	  			  "-c (0|1)\n"
	  			  "--bin bin\n"
	  			  "--IPbin IPbin\n"
	  			  "--cutsHelp\n"
	  			  );
    exit(1);
  }

  printf("******************************************\n");
  printf("Bucket Size =  %d\n", bucketSize);
  printf("Space Factor = %f\n", spfac);
  printf("bin = %f\n",bin);
	printf("IPbin = %f\n",IPbin);
  printf("hypercuts = %d\n",hypercuts);
  printf("Num_Rules_Moved_Up = %d\n",Num_Rules_Moved_Up);
  printf("compressionON = %d\n",compressionON);
  printf("binningON = %d\n",binningON);
  printf("mergingON = %d\n",mergingON);
  printf("num_intervals = %d\n",num_intervals);
  printf("******************************************\n");
}

void cuts::ClearMem(node *curr_node)
{
  curr_node->classifier.clear();
  curr_node->children.clear();
  delete curr_node;
};

bool cuts::IsPowerOfTwo(int x)
{
  return (x & (x - 1)) == 0;
}

Rule cuts::get_bound(node *curr_node,int *offset)
{
  Rule boundary;
  unsigned long long interval;

  for (int i = 0;i < MAXDIMENSIONS;i++)
  {
    interval = curr_node->boundary.range[i][HighDim] - curr_node->boundary.range[i][LowDim] + 1;
    interval = interval / curr_node->cuts[i];
    boundary.range[i][LowDim] = curr_node->boundary.range[i][LowDim] + offset[i] * interval;
    if (offset[i] == curr_node->cuts[i] - 1)
      boundary.range[i][HighDim] = curr_node->boundary.range[i][HighDim];
    else
      if (interval == 0)
        boundary.range[i][HighDim] = boundary.range[i][LowDim];
      else
        boundary.range[i][HighDim] = boundary.range[i][LowDim] + interval - 1;
  }
  // check the node's bounds
  range ra; ra.low = boundary.range[0][LowDim]; ra.high = boundary.range[0][HighDim];
  if (CheckIPBounds(ra))
  {
    printf("Error: get_bound bounds check for 0 failed\n");
    printf("[%llu - %llu] => [%llu - %llu] @ %d\n",
      curr_node->boundary.range[0][LowDim],curr_node->boundary.range[0][HighDim],
      boundary.range[0][LowDim],boundary.range[0][HighDim],curr_node->cuts[0]);
    exit(1);
  }
  ra.low = boundary.range[1][LowDim]; ra.high = boundary.range[1][HighDim];
  if (CheckIPBounds(ra))
  {
    printf("Error: get_bound bounds check for 1 failed\n");
    printf("[%llu - %llu] => [%llu - %llu] @ %d\n",
      curr_node->boundary.range[1][LowDim],curr_node->boundary.range[1][HighDim],
      boundary.range[1][LowDim],boundary.range[1][HighDim],curr_node->cuts[1]);
    exit(1);
  }
  ra.low = boundary.range[2][LowDim]; ra.high = boundary.range[2][HighDim];
  if (CheckPortBounds(ra))
  {
    printf("Error: get_bound bounds check for 2 failed\n");
    printf("[%llu - %llu] => [%llu - %llu] @ %d\n",
      curr_node->boundary.range[2][LowDim],curr_node->boundary.range[2][HighDim],
      boundary.range[2][LowDim],boundary.range[2][HighDim],curr_node->cuts[2]);
    exit(1);
  }
  ra.low = boundary.range[3][LowDim]; ra.high = boundary.range[3][HighDim];
  if (CheckPortBounds(ra))
  {
    printf("Error: get_bound bounds check for 3 failed\n");
    printf("[%llu - %llu] => [%llu - %llu] @ %d\n",
      curr_node->boundary.range[3][LowDim],curr_node->boundary.range[3][HighDim],
      boundary.range[3][LowDim],boundary.range[3][HighDim],curr_node->cuts[3]);
    exit(1);
  }
  ra.low = boundary.range[4][LowDim]; ra.high = boundary.range[4][HighDim];
  if (CheckProtoBounds(ra))
  {
    printf("Error: get_bound bounds check for 4 failed\n");
    printf("[%llu - %llu] => [%llu - %llu] @ %d\n",
      curr_node->boundary.range[4][LowDim],curr_node->boundary.range[4][HighDim],
      boundary.range[4][LowDim],boundary.range[4][HighDim],curr_node->cuts[4]);
    exit(1);
  }
  return boundary;
}

bool cuts::is_present(Rule boundary,Rule *rule)
{
  if ( ((rule->range[0][LowDim]  <= boundary.range[0][LowDim]  && rule->range[0][HighDim] >= boundary.range[0][LowDim])  ||  // cuts to the left of range
        (rule->range[0][HighDim] >= boundary.range[0][HighDim] && rule->range[0][LowDim]  <= boundary.range[0][HighDim]) ||  // cuts to the right of range
        (rule->range[0][LowDim]  >= boundary.range[0][LowDim]  && rule->range[0][HighDim] <= boundary.range[0][HighDim])) && // completely inside the range
      ((rule->range[1][LowDim]  <= boundary.range[1][LowDim]  && rule->range[1][HighDim] >= boundary.range[1][LowDim])  ||  // cuts to the left of range
       (rule->range[1][HighDim] >= boundary.range[1][HighDim] && rule->range[1][LowDim]  <= boundary.range[1][HighDim]) ||  // cuts to the right of range
       (rule->range[1][LowDim]  >= boundary.range[1][LowDim]  && rule->range[1][HighDim] <= boundary.range[1][HighDim])) && // completely inside the range
      ((rule->range[2][LowDim]  <= boundary.range[2][LowDim]  && rule->range[2][HighDim] >= boundary.range[2][LowDim])  ||  // cuts to the left of range
       (rule->range[2][HighDim] >= boundary.range[2][HighDim] && rule->range[2][LowDim]  <= boundary.range[2][HighDim]) ||  // cuts to the right of range
       (rule->range[2][LowDim]  >= boundary.range[2][LowDim]  && rule->range[2][HighDim] <= boundary.range[2][HighDim])) && // completely inside the range
      ((rule->range[3][LowDim]  <= boundary.range[3][LowDim]  && rule->range[3][HighDim] >= boundary.range[3][LowDim])  ||  // cuts to the left of range
       (rule->range[3][HighDim] >= boundary.range[3][HighDim] && rule->range[3][LowDim]  <= boundary.range[3][HighDim]) ||  // cuts to the right of range
       (rule->range[3][LowDim]  >= boundary.range[3][LowDim]  && rule->range[3][HighDim] <= boundary.range[3][HighDim])) && // completely inside the range
      ((rule->range[4][LowDim]  <= boundary.range[4][LowDim]  && rule->range[4][HighDim] >= boundary.range[4][LowDim])  ||  // cuts to the left of range
       (rule->range[4][HighDim] >= boundary.range[4][HighDim] && rule->range[4][LowDim]  <= boundary.range[4][HighDim]) ||  // cuts to the right of range
       (rule->range[4][LowDim]  >= boundary.range[4][LowDim]  && rule->range[4][HighDim] <= boundary.range[4][HighDim])) )  // completely inside the range
  {
    return true;
  }
  else
  {
    return false;
  }

}

void cuts::modifyrule(Rule boundary,Rule *rule)
{
  for (int i = 0;i < MAXDIMENSIONS;i++)
  {
    if (rule->range[i][LowDim] < boundary.range[i][LowDim])
      rule->range[i][LowDim] = boundary.range[i][LowDim];
    if (rule->range[i][HighDim] > boundary.range[i][HighDim])
      rule->range[i][HighDim] = boundary.range[i][HighDim];
  }
}

bool cuts::is_equal(Rule rule1,Rule rule2, Rule boundary)
{
  int count = 0;
  range r1, r2;
  for (int i = 0;i < MAXDIMENSIONS;i++)
  {
    if (rule1.range[i][LowDim] > boundary.range[i][LowDim]) {
      r1.low = rule1.range[i][LowDim];
    } else {
      r1.low = boundary.range[i][LowDim];
    }
    if (rule1.range[i][HighDim] < boundary.range[i][HighDim]) {
      r1.high = rule1.range[i][HighDim];
    } else {
      r1.high = boundary.range[i][HighDim];
    }
    if (rule2.range[i][LowDim] > boundary.range[i][LowDim]) {
      r2.low = rule2.range[i][LowDim];
    } else {
      r2.low = boundary.range[i][LowDim];
    }
    if (rule2.range[i][HighDim] < boundary.range[i][HighDim]) {
      r2.high = rule2.range[i][HighDim];
    } else {
      r2.high = boundary.range[i][HighDim];
    }
    if (r1.low <= r2.low && r1.high >= r2.high)
    {
      count++;
    }
  }

  if (count == MAXDIMENSIONS)
    return true;
  else
    return false;
}

void cuts::remove_redund(node *curr_node)
{
  list <Rule*> rulelist;
  rulelist.clear();

  for (auto rule : curr_node->classifier)
  {
    int found = 0;
    for (auto mule : rulelist)
    {
      if (is_equal(*mule, *rule, curr_node->boundary) == true)
      {
        found = 1;
      }
    }
    if (found != 1)
    {
      rulelist.push_back(rule);
    }
  }
  // Now add back the rules
  curr_node->classifier.clear();
  curr_node->classifier = rulelist;
  curr_node->classifier.unique(myequal);
}

void cuts::calc_dimensions_to_cut(node *curr_node,int *select_dim)
{
  int unique_elements[MAXDIMENSIONS];
  double average = 0;
  //int average = 0;
  range check;
  for (int i = 0;i < MAXDIMENSIONS;++i)
  {
    list <range> rangelist;
    rangelist.clear();
    for (list<Rule*>::iterator rule = curr_node->classifier.begin();
        rule != curr_node->classifier.end();++rule)
    {
      int found = 0;
      if ((*rule)->range[i][LowDim]> curr_node->boundary.range[i][LowDim]) {
        check.low = (*rule)->range[i][LowDim];
      } else {
        check.low = curr_node->boundary.range[i][LowDim];
      }
      if ((*rule)->range[i][HighDim]< curr_node->boundary.range[i][HighDim]) {
        check.high = (*rule)->range[i][HighDim];
      } else {
        check.high = curr_node->boundary.range[i][HighDim];
      }
      for (list <range>::iterator range = rangelist.begin();
          range != rangelist.end();++range)
      {
        if (check.low == (*range).low && check.high == (*range).high)
        {
          found = 1;
          break;
        }
      }
      if (!found)
        rangelist.push_back(check);
    }
    unique_elements[i] = rangelist.size();
    //  printf("unique_elements[%d] = %d\n",i,unique_elements[i]);

  }
  int dims_cnt = 0;
  for (int i = 0;i < MAXDIMENSIONS;++i)
  {
    if (curr_node->boundary.range[i][HighDim]> curr_node->boundary.range[i][LowDim])
    {
      average += unique_elements[i];
      dims_cnt++;
    }
  }
  average = average / dims_cnt;

  int max = -1;
  for (int i = 0;i < MAXDIMENSIONS;++i)
  {
    if (curr_node->boundary.range[i][HighDim] > curr_node->boundary.range[i][LowDim])
      if (unique_elements[i] > max)
        max = unique_elements[i];
  }

  for (int i = 0;i < MAXDIMENSIONS;++i)
    select_dim[i] = 0;

  int dim_count = 0;
  for (int i = 0;i < MAXDIMENSIONS;++i)
  {
    if (curr_node->boundary.range[i][HighDim] > curr_node->boundary.range[i][LowDim])
    {
      if (hypercuts)
      {
        if (unique_elements[i] >= average)
        {
          select_dim[i] = 1;
          dim_count++;
          // don't cut on more than 2 dimensions
          if (dim_count == 2)
            break;
        }
      }
      else
      {
        if (unique_elements[i] == max)
        {
          select_dim[i] = 1;
          break;
        }
      }
    }
  }

}

void cuts::cp_node(node* src,node* dest)
{
  dest->depth = src->depth;

  dest->boundary.priority = src->boundary.priority;
  for (int i = 0;i < MAXDIMENSIONS;i++)
  {
    dest->boundary.range[i][LowDim]  = src->boundary.range[i][LowDim];
    dest->boundary.range[i][HighDim]= src->boundary.range[i][HighDim];
  }

  dest->classifier = src->classifier;

  dest->children = src->children;

  for (int i = 0;i < MAXDIMENSIONS;i++)
    dest->cuts[i] = src->cuts[i];

  dest->Row = src->Row;
  dest->Column = src->Column;

  dest->Index = src->Index;

  dest->is_compressed = src->is_compressed;

}

void cuts::calc_num_cuts_1D(node *root,int dim)
{
  node *curr_node;

  int nump = 0;

  int spmf = int(floor(root->classifier.size() * spfac));
  int sm;

  int prev_depth = -1;

  int offset[MAXDIMENSIONS];

  int Index;

  if (!childlist.empty())
  {
    printf("Error: Unread Child nodes!\n");
    exit(1);
  }

  node* top = new node;
  cp_node(root,top);

  childlist.push_back(top);

  while (!childlist.empty())
  {
    curr_node = childlist.front();

    if (prev_depth != curr_node->depth)
    {
      if (sm < spmf)
      {
        nump++;
        sm = 1 << nump;
        prev_depth = curr_node->depth;
        Index = 0;
      }
      else
        break;
    }

    for (int k = 0;k < 2;k++)
    {
      curr_node->cuts[dim] = 2;

      node* child = new node;
      child->depth = curr_node->depth + 1;

      for (int i = 0;i < MAXDIMENSIONS;i++)
        if (i == dim)
          offset[i] = k;
        else
          offset[i] = 0;

      child->boundary = get_bound(curr_node,offset);
      child->children.clear();

      for (int i=0;i < MAXDIMENSIONS;i++)
        child->cuts[i] = 1;

      for (list <Rule*>::iterator rule = curr_node->classifier.begin();rule != curr_node->classifier.end();
          ++rule)
      {
        if (is_present(child->boundary,(*rule)) == true)
        {
          child->classifier.push_back(*rule);
        }
      }

      child->Index = Index++;

      child->is_compressed = false;

      sm += child->classifier.size();
      if (child->boundary.range[0][LowDim] == child->boundary.range[0][HighDim] &&
          child->boundary.range[1][LowDim] == child->boundary.range[1][HighDim] &&
          child->boundary.range[2][LowDim] == child->boundary.range[2][HighDim] &&
          child->boundary.range[3][LowDim] == child->boundary.range[3][HighDim] &&
          child->boundary.range[4][LowDim] == child->boundary.range[4][HighDim] )
        if (child->classifier.size() > 1)
        {
          printf("Error: Box 1X1X1X1X1 cannot contain more than 1 rule!\n");
          exit(1);
        }

      childlist.push_back(child);

    }

    childlist.pop_front();

    ClearMem(curr_node);

  }

  root->cuts[dim] = 1 << nump;
}

void cuts::LinearizeChildren(int RowSize)
{

  for (list <node*>::iterator item = childlist.begin();
      item != childlist.end();++item)
  {
    (*item)->Index = (*item)->Column * RowSize + (*item)->Row;
  }

}

void cuts::SortChildren()
{
  list <node*> childlist_sorted;

  childlist_sorted.clear();

  for (int i = 0; i < childlist.size();i++)
  {
    int found = 0;
    for (list <node*>::iterator item = childlist.begin();
        item != childlist.end();++item)
    {
      if ((*item)->Index == i)
      {
        childlist_sorted.push_back(*item);
        found = 1;
        break;
      }
    }
    if (found == 0)
    {
      printf("Error: Index = %d not found in childlist\n",i);
      exit(1);
    }
  }

  if (childlist.size() != childlist_sorted.size())
  {
    printf("Error: Please check Index calculation\n");
    exit(1);
  }

  childlist.clear();

  childlist = childlist_sorted;

}

void cuts::calc_num_cuts_2D(node *root,int *dim)
{
  root->Row = 0;
  root->Column = 0;

  node *curr_node;

  int nump[2];
  for (int i=0;i<2;i++)
    nump[i] = 0;

  int spmf = int(floor(root->classifier.size() * spfac));
  int sm;

  int prev_depth = -1;

  unsigned short chosen = 1;

  int offset[MAXDIMENSIONS];

  if (!childlist.empty())
  {
    printf("Error: Unread Child nodes!\n");
    exit(1);
  }

  node* top = new node;
  cp_node(root,top);

  childlist.push_back(top);

  while (!childlist.empty())
  {
    curr_node = childlist.front();

    if (prev_depth != curr_node->depth)
    {
      if (sm < spmf)
      {
        chosen = chosen ^ 1;
        nump[chosen]++;
        sm = 1 << (nump[0] + nump[1]);
        prev_depth = curr_node->depth;
        //      printf("---------------------------------------\n");
      }
      else
        break;
    }

    //  printf("[%d,%d] -> ",curr_node->Row,curr_node->Column);

    for (int k = 0;k < 2;k++)
    {
      curr_node->cuts[dim[chosen]] = 2;
      curr_node->cuts[dim[chosen ^ 1]] = 1;

      node* child = new node;
      child->depth = curr_node->depth + 1;

      for (int i = 0;i < MAXDIMENSIONS;i++)
        if (i == dim[chosen])
          offset[i] = k;
        else
          offset[i] = 0;

      child->boundary = get_bound(curr_node,offset);
      child->children.clear();

      for (int i=0;i < MAXDIMENSIONS;i++)
        child->cuts[i] = 1;

      for (list <Rule*>::iterator rule = curr_node->classifier.begin();rule != curr_node->classifier.end();
          ++rule)
      {
        if (is_present(child->boundary,*rule) == true)
        {
          child->classifier.push_back(*rule);
        }
      }

      if (chosen == 0)
      {
        child->Row = (2 * curr_node->Row) + k;
        child->Column = curr_node->Column;
      }
      else
      {
        child->Column = (2 * curr_node->Column) + k;
        child->Row = curr_node->Row;
      }

      child->is_compressed = false;

      //    printf("[%d,%d] ",child->Row,child->Column);

      sm += child->classifier.size();
      if (child->boundary.range[0][LowDim] == child->boundary.range[0][HighDim] &&
          child->boundary.range[1][LowDim] == child->boundary.range[1][HighDim] &&
          child->boundary.range[2][LowDim] == child->boundary.range[2][HighDim] &&
          child->boundary.range[3][LowDim] == child->boundary.range[3][HighDim] &&
          child->boundary.range[4][LowDim] == child->boundary.range[4][HighDim] )
        if (child->classifier.size() > 1)
        {
          printf("Box 1X1X1X1X1 cannot contain more than 1 rule!\n");
          exit(1);
        }
      childlist.push_back(child);

    }

    //  printf("\n");

    childlist.pop_front();

    ClearMem(curr_node);

  }

  root->cuts[dim[0]] = ( 1 << nump[0]);
  root->cuts[dim[1]] = ( 1 << nump[1]);

  //printf("%d X %d\n",root->cuts[dim[0]],root->cuts[dim[1]]);
  if (compressionON)
  {
    LinearizeChildren(root->cuts[dim[0]]);
    SortChildren();
  }

}

void cuts::calc_cuts(node *curr_node)
{
  int select_dim[MAXDIMENSIONS];
  int chosen_dim[2];
  int chosen_cnt = 0;

  calc_dimensions_to_cut(curr_node,select_dim);

  for (int i = 0;i < MAXDIMENSIONS;++i)
    if (select_dim[i])
      chosen_dim[chosen_cnt++] = i;

//for (int i = 0;i < chosen_cnt;i++)
//  printf("chosen_dim[%d] = %d [%llu - %llu]\n",i,chosen_dim[i],
//      curr_node->boundary.field[chosen_dim[i]].low,
//      curr_node->boundary.field[chosen_dim[i]].high);


  if (chosen_cnt > 2)
  {
    printf("Error: More than 2 dimensions are cut!\n");
    exit(1);
  }

  if (chosen_cnt > 1 && hypercuts == 0)
  {
    printf("Error: Hicut: More than 1 dimensions are cut!\n");
    exit(1);
  }

  if (chosen_cnt == 0)
  {
    printf("Error: Atleast 1 dimension needs to be cut!\n");
    exit(1);
  }

  if (chosen_cnt == 2)
  {
    calc_num_cuts_2D(curr_node,chosen_dim);
  }
  else if (chosen_cnt == 1)
  {
    calc_num_cuts_1D(curr_node,chosen_dim[0]);
  }

//for (int i = 0;i < chosen_cnt;i++)
//  printf("chosen_dim[%d] = %d, cuts = %d\n",i,chosen_dim[i],curr_node->cuts[chosen_dim[i]]);

}

// makes a = a + b
void cuts::createBoundary(node *a,node *b,node *c)
{
  for (int i = 0;i < MAXDIMENSIONS;i++)
  {
    list<unsigned long long> EndPoints;
    EndPoints.clear();
    EndPoints.push_back(a->boundary.range[i][LowDim]);
    EndPoints.push_back(a->boundary.range[i][HighDim]);
    EndPoints.push_back(b->boundary.range[i][LowDim]);
    EndPoints.push_back(b->boundary.range[i][HighDim]);
    EndPoints.sort();
    c->boundary.range[i][LowDim]  = EndPoints.front();
    c->boundary.range[i][HighDim] = EndPoints.back();
  }

}

int cuts::LogicalMerge(node* a,node* b,int Max)
{
  node *c = new node;
  cp_node(a,c);
  createBoundary(a,b,c);
  c->classifier.clear();


  // create a sort list of rules in a or b in rules
  list<Rule*> templist = b->classifier;
  c->classifier = a->classifier;
  c->classifier.merge(templist,mycomparison);
  c->classifier.unique(myequal);

  // remove redundant rules
  remove_redund(c);

  int asize = a->classifier.size();
  int bsize = b->classifier.size();
  int csize = c->classifier.size();
  int maxsize = (asize > bsize) ? asize : bsize;

  if (c->classifier.size() <= bucketSize ||
      ((c->classifier.size() <= maxsize) &&
       (c->classifier.size() < Max)) )
  {
    cp_node(c,a);
    ClearMem(c);
    return 1;
  }
  else
  {
    ClearMem(c);
    return 0;
  }
}

bool cuts::NodeCompress(list <node*> &nodelist)
{
  int Max;
  int merge_possible = compressionON;
  int original_size = nodelist.size();
  while (merge_possible)
  {
    merge_possible = 0;

    // find the max. rules among all child nodes
    Max = 0;
    for (list <node*>::iterator item = nodelist.begin();
        item != nodelist.end();++item)
    {
      if ((*item)->classifier.size() > Max)
        Max = (*item)->classifier.size();
    }


    for (list <node*>::iterator item = nodelist.begin();
        item != nodelist.end();++item)
    {
      ++item;
      list <node*>::iterator item_p1 = item;
      --item;
      if (item_p1 != nodelist.end())
      {
        if (LogicalMerge(*item,*item_p1,Max))
        {
          nodelist.erase(item_p1);
          ClearMem(*item_p1);
          merge_possible = 1;
        }
      }
    }
  }

  if (nodelist.size() > original_size)
  {
    printf("Error: Compression resulted in increase of nodes!\n");
    exit(1);
  }

  if (nodelist.size() < original_size)
    return true;
  else
    return false;

}

void cuts::InitStats(int No_Rules)
{
  p_record = new TreeStat;
  p_record->Id = treecount++;
  p_record->No_Rules = No_Rules;

  Max_Depth = 0;
  Max_Levels = 0;
  Max_Cuts = 0;
  Rules_at_the_Leaf = 0;
  Rules_along_path = 0;
  Max_WorklistSize = 0;
  Node_Count = 0;
  Problematic_Node_Count = 0;
  NonLeaf_Node_Count = 0;
  Compressed_NonLeaf_Node_Count = 0;
  Uncompressed_NonLeaf_Node_Count = 0;
  Total_Array_Size = 0;
  Total_Rule_Size = 0;
  Total_Rules_Moved_Up = 0;
  Max_Degree = 0;
  Avg_Degree = 0;
  Num_Partitions = 0;

  interval_per_node.clear();
  cuts_per_node.clear();
}

void cuts::NodeStats(node *curr_node)
{

  if (curr_node->problematic == 1)
    Problematic_Node_Count++;


  Num_Partitions = curr_node->cuts[0] * curr_node->cuts[1] * curr_node->cuts[2] * curr_node->cuts[3] * curr_node->cuts[4];

  int mcuts = curr_node->cuts[0];
  for (int di = 1;di < MAXDIMENSIONS;di++)
    if (curr_node->cuts[di] > mcuts)
      mcuts = curr_node->cuts[di];

  if (mcuts > Max_Cuts)
    Max_Cuts = mcuts;

  // checks
  if ( curr_node->classifier.size() > bucketSize &&
      curr_node->children.size() == 0 && curr_node->problematic == 0)
  {
    printf("Error: This node is not cut further!\n");
    exit(1);
  }

  if (curr_node->problematic == 1 && curr_node->classifier.size() > TOO_MUCH)
  {
    printf("Error: This problematic node has %d rules!\n",curr_node->classifier.size());
    // Too harsh exit statement here. This depends on the ruleset. Its fine if a node ends up with too many children.
    // exit(1);
  }

  if (Num_Partitions != curr_node->children.size()
      && curr_node->children.size() != 0 && compressionON == 0)
  {
    printf("Error: num children != partitions!(%d != %d)\n",curr_node->children.size(),Num_Partitions);
    exit(1);
  }

  for (int i = 0;i < MAXDIMENSIONS;++i)
    if (IsPowerOfTwo(curr_node->cuts[i]) == false)
    {
      printf("Error: ncuts[%d] = %d is not a power of 2!\n",i,curr_node->cuts[i]);
      exit(1);
    }

  // check the node's bounds
  range ra ; ra.low = curr_node->boundary.range[0][LowDim]; ra.high = curr_node->boundary.range[0][HighDim];
  if (CheckIPBounds(ra))
  {
    printf("Error: NodeStat bounds check for 0 failed\n");
    exit(1);
  }
  ra.low = curr_node->boundary.range[1][LowDim]; ra.high = curr_node->boundary.range[1][HighDim];
  if (CheckIPBounds(ra))
  {
    printf("Error: NodeStat bounds check for 1 failed\n");
    exit(1);
  }
  ra.low = curr_node->boundary.range[2][LowDim]; ra.high = curr_node->boundary.range[2][HighDim];
  if (CheckPortBounds(ra))
  {
    printf("Error: NodeStat bounds check for 2 failed\n");
    exit(1);
  }
  ra.low = curr_node->boundary.range[3][LowDim]; ra.high = curr_node->boundary.range[3][HighDim];
  if (CheckPortBounds(ra))
  {
    printf("Error: NodeStat bounds check for 3 failed\n");
    exit(1);
  }
  ra.low = curr_node->boundary.range[4][LowDim]; ra.high = curr_node->boundary.range[4][HighDim];
  if (CheckProtoBounds(ra))
  {
    printf("Error: NodeStat bounds check for 4 failed\n");
    exit(1);
  }

  // stats
  Node_Count++;

  if (curr_node->children.size() != 0)
  {
    NonLeaf_Node_Count++;
    if (curr_node->is_compressed == true)
      Compressed_NonLeaf_Node_Count++;
    else
      Uncompressed_NonLeaf_Node_Count++;
  }

  if (curr_node->is_compressed == true && curr_node->children.size() == 0)
  {
    printf("Error: How the heck is leaf node compressed, exiting..\n");
    exit(1);
  }

  int Actual_Curr_Level = curr_node->depth;

  if (curr_node->is_compressed != true)
    Actual_Curr_Level++;


  if (Actual_Curr_Level > Max_Levels)
  {

    Max_Levels = Actual_Curr_Level;
    //printf("[Tree %d] currently at level %d ...with %d children\n",p_record->Id,Max_Levels,curr_node->children.size());
    if (Max_Levels > MAX_ALLOWED_LEVELS)
    {
      printf("Error: [Tree %d] more that %d levels!\n",
          p_record->Id,MAX_ALLOWED_LEVELS);
      exit(1);
    }

    if (curr_node->children.empty())
      Rules_at_the_Leaf = curr_node->classifier.size();

    if (curr_node->node_has_rule == 1)
      Rules_along_path++;
  }

  int depth = curr_node->depth + (curr_node->children.empty() ? curr_node->classifier.size() : 0);

  if (depth > Max_Depth)
    Max_Depth = depth;

  if (curr_node->children.size() != 0)
    Total_Array_Size += curr_node->children.size();
  else
  {
    Total_Rule_Size += curr_node->classifier.size();
  }

  if (curr_node->children.size() > Max_Degree)
    Max_Degree = curr_node->children.size();

  Avg_Degree += curr_node->children.size();

  // intervals per node
  if (curr_node->children.size() != 0 && curr_node->is_compressed == true)
  {
    map<unsigned,unsigned long long>::iterator iter = interval_per_node.find(curr_node->children.size());
    if (iter != interval_per_node.end())
    {
      unsigned long long count = iter->second;
      count++;
      interval_per_node[curr_node->children.size()] = count;
    }
    else
    {
      interval_per_node[curr_node->children.size()] = 1;
    }
  }

  // cuts per node
  if (curr_node->children.size() != 0)
  {
    map<unsigned,unsigned long long>::iterator iter = cuts_per_node.find(Num_Partitions);
    if (iter != cuts_per_node.end())
    {
      unsigned long long count = iter->second;
      count++;
      cuts_per_node[Num_Partitions] = count;
    }
    else
    {
      cuts_per_node[Num_Partitions] = 1;
    }
  }

}

void cuts::InterValHist(map <unsigned,unsigned long long> interval_per_node)
{
  for (map<unsigned,unsigned long long>::iterator iter = interval_per_node.begin();
      iter != interval_per_node.end();++iter)
  {
    printf("I %u,%llu\n",(*iter).first,(*iter).second);
  }
}
void cuts::CutsHist(map <unsigned,unsigned long long> cuts_per_node)
{
  for (map<unsigned,unsigned long long>::iterator iter = cuts_per_node.begin();
      iter != cuts_per_node.end();++iter)
  {
    printf("C %u,%llu\n",(*iter).first,(*iter).second);
  }
}

void cuts::PrintStatRecord(TreeStat *p_record)
{
  printf("******************************************\n");
  printf("Tree: %d\n",p_record->Id);
  printf("******************************************\n");
  printf("Rules: %d\n",p_record->No_Rules);
  printf("Depth: %d\n",p_record->Max_Depth);
  printf("Levels: %d\n",p_record->Max_Levels);
  printf("Cuts: %d\n",p_record->Max_Cuts);
  printf("Rules_at_the_Leaf: %d\n",p_record->Rules_at_the_Leaf);
  printf("Rules_along_path: %d\n",p_record->Rules_along_path);
  printf("Total_Rule_Size: %lld\n",p_record->Total_Rule_Size);
  printf("Total_Rules_Moved_Up: %lld\n",p_record->Total_Rules_Moved_Up);
  printf("Total_Array_Size: %lld\n",p_record->Total_Array_Size);
  printf("Node_Count: %lld\n",p_record->Node_Count);
  printf("Problematic_Node_Count: %lld\n",p_record->Problematic_Node_Count);
  printf("NonLeaf_Node_Count: %lld\n",p_record->NonLeaf_Node_Count);
  printf("Compressed_NonLeaf_Node_Count: %lld\n",p_record->Compressed_NonLeaf_Node_Count);
  printf("Uncompressed_NonLeaf_Node_Count: %lld\n",p_record->Uncompressed_NonLeaf_Node_Count);
  printf("------------------------------------------\n");
  printf("ruleptr_memory: %lld\n",p_record->ruleptr_memory);;
  printf("array_memory: %lld\n",p_record->array_memory);;
  printf("leaf_node_memory: %lld\n",p_record->leaf_node_memory);;
  printf("compressed_int_node_memory: %lld\n",p_record->compressed_int_node_memory);;
  printf("uncompressed_int_node_memory: %lld\n",p_record->uncompressed_int_node_memory);;
  printf("total_memory: %lld\n",p_record->total_memory);;
  printf("total_memory_in_KB: %lld\n",p_record->total_memory_in_KB);;
  printf("------------------------------------------\n");
  InterValHist(p_record->interval_per_node);
  printf("------------------------------------------\n");
  CutsHist(p_record->cuts_per_node);
}

void cuts::PrintStats()
{
  unsigned long long OVERALL_MEMORY = 0;
  int OVERALL_DEPTH = 0;
  int OVERALL_LEVELS = 0;
  int No_Rules = 0;

  for (list<TreeStat*>::iterator iter = Statistics.begin();
        iter != Statistics.end();iter++)
  {
    PrintStatRecord(*iter);
    OVERALL_MEMORY += (*iter)->total_memory_in_KB;
    No_Rules += (*iter)->No_Rules;
    if ((*iter)->Max_Depth > OVERALL_DEPTH)
      OVERALL_DEPTH = (*iter)->Max_Depth;
    if ((*iter)->Max_Levels > OVERALL_LEVELS)
      OVERALL_LEVELS = (*iter)->Max_Levels;
  }
  printf("******************************************\n");
  printf("OVERALL_MEMORY: %lld\n",OVERALL_MEMORY);
  printf("OVERALL_DEPTH: %d\n",OVERALL_DEPTH);
  printf("OVERALL_LEVELS: %d\n",OVERALL_LEVELS);

  // some final checks
  if (No_Rules != classifier.size())
  {
    printf("Error: Some rules got dropped while binning!\n");
    exit(1);
  }

}


void cuts::RecordTreeStats()
{
  p_record->Max_Depth = Max_Depth;

  p_record->Max_Levels = Max_Levels;

  p_record->Max_Cuts = Max_Cuts;

  p_record->Rules_at_the_Leaf = Rules_at_the_Leaf;

  p_record->Rules_along_path = Rules_along_path;

  p_record->Total_Rule_Size = Total_Rule_Size;

  p_record->Total_Rules_Moved_Up = Total_Rules_Moved_Up;

  p_record->Total_Array_Size = Total_Array_Size;

  p_record->Node_Count = Node_Count;

  p_record->Problematic_Node_Count = Problematic_Node_Count;

  p_record->NonLeaf_Node_Count = NonLeaf_Node_Count;

  p_record->Compressed_NonLeaf_Node_Count = Compressed_NonLeaf_Node_Count;

  p_record->Uncompressed_NonLeaf_Node_Count = Uncompressed_NonLeaf_Node_Count;

  p_record->interval_per_node = interval_per_node;

  p_record->cuts_per_node = cuts_per_node;

  p_record->ruleptr_memory =  PTR_SIZE * Total_Rule_Size;

  p_record->array_memory = PTR_SIZE * Total_Array_Size;

  p_record->leaf_node_memory = LEAF_NODE_SIZE * (Node_Count - NonLeaf_Node_Count);

  p_record->compressed_int_node_memory = (INTERNAL_NODE_SIZE + INTERVAL_SIZE * num_intervals) *
                                        Compressed_NonLeaf_Node_Count;

  p_record->uncompressed_int_node_memory = INTERNAL_NODE_SIZE * Uncompressed_NonLeaf_Node_Count;

  p_record->total_memory = p_record->ruleptr_memory + p_record->array_memory + p_record->leaf_node_memory
                          + p_record->compressed_int_node_memory + p_record->uncompressed_int_node_memory;

  p_record->total_memory_in_KB = p_record->total_memory / 1024;

  Statistics.push_back(p_record);

}

void cuts::moveRulesUp(node* curr_node) {
  curr_node->node_has_rule = 0;
  list<Rule*> rulesMovedUp, setToCheck;
  int emptyIntersect = 1;
  rulesMovedUp = ((curr_node->children).front())->classifier; // start with this set
  // get list of rules that exist in all
  for (list <node*>::iterator item = (curr_node->children).begin();item != (curr_node->children).end();++item) {
    if (emptyIntersect) {
      setToCheck.clear();
      setToCheck = rulesMovedUp;
      rulesMovedUp.clear();
      for (list<Rule*>::iterator ptr = (*item)->classifier.begin(); ptr != (*item)->classifier.end(); ptr++) {
        for (list<Rule*>::iterator setptr = setToCheck.begin();setptr != setToCheck.end();setptr++) {
          if (*setptr == *ptr) {
            rulesMovedUp.push_back(*setptr);
            break;
          }
        }
      }
      if (rulesMovedUp.empty()) {
        emptyIntersect = 0;
      }
    }
  }

  if (rulesMovedUp.size() > Num_Rules_Moved_Up) {
    // truncate to first bucketSize children
    rulesMovedUp.resize(Num_Rules_Moved_Up);
  }

  // remove duplicate rules from all children
  if (emptyIntersect) {
    for(list<Rule*>::iterator setptr = rulesMovedUp.begin();setptr != rulesMovedUp.end();setptr++)
    {
    //setptr--;
    for (list <node*>::iterator item = (curr_node->children).begin();item != (curr_node->children).end();++item) {
      for (list<Rule*>::iterator ptr = (*item)->classifier.begin();ptr != (*item)->classifier.end();ptr++) {
        if (*setptr == *ptr) {
          ptr = (*item)->classifier.erase(ptr);
          break;
        }
      }
    }
    Total_Rule_Size++;
    Total_Rules_Moved_Up++;
    curr_node->node_has_rule = 1;
    }
  }
  //printf("Rules moved up: %d\n", rulesMovedUp.size());
}

int cuts::samerules(node * r1, node * r2) {
  if (r1->classifier.empty() || r2->classifier.empty()) {
    return 0;
  }
  if (r1->classifier.size() != r2->classifier.size()) {
    return 0;
  }
  int found = 0;
  int num = 0;
  for (list<Rule*>::iterator itr1 = r1->classifier.begin();itr1 != r1->classifier.end();itr1++) {
    found = 0;
    for (list<Rule*>::iterator itr2 = r2->classifier.begin();itr2 != r2->classifier.end();itr2++) {
      if ((**itr1).priority == (**itr2).priority) {
        found = 1;
        num++;
        break;
      }
    }
    if (!found) {
      return 0;
    }
  }
  if (num != r1->classifier.size()) {
    printf("ERROR: Too many or too few rules matched\n");
  }
  return 1;
}

list<node*> cuts::nodeMerging(node * curr_node) {
	list<node*> newlist = curr_node->children;
  int num = 0;
  list<node*>::iterator itr2;
/*	for(list<node*>::iterator junk = curr_node->children.begin(); junk != curr_node->children.end();junk++) {
		remove_redund(junk++);
	}*/

  for (list<node*>::iterator itr1 = newlist.begin(); itr1 != newlist.end(); itr1++) {
    itr2 = itr1;
    itr2++;
    while(itr2 != newlist.end()) {
      if (samerules(*itr1, *itr2)) {
        num++;
        //printf("samerules returned true\n");
        for (int i = 0; i < MAXDIMENSIONS; i++) {
          if ((**itr1).boundary.range[i][LowDim] > (**itr2).boundary.range[i][LowDim]) {
            (**itr1).boundary.range[i][LowDim] = (**itr2).boundary.range[i][LowDim];
          }
          if ((**itr1).boundary.range[i][HighDim] < (**itr2).boundary.range[i][HighDim]) {
            (**itr1).boundary.range[i][HighDim] = (**itr2).boundary.range[i][HighDim];
          }
        }
        ClearMem(*itr2);
        itr2 = newlist.erase(itr2);
      } else {
        itr2++;
      }
    }
  }
  if (num > curr_node->children.size()) {
    printf("Odd: Of %d children, %d were identical\n",newlist.size(),num);
  }
  return newlist;
}

void cuts::regionCompaction(node * curr_node) {
  list<unsigned long long> f0, f1, f2, f3, f4;
  for (list<Rule*>::iterator itr = (curr_node->classifier).begin();itr != (curr_node->classifier).end();itr++) {
    if ((**itr).range[0][LowDim] < curr_node->boundary.range[0][LowDim]) { f0.push_back(curr_node->boundary.range[0][LowDim]);}
    else { f0.push_back((**itr).range[0][LowDim]);}
    if ((**itr).range[0][HighDim] > curr_node->boundary.range[0][HighDim]) {f0.push_back(curr_node->boundary.range[0][HighDim]);}
    else { f0.push_back((**itr).range[0][HighDim]);}

    if ((**itr).range[1][LowDim] < curr_node->boundary.range[1][LowDim]) { f1.push_back(curr_node->boundary.range[1][LowDim]);}
    else { f1.push_back((**itr).range[1][LowDim]);}
    if ((**itr).range[1][HighDim] > curr_node->boundary.range[1][HighDim]) {f1.push_back(curr_node->boundary.range[1][HighDim]);}
    else { f1.push_back((**itr).range[1][HighDim]);}

    if ((**itr).range[2][LowDim] < curr_node->boundary.range[2][LowDim]) { f2.push_back(curr_node->boundary.range[2][LowDim]);}
    else { f2.push_back((**itr).range[2][LowDim]);}
    if ((**itr).range[2][HighDim] > curr_node->boundary.range[2][HighDim]) {f2.push_back(curr_node->boundary.range[2][HighDim]);}
    else { f2.push_back((**itr).range[2][HighDim]);}

    if ((**itr).range[3][LowDim] < curr_node->boundary.range[3][LowDim]) { f3.push_back(curr_node->boundary.range[3][LowDim]);}
    else { f3.push_back((**itr).range[3][LowDim]);}
    if ((**itr).range[3][HighDim] > curr_node->boundary.range[3][HighDim]) {f3.push_back(curr_node->boundary.range[3][HighDim]);}
    else { f3.push_back((**itr).range[3][HighDim]);}

    if ((**itr).range[4][LowDim] < curr_node->boundary.range[4][LowDim]) { f4.push_back(curr_node->boundary.range[4][LowDim]);}
    else { f4.push_back((**itr).range[4][LowDim]);}
    if ((**itr).range[4][HighDim] > curr_node->boundary.range[4][HighDim]) {f4.push_back(curr_node->boundary.range[4][HighDim]);}
    else { f4.push_back((**itr).range[4][HighDim]);}
  }
  f0.sort();
  f1.sort();
  f2.sort();
  f3.sort();
  f4.sort();
  curr_node->boundary.range[0][LowDim] = f0.front();
  curr_node->boundary.range[0][HighDim] = f0.back();
  curr_node->boundary.range[1][LowDim] = f1.front();
  curr_node->boundary.range[1][HighDim] = f1.back();
  curr_node->boundary.range[2][LowDim] = f2.front();
  curr_node->boundary.range[2][HighDim] = f2.back();
  curr_node->boundary.range[3][LowDim] = f3.front();
  curr_node->boundary.range[3][HighDim] = f3.back();
  curr_node->boundary.range[4][LowDim] = f4.front();
  curr_node->boundary.range[4][HighDim] = f4.back();
}

void cuts::create_tree(list <Rule*> p_classifier)
{

  printf("Incoming No of Rules in this tree = %d\n",p_classifier.size());

  list <node*> worklist;

  node *curr_node;

  // create a root node, put all rules in it.
  root = new node;
  root->depth = 1;
  for (int i = 0;i < MAXDIMENSIONS;++i) {
	root->boundary.range[i][LowDim] = 0;
    if (i < 2)
      root->boundary.range[i][HighDim] = 0xffffffff;
    else if (i < 4)
      root->boundary.range[i][HighDim] = 0xffff;
    else
      root->boundary.range[i][HighDim] = 0xff;
  }
  root->children.clear();
  for (int i=0;i < MAXDIMENSIONS;i++)
    root->cuts[i] = 1;

  for (auto i : p_classifier)
  {
    root->classifier.push_back(i);
  }

  root->is_compressed = false;

  remove_redund(root);

  if (root->classifier.size() > bucketSize)
    worklist.push_back(root);
  else
  {
    root->problematic = 0;
    NodeStats(root);
  }

  while (!worklist.empty())
  {
    curr_node = worklist.back();

    worklist.pop_back();

    // HEURISTIC 3
		if (hypercuts) {
			regionCompaction(curr_node);
		}
    calc_cuts(curr_node);

    for (list <node*>::iterator item = childlist.begin();
        item != childlist.end();++item)
    {
      (*item)->depth = curr_node->depth + 1;
      for (int i=0;i<MAXDIMENSIONS;i++)
        (*item)->cuts[i] = 1;
    }

    for (list <node*>::iterator item = childlist.begin();
        item != childlist.end();++item)
      curr_node->children.push_back(*item);

    childlist.clear();

    if (compressionON) {
      moveRulesUp(curr_node);

      // backup the number of children, incase compression
      // can't fit in!
      list <node*> Backup;
      for (list <node*>::iterator item = curr_node->children.begin();
          item != curr_node->children.end();++item)
      {
        // create a new node and make a copy
        node *child_copy = new node;
        cp_node(*item,child_copy);
        Backup.push_back(child_copy);
      }

      curr_node->is_compressed = NodeCompress(curr_node->children);

      if (curr_node->children.size() > num_intervals)
      {
        // printf("Before: %d\n",curr_node->children.size());
        // clear the current children
        for (list <node*>::iterator item = curr_node->children.begin();
            item != curr_node->children.end();++item)
        {
          ClearMem(*item);
        }

        curr_node->children.clear();

        // reload Backup
        for (list <node*>::iterator item = Backup.begin();
            item != Backup.end();++item)
        {
          curr_node->children.push_back(*item);
        }

        curr_node->is_compressed = false;

        // printf("After: %d\n",curr_node->children.size());
      }
      else
      {
        // empty Backup No longer needed
        for (list <node*>::iterator item = Backup.begin();
              item != Backup.end();++item)
        {
          ClearMem(*item);
        }
      }

      Backup.clear();
    }

    if (compressionON == 0 && hypercuts) {
      // HEURISTIC 4
      moveRulesUp(curr_node);
    }

    // HEURISTIC 1 - create a list of nodes that should actually exist - both leaf and non-leaf
    list<node*> topush = nodeMerging(curr_node);

    // HEURISTIC 2
    for (list <node*>::iterator item = topush.begin();item != topush.end();++item)
    {
      remove_redund(*item);
    }

    for (list <node*>::iterator item = topush.begin();
        item != topush.end();++item)
    {

      if ((*item)->classifier.size() > bucketSize)
      {
        if ((*item)->boundary.range[0][LowDim] == curr_node->boundary.range[0][LowDim] && (*item)->boundary.range[0][HighDim] == curr_node->boundary.range[0][HighDim] &&
            (*item)->boundary.range[1][LowDim] == curr_node->boundary.range[1][LowDim] && (*item)->boundary.range[1][HighDim] == curr_node->boundary.range[1][HighDim] &&
            (*item)->boundary.range[2][LowDim] == curr_node->boundary.range[2][LowDim] && (*item)->boundary.range[2][HighDim] == curr_node->boundary.range[2][HighDim] &&
            (*item)->boundary.range[3][LowDim] == curr_node->boundary.range[3][LowDim] && (*item)->boundary.range[3][HighDim] == curr_node->boundary.range[3][HighDim] &&
            (*item)->boundary.range[4][LowDim] == curr_node->boundary.range[4][LowDim] && (*item)->boundary.range[4][HighDim] == curr_node->boundary.range[4][HighDim] &&
            (*item)->classifier.size() == curr_node->classifier.size())
        {
          printf("Warning: parent and child are identical with %d rules!\n",curr_node->classifier.size());
          (*item)->problematic = 1;
          NodeStats(*item);
          ClearMem(*item);
        }
        else
        {
          worklist.push_back(*item);
          if (worklist.size() > Max_WorklistSize)
          {
            Max_WorklistSize = worklist.size();
            if (Max_WorklistSize % 100 == 0)
              printf("Worklist.size() = %lld\n",Max_WorklistSize);
          }
        }
      }
      else
      {
        if (! (*item)->classifier.empty())
        {
          (*item)->problematic = 0;
          NodeStats(*item);
        }
        ClearMem(*item);
      }
    }

    curr_node->problematic = 0;
    NodeStats(curr_node);

    ClearMem(curr_node);

  }

}

void cuts::binRules() {
  int min, wild;
  int secondmin, thirdmin;
  double field[5];
  for (list<Rule>::iterator itr = classifier.begin(); itr != classifier.end(); itr++) {
    wild = 0;
    field[0] = ((double) ((*itr).range[0][HighDim] - (*itr).range[0][LowDim]))/0xFFFFFFFF;
    field[1] = ((double) ((*itr).range[1][HighDim] - (*itr).range[1][LowDim]))/0xFFFFFFFF;
    field[2] = ((double) ((*itr).range[2][HighDim] - (*itr).range[2][LowDim]))/65535;
    field[3] = ((double) ((*itr).range[3][HighDim] - (*itr).range[3][LowDim]))/65535;
    if (((*itr).range[4][LowDim] == 0) && ((*itr).range[4][HighDim] == 0xFF)) {
      field[4] = 1;
      wild++;
    } else {
      field[4] = 0;
    }
    /*for (int i = 0; i < 5; i++) {
      printf("Field %d is %f, "i, field[i],priority);
      }*/
    //printf("\n");
    min = 0;
		if (field[0] >= IPbin) { wild++; }
		if (field[1] >= IPbin) { wild++; }
		if (field[2] >= bin) { wild++; }
		if (field[3] >= bin) { wild++; }
    for (int i = 0; i < 4; i++) {
      if (field[i] < field[min]) {
        min = i;
      }
    }
    if (wild >= 4) {
      if ((field[0] > IPbin) && (field[1] > IPbin) && (field[2] > bin) && (field[3] > bin) && (field[4] != 1))
        bigrules[4].push_back(&(*itr));
      else {
        bigrules[min].push_back(&(*itr));
      }
    } else if (wild == 3) {
      if ((field[0] < IPbin) && (field[1] < IPbin)) {
        kindabigrules[9].push_back(&(*itr));	// wc except 0 and 1
      } else if ((field[0] < IPbin) && (field[2] < bin)){
        kindabigrules[8].push_back(&(*itr));	// wc except 0 and 2
      } else if ((field[0] < IPbin) && (field[3] < bin)) {
        kindabigrules[7].push_back(&(*itr));	// wc except 0 and 3
      } else if ((field[0] < IPbin) && (field[4] < bin)) {
        kindabigrules[6].push_back(&(*itr));	// wc except 0 and 4
      } else if ((field[1] < IPbin) && (field[2] < bin)) {
        kindabigrules[5].push_back(&(*itr));	// wc except 1 and 2
      } else if ((field[1] < IPbin) && (field[3] < bin)) {
        kindabigrules[4].push_back(&(*itr));	// wc except 1 and 3
      } else if ((field[1] < IPbin) && (field[4] < bin)) {
        kindabigrules[3].push_back(&(*itr));	// wc except 1 and 4
      } else if ((field[2] < bin) && (field[3] < bin)) {
        kindabigrules[2].push_back(&(*itr));	// wc except 2 and 3
      } else if ((field[2] < bin) && (field[4] < bin)) {
        kindabigrules[1].push_back(&(*itr));	// wc except 2 and 4
      } else if ((field[3] < bin) && (field[4] < bin)) {
        kindabigrules[0].push_back(&(*itr));	// wc except 3 and 4
      } else {
        printf("ERROR: Rule had 3 wc but did not match any of the bins!\n");
      }
    } else if (wild == 2) {
      if ((field[0] < IPbin) && (field[1] < IPbin) && (field[2] < bin)) {
        mediumrules[9].push_back(&(*itr));	// wc except 0, 1 and 2
      } else if ((field[0] < IPbin) && (field[1] < IPbin) && (field[3] < bin)){
        mediumrules[8].push_back(&(*itr));	// wc except 0, 1 and 3
      } else if ((field[0] < IPbin) && (field[1] < IPbin) && (field[4] < bin)) {
        mediumrules[7].push_back(&(*itr));	// wc except 0, 1 and 4
      } else if ((field[0] < IPbin) && (field[2] < bin) && (field[3] < bin)) {
        mediumrules[6].push_back(&(*itr));	// wc except 0, 2 and 3
      } else if ((field[0] < IPbin) && (field[2] < bin) && (field[4] < bin)) {
        mediumrules[5].push_back(&(*itr));	// wc except 0, 2 and 4
      } else if ((field[0] < IPbin) && (field[3] < bin) && (field[4] < bin)) {
        mediumrules[4].push_back(&(*itr));	// wc except 0, 3 and 4
      } else if ((field[1] < IPbin) && (field[2] < bin) && (field[3] < bin)) {
        mediumrules[3].push_back(&(*itr));	// wc except 1, 2 and 3
      } else if ((field[1] < IPbin) && (field[2] < bin) && (field[4] < bin)) {
        mediumrules[2].push_back(&(*itr));	// wc except 1, 2 and 4
      } else if ((field[1] < IPbin) && (field[3] < bin) && (field[4] < bin)) {
        mediumrules[1].push_back(&(*itr));	// wc except 1, 3 and 4
      } else if ((field[2] < bin) && (field[3] < bin) && (field[4] < bin)) {
        mediumrules[0].push_back(&(*itr));	// wc except 2, 3 and 4
      } else {
        printf("ERROR: Rule had 2 wc but did not match any of the bins!: %lf, %lf, %lf, %lf, %lf\n",field[0],field[1],field[2],field[3],field[4]);
      }
    } else {
      if (thirtyone) {
				if (wild == 1) {
					if (field[0] >= IPbin) {
						littlerules[0].push_back(&(*itr));
						printf("littlerules[0]\n");
					} else if (field[1] >= IPbin){
						printf("littlerules[1]\n");
						littlerules[1].push_back(&(*itr));
					} else if (field[2] >= IPbin) {
						littlerules[2].push_back(&(*itr));
						//printf("littlerules[1]\n");
					} else if (field[3] >= IPbin) {
						printf("littlerules[3]\n");
						littlerules[3].push_back(&(*itr));
					} else if (field[4] >= IPbin) {
						printf("littlerules[4]\n");
						littlerules[4].push_back(&(*itr));
					} else {
						printf("ERROR: Rule had 1 wc but did not match any of the bins!\n");
					}
				} else {
					smallrules.push_back(&(*itr));
				}
			} else {
				smallrules.push_back(&(*itr));
			}
    }
  }
  numTrees = 0;

  for (int i = 0; i < 5; i++) {
    if (bigrules[i].size() > 0) {
      numTrees++;
      rulelists[i] = 1;
    } else {
      rulelists[i] = 0;
    }
  }
  for (int j = 0; j < 10; j++) {
    if (kindabigrules[j].size() > 0) {
      numTrees++;
      rulelists[(j+5)] = 1;
    } else {
      rulelists[(j+5)] = 0;
    }
  }
  for (int k = 0; k < 10; k++) {
    if (mediumrules[k].size() > 0) {
      numTrees++;
      rulelists[k+15] = 1;
    } else {
      rulelists[k+15] = 0;
    }
  }
	for (int l = 0; l < 5; l++) {
		if (littlerules[l].size() > 0) {
			numTrees++;
		}
	}
  if (smallrules.size() > 0) {
    numTrees++;
    rulelists[25] = 1;
  } else {
    rulelists[25] = 0;
  }
}

/*
 *  Method to merge trees together
 *  Will try to merge trees that have no more than one field that is not overlapping (i.e. where one tree is WC and one tree is not)
 */
void cuts::MergeTrees() {
  printf("Number of trees before merge: %d\n",numTrees);
  int merged[26]; // array - if the value is 0 than that try is not merged, if it is 1 it has been and is NOT a candidate for merging anymore!
  for (int i = 0; i < 26; i++) { merged[i] = 0; } // make sure array is initialized to 0

  // try to merge any of the 1* into a 2* if it exists
  if (rulelists[0] == 1) {
    if (rulelists[11] == 1 && !(merged[11])) {
      bigrules[0].merge(kindabigrules[6],mycomparison);
      rulelists[11] = 0;
      merged[0] = 1;
      numTrees--;
    } else if (rulelists[12] == 1 && !(merged[12])) {
      bigrules[0].merge(kindabigrules[7],mycomparison);
      rulelists[12] = 0;
      merged[0] = 1;
       numTrees--;
    } else if (rulelists[13] == 1 && !(merged[13])) {
      bigrules[0].merge(kindabigrules[8],mycomparison);
      rulelists[13] = 0;
      merged[0] = 1;
      numTrees--;
    } else if (rulelists[14] == 1 && !(merged[14])) {
      bigrules[0].merge(kindabigrules[9],mycomparison);
      rulelists[14] = 0;
      merged[0] = 1;
      numTrees--;
    }
  }

  if (rulelists[1] == 1) {
     if (rulelists[8] == 1 && !(merged[8])) {
      bigrules[1].merge(kindabigrules[3],mycomparison);
      rulelists[8] = 0;
      merged[1] = 1;
      numTrees--;
    } else if (rulelists[9] == 1 && !(merged[9])) {
      bigrules[1].merge(kindabigrules[4],mycomparison);
      rulelists[9] = 0;
      merged[1] = 1;
      numTrees--;
    } else if (rulelists[10] == 1 && !(merged[10])) {
      bigrules[1].merge(kindabigrules[5],mycomparison);
      rulelists[10] = 0;
      merged[1] = 1;
      numTrees--;
    } else if (rulelists[14] == 1 && !(merged[14])) {
      bigrules[1].merge(kindabigrules[9],mycomparison);
      rulelists[14] = 0;
      merged[1] = 1;
      numTrees--;
    }
  }
   if (rulelists[2] == 1) {
     if (rulelists[6] == 1 && !(merged[6])) {
      bigrules[2].merge(kindabigrules[1],mycomparison);
      rulelists[6] = 0;
      merged[2] = 1;
      numTrees--;
    } else if (rulelists[7] == 1 && !(merged[7])) {
      bigrules[2].merge(kindabigrules[2],mycomparison);
      rulelists[7] = 0;
      merged[2] = 1;
      numTrees--;
    } else if (rulelists[10] == 1 && !(merged[10])) {
      bigrules[2].merge(kindabigrules[5],mycomparison);
      rulelists[10] = 0;
      merged[2] = 1;
      numTrees--;
    } else if (rulelists[13] == 1 && !(merged[13])) {
      bigrules[2].merge(kindabigrules[8],mycomparison);
      rulelists[13] = 0;
      merged[2] = 1;
      numTrees--;
    }
  }
  if (rulelists[3] == 1) {
     if (rulelists[5] == 1 && !(merged[5])) {
      bigrules[3].merge(kindabigrules[0],mycomparison);
      rulelists[5] = 0;
      merged[3] = 1;
      numTrees--;
    } else if (rulelists[7] == 1 && !(merged[7])) {
      bigrules[3].merge(kindabigrules[2],mycomparison);
      rulelists[7] = 0;
      merged[3] = 1;
      numTrees--;
    } else if (rulelists[9] == 1 && !(merged[9])) {
      bigrules[3].merge(kindabigrules[4],mycomparison);
      rulelists[9] = 0;
      merged[3] = 1;
      numTrees--;
    } else if (rulelists[12] == 1 && !(merged[12])) {
      bigrules[3].merge(kindabigrules[7],mycomparison);
      rulelists[12] = 0;
      merged[3] = 1;
      numTrees--;
    }
  }
  if (rulelists[4] == 1) {
    if (rulelists[5] == 1 && !(merged[5])) {
      bigrules[4].merge(kindabigrules[0],mycomparison);
      rulelists[5] = 0;
      merged[4] = 1;
      numTrees--;
    } else if (rulelists[6] == 1 && !(merged[6])) {
      bigrules[4].merge(kindabigrules[1],mycomparison);
      rulelists[6] = 0;
      merged[4] = 1;
      numTrees--;
    } else if (rulelists[8] == 1 && !(merged[8])) {
      bigrules[4].merge(kindabigrules[3],mycomparison);
      rulelists[8] = 0;
      merged[4] = 1;
      numTrees--;
    } else if (rulelists[11] == 1 && !(merged[11])) {
      bigrules[4].merge(kindabigrules[6],mycomparison);
      rulelists[11] = 0;
      merged[4] = 1;
      numTrees--;
    }
  }
  if (rulelists[5] == 1) {
    if (rulelists[15] == 1 && !(merged[15])) {
      kindabigrules[0].merge(mediumrules[0],mycomparison);
      rulelists[15] = 0;
      merged[5] = 1;
      numTrees--;
    } else if (rulelists[16] == 1 && !(merged[16])) {
      kindabigrules[0].merge(mediumrules[1],mycomparison);
      rulelists[16] = 0;
      merged[5] = 1;
      numTrees--;
    } else if (rulelists[19] == 1 && !(merged[19])) {
      kindabigrules[0].merge(mediumrules[4],mycomparison);
      rulelists[19] = 0;
      merged[5] = 1;
      numTrees--;
    }
  }
  if (rulelists[6] == 1) {
    if (rulelists[15] == 1 && !(merged[15])) {
      kindabigrules[1].merge(mediumrules[0],mycomparison);
      rulelists[15] = 0;
      merged[6] = 1;
      numTrees--;
    } else if (rulelists[17] == 1 && !(merged[17])) {
      kindabigrules[1].merge(mediumrules[2],mycomparison);
      rulelists[17] = 0;
      merged[6] = 1;
      numTrees--;
    } else if (rulelists[20] == 1 && !(merged[20])) {
      kindabigrules[1].merge(mediumrules[5],mycomparison);
      rulelists[20] = 0;
      merged[6] = 1;
      numTrees--;
    }
  }
 if (rulelists[7] == 1) {
    if (rulelists[15] == 1 && !(merged[15])) {
      kindabigrules[2].merge(mediumrules[0],mycomparison);
      rulelists[15] = 0;
      merged[7] = 1;
      numTrees--;
    } else if (rulelists[18] == 1 && !(merged[18])) {
      kindabigrules[2].merge(mediumrules[3],mycomparison);
      rulelists[18] = 0;
      merged[7] = 1;
      numTrees--;
    } else if (rulelists[21] == 1 && !(merged[21])) {
      kindabigrules[2].merge(mediumrules[6],mycomparison);
      rulelists[21] = 0;
      merged[7] = 1;
      numTrees--;
    }
  }
 if (rulelists[8] == 1) {
    if (rulelists[16] == 1 && !(merged[16])) {
      kindabigrules[3].merge(mediumrules[1],mycomparison);
      rulelists[16] = 0;
      merged[8] = 1;
      numTrees--;
    } else if (rulelists[17] == 1 && !(merged[17])) {
      kindabigrules[3].merge(mediumrules[2],mycomparison);
      rulelists[17] = 0;
      merged[8] = 1;
      numTrees--;
    } else if (rulelists[22] == 1 && !(merged[22])) {
      kindabigrules[3].merge(mediumrules[7],mycomparison);
      rulelists[22] = 0;
      merged[8] = 1;
      numTrees--;
    }
  }
 if (rulelists[9] == 1) {
    if (rulelists[16] == 1 && !(merged[16])) {
      kindabigrules[4].merge(mediumrules[1],mycomparison);
      rulelists[16] = 0;
      merged[9] = 1;
      numTrees--;
    } else if (rulelists[18] == 1 && !(merged[18])) {
      kindabigrules[4].merge(mediumrules[3],mycomparison);
      rulelists[18] = 0;
      merged[9] = 1;
      numTrees--;
    } else if (rulelists[23] == 1 && !(merged[23])) {
      kindabigrules[4].merge(mediumrules[8],mycomparison);
      rulelists[23] = 0;
      merged[9] = 1;
      numTrees--;
    }
  }
 if (rulelists[10] == 1) {
    if (rulelists[17] == 1 && !(merged[17])) {
      kindabigrules[5].merge(mediumrules[2],mycomparison);
      rulelists[17] = 0;
      merged[10] = 1;
      numTrees--;
    } else if (rulelists[18] == 1 && !(merged[18])) {
      kindabigrules[5].merge(mediumrules[3],mycomparison);
      rulelists[18] = 0;
      merged[10] = 1;
      numTrees--;
    } else if (rulelists[24] == 1 && !(merged[24])) {
      kindabigrules[5].merge(mediumrules[9],mycomparison);
      rulelists[24] = 0;
      merged[10] = 1;
      numTrees--;
    }
  }
 if (rulelists[11] == 1) {
    if (rulelists[19] == 1 && !(merged[19])) {
      kindabigrules[6].merge(mediumrules[4],mycomparison);
      rulelists[19] = 0;
      merged[11] = 1;
      numTrees--;
    } else if (rulelists[20] == 1 && !(merged[20])) {
      kindabigrules[6].merge(mediumrules[5],mycomparison);
      rulelists[20] = 0;
      merged[11] = 1;
      numTrees--;
    } else if (rulelists[22] == 1 && !(merged[22])) {
      kindabigrules[6].merge(mediumrules[7],mycomparison);
      rulelists[22] = 0;
      merged[11] = 1;
      numTrees--;
    }
  }
 if (rulelists[12] == 1) {
    if (rulelists[19] == 1 && !(merged[19])) {
      kindabigrules[7].merge(mediumrules[4],mycomparison);
      rulelists[19] = 0;
      merged[12] = 1;
      numTrees--;
    } else if (rulelists[21] == 1 && !(merged[21])) {
      kindabigrules[7].merge(mediumrules[6],mycomparison);
      rulelists[21] = 0;
      merged[12] = 1;
      numTrees--;
    } else if (rulelists[23] == 1 && !(merged[23])) {
      kindabigrules[7].merge(mediumrules[8],mycomparison);
      rulelists[23] = 0;
      merged[12] = 1;
      numTrees--;
    }
  }
  if (rulelists[13] == 1) {
    if (rulelists[20] == 1 && !(merged[20])) {
      kindabigrules[8].merge(mediumrules[5],mycomparison);
      rulelists[20] = 0;
      merged[13] = 1;
      numTrees--;
    } else if (rulelists[21] == 1 && !(merged[21])) {
      kindabigrules[8].merge(mediumrules[6],mycomparison);
      rulelists[21] = 0;
      merged[13] = 1;
      numTrees--;
    } else if (rulelists[24] == 1 && !(merged[24])) {
      kindabigrules[8].merge(mediumrules[9],mycomparison);
      rulelists[24] = 0;
      merged[13] = 1;
      numTrees--;
    }
  }
  if (rulelists[14] == 1) {
    if (rulelists[22] == 1 && !(merged[22])) {
      kindabigrules[9].merge(mediumrules[7],mycomparison);
      rulelists[22] = 0;
      merged[14] = 1;
      numTrees--;
    } else if (rulelists[23] == 1 && !(merged[23])) {
      kindabigrules[9].merge(mediumrules[8],mycomparison);
      rulelists[23] = 0;
      merged[14] = 1;
      numTrees--;
    } else if (rulelists[24] == 1 && !(merged[24])) {
      kindabigrules[9].merge(mediumrules[9],mycomparison);
      rulelists[24] = 0;
      merged[14] = 1;
      numTrees--;
    }
  }
#if 0
  for (int i = 0; i < 9; i++) {
    if (rulelists[i+15] == 1 && rulelists[25] == 1) {
      mediumrules[i].merge(smallrules,mycomparison);
      merged[i+15] = 1;
      rulelists[25] = 0;
      numTrees--;
      break;
    }
  }
#endif
  printf("Number of trees after merge: %d\n",numTrees);
}


void cuts::BinPack(int bins,list <TreeStat*> Statistics)
{
  list <MemBin*> Memory;
  for (int i = 0;i < bins;i++)
  {
    MemBin* newbin = new MemBin;
    newbin->Max_Depth = 0;
		newbin->Max_Levels = 0;
    newbin->total_memory = 0;
    newbin->Trees.clear();
    Memory.push_back(newbin);
  }
  while (!Statistics.empty())
  {
    Statistics.sort(mystatsort);
    TreeStat* selected_tree = Statistics.front();
    // printf("tree %d allocated!\n",selected_tree->Id);

    Memory.sort(mymemsort);
    (Memory.front())->Trees.push_back(selected_tree);
    (Memory.front())->Max_Depth     += selected_tree->Max_Depth;
		(Memory.front())->Max_Levels	  += selected_tree->Max_Levels;
    (Memory.front())->total_memory  += selected_tree->total_memory;
    Statistics.pop_front();
  }


  printf("******************************************\n");
  printf("Memory Channels = %d\n",bins);
  printf("******************************************\n");
  int count = 0;
  int ADJUSTED_OVERALL_DEPTH = 0;
	int ADJUSTED_OVERALL_LEVELS = 0;
  for (list <MemBin*>::iterator iter = Memory.begin();
        iter != Memory.end();++iter)
  {
    if ((*iter)->Max_Depth > ADJUSTED_OVERALL_DEPTH)
      ADJUSTED_OVERALL_DEPTH = (*iter)->Max_Depth;

    if ((*iter)->Max_Levels > ADJUSTED_OVERALL_LEVELS)
      ADJUSTED_OVERALL_LEVELS = (*iter)->Max_Levels;
    printf("Channel %d: Depth = %d Levels = %d Memory = %llu;Trees - ",count++,
              (*iter)->Max_Depth,(*iter)->Max_Levels,(*iter)->total_memory);
    for (list<TreeStat*>::iterator sub_iter = (*iter)->Trees.begin();
            sub_iter != (*iter)->Trees.end();sub_iter++)
    {
      printf("{%d} ",(*sub_iter)->Id);
    }
    printf("\n");
  }
  printf("ADJUSTED_OVERALL_DEPTH: %d\n",ADJUSTED_OVERALL_DEPTH);
	printf("ADJUSTED_OVERALL_LEVELS: %d\n",ADJUSTED_OVERALL_LEVELS);


}

int cuts::ComputeCutoffs()
{
  if (binningON == 0)
  {
    Cutoffs[0] = numrules;
    Num_Junk = 1;
    return 0;
  }
  for (int i = 0;i < NUM_JUNK;i++)
  {
    Cutoffs[i] = numrules * Percents[i] / 100;
    printf("Cutoffs[%d] = %lld\n",i,Cutoffs[i]);
  }
  Num_Junk = NUM_JUNK;
  return 0;
}


cuts::cuts(){

}

cuts::~cuts(){

}

void cuts::LoadRulePtr(list<Rule> ruleset,list <Rule*> ruleptr_list,int start,int end)
{
  printf("Rule:%d - %d\n",start,end);
  int count = 0;
  for (auto i : ruleset)
  {
    if (count >= start && count <= end)
      ruleptr_list.push_back(&i);
    count++;
  }
}

int
cuts::loadrule(const vector<Rule>& ruleset) {
  int i = 0;
  int wild = 0;

  for (auto r: ruleset){
	  wild = 0;

// std::cout << r.prefix_length[4] << std::endl;
    if(r.prefix_length[4] == 32){
    	r.prefix_length[4] = 0xFF;
//      rule.field[4].low = proto; // already set in InputReader::ReadFilter
//      rule.field[4].high = proto; // already set in InputReader::ReadFilter
    }else if(r.prefix_length[4] == 24){
    	r.prefix_length[4] = 0;
    	r.range[4][LowDim]= 0;
    	r.range[4][HighDim] = 0xFF;
      // std::cout << r.prefix_length[4] << std::endl;
    	wild++;
    }else{
      printf("Protocol mask error\n");
      return 0;
    }

    r.priority=i; // InputReader::ReadFilter already sets a priority but in the decreasing order. Changing it back to increasing order here.

    if ((r.range[2][LowDim] == 0) && (r.range[2][HighDim] == 65535)) {
      wild++;
    }
    if ((r.range[3][LowDim] == 0) && (r.range[3][HighDim] == 65535)) {
      wild++;
    }
    if (wild != 5) {
  	  classifier.push_back(r);
    }
    i++;
  }
  return i;
}

int
cuts::CreateClassifier(const vector <Rule>& ruleset){

	time_point<steady_clock> start, end;
	duration<double,std::milli> elapsed_milliseconds;
	start = steady_clock::now();

	classifier.clear();
	numrules = ruleset.size();

	printf("number of rules read from file = %d\n", numrules);

	int numrules1 = loadrule(ruleset);

	if (numrules1 != numrules){
		printf("Error: Number of rules read != Number of rules loaded!\n");
		exit(1);
	}

	ComputeCutoffs();

	if (binningON == 1){
	    binRules();
	    if (mergingON == 1)
	      MergeTrees();

	    for (int i = 0; i < 5; i++) {
	      if (!(bigrules[i].empty())) {
	        InitStats(bigrules[i].size());
	        create_tree(bigrules[i]);
	        RecordTreeStats();
	        bigrules[i].clear();
	      }
	    }
	    for (int j = 0; j < 10; j++) {
	      if (!(kindabigrules[j].empty())) {
	        InitStats(kindabigrules[j].size());
	        create_tree(kindabigrules[j]);
	        RecordTreeStats();
	        kindabigrules[j].clear();
	      }
	    }
	    for (int k = 0; k < 10; k++) {
	      if (!(mediumrules[k].empty())) {
	        InitStats(mediumrules[k].size());
	        create_tree(mediumrules[k]);
	        RecordTreeStats();
	        mediumrules[k].clear();
	      }
	    }
	    for (int l = 0; l < 5; l++) {
	      if (!(littlerules[l].empty())) {
	        InitStats(littlerules[l].size());
	        create_tree(littlerules[l]);
	        RecordTreeStats();
	        mediumrules[l].clear();
	      }
	    }
	    if (!(smallrules.empty())) {
	      InitStats(smallrules.size());
	      create_tree(smallrules);
	      RecordTreeStats();
	      smallrules.clear();
	    }
	  /*else
	  {
	    LoadRulePtr(classifier,p_classifier);
	    InitStats(p_classifier.size());
	    create_tree(p_classifier);
	    RecordTreeStats();
	    p_classifier.clear();
	  }*/
	}
	else{
	    int start = 0;
	    int end = 0;
	    for (int i = 0;i < Num_Junk;i++)
	    {
	      if (i == Num_Junk - 1)
	        end = classifier.size() - 1;
	      else
	        end = start + Cutoffs[i] - 1;
	      LoadRulePtr(classifier,p_classifier,start,end);
	      start = end + 1;
	      InitStats(p_classifier.size());
	      create_tree(p_classifier);
	      RecordTreeStats();
	      p_classifier.clear();
	    }
	}

	PrintStats();

	end = steady_clock::now();

	elapsed_milliseconds = end - start;

	this->initDelay = elapsed_milliseconds.count();

	return 0;
}

unsigned int
cuts::InsertRule(const Rule& r){

	return 0;
}

unsigned int
cuts::DeleteRule(const Rule& r){

	return 0;
}

unsigned int
cuts::AccessRule(const Packet& p){

	return 0;
}

unsigned int
cuts::GetNumRules() const{

	return 0;
}

unsigned int
cuts::GetMemorysize() const{

	return 0;
}

double
cuts::GetInitDelay() const{
	return this->initDelay;
}


}

