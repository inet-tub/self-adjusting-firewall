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

/*-----------------------------------------------------------------------------
 *
 *  Name:		trie.c for CutSplit
 *  Description:	trie construction for CutSplit: Pre-Cutting + Post-Splitting
 *  Version:		1.0 (release)
 *  Author:		Wenjun Li(Peng Cheng Laboratory, Email:wenjunli@pku.edu.cn)
 *  Date:		15/3/2019
 *
 *-----------------------------------------------------------------------------*/

#include "trie.h"
#include <math.h>

using namespace std;
namespace simulator{
trie::trie(int numrules1, int binth1, vector<Rule*> &rule1, vector<Rule*> &rule2, int threshold1, int k1)
{
   numrules = numrules1;
   binth = binth1;
   rule.clear();
   rule = rule1;
   k=k1;      //0:SA, 1:DA

   subset_node.clear();

   speedUpFlag=k1;//3:sa&da,1:sa, 2:da, others:for !5-tuple rules
   for (int i=0; i<MAXDIMENSIONS; i++){
       dim[i] = 0;
       if((k & (1 << i)) > 0) dim[i] = 1;
   }
   threshold = pow(2,threshold1);
   nodeSet = new nodeItem[MAXNODES+1];
   root = 1;

   pass=1;
   Total_Rule_Size=0;
   Total_Array_Size=0;
   Leaf_Node_Count=0;
   NonLeaf_Node_Count=0;
   total_ficuts_memory_in_KB=0;
   total_hs_memory_in_KB=0;
   total_memory_in_KB=0;


   for(int i=1; i<=MAXNODES; i++)
      nodeSet[i].child = (int*)malloc(sizeof(int));


   nodeSet[root].isleaf = 0;
   nodeSet[root].nrules = numrules;

   for (int i=0; i<MAXDIMENSIONS; i++){
        nodeSet[root].field[i].low = 0;
        if(i<2)
           nodeSet[root].field[i].high = 0xffffffff;
        else if(i==4)
           nodeSet[root].field[i].high = 255;
        else
           nodeSet[root].field[i].high = 65535;

       nodeSet[root].ncuts[i] = 0;
   }

  nodeSet[root].ruleid = (int*)calloc(numrules, sizeof(int));
  for(int i=0; i<numrules; i++){
      nodeSet[root].ruleid[i] = rule2[i]->id;
  }


  nodeSet[root].layNo = 1;
  nodeSet[root].flag = 1;

  freelist = 2;
  for (int i = 2; i < MAXNODES; i++)
      nodeSet[i].child[0] = i+1;
  nodeSet[MAXNODES].child[0] = Null;

  createtrie();

}

trie::~trie() {
  delete [] nodeSet;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  count_np_ficut
 *  Description:  count np_ficut in pre-cutting stage (using FiCut[from HybridCuts])
 * =====================================================================================
 */
void trie::count_np_ficut1(nodeItem *node){
	int done=0;

	for(int i=0;i<MAXDIMENSIONS;i++){
		node->ncuts[i] = 1;
		if(dim[i]){
			done = 0;
			while(!done){
				if(node->ncuts[i] < MAXCUTS1 && (node->field[i].high - node->field[i].low) > threshold)
					node->ncuts[i]=node->ncuts[i]*2;
				else
					done=1;
			}
		}
	}
}

void trie::count_np_ficut2(nodeItem *node){
	int done=0;
	for(int i=0;i<MAXDIMENSIONS;i++){
		node->ncuts[i] = 1;
		if(dim[i]){
			done = 0;
			while(!done){
				if(node->ncuts[i] < MAXCUTS2 && (node->field[i].high - node->field[i].low) > threshold)
					node->ncuts[i]=node->ncuts[i]*2;
				else
					done=1;
			}
		}
	}
}



void trie::createtrie()
{
    int nr=0;
    int empty=0;
    unsigned int r[MAXDIMENSIONS], lo[MAXDIMENSIONS], hi[MAXDIMENSIONS];
    int u,v=0;
    int s = 0;
    int i[MAXDIMENSIONS];
    int flag,index=0;

    qNode.push(root);

    while(!qNode.empty()){
        v=qNode.front();
        qNode.pop();


		if(nodeSet[v].flag==1){

			if(k==3){
				count_np_ficut2(&nodeSet[v]);
				for(int t=0;t<MAXDIMENSIONS;t++){
					if(dim[t]){
						if(nodeSet[v].ncuts[t] < MAXCUTS2)
							nodeSet[v].flag=2;
					}
				}
			} else{
				count_np_ficut1(&nodeSet[v]);
				for(int t=0;t<MAXDIMENSIONS;t++){
					if(dim[t]){
						if(nodeSet[v].ncuts[t] < MAXCUTS1)
							nodeSet[v].flag=2;
					}
				}


			}
		}




        if(nodeSet[v].flag==1) //FiCuts stage
        {
            if(nodeSet[v].nrules <= binth){ //leaf nodes
                nodeSet[v].isleaf = 1;
                Total_Rule_Size+= nodeSet[v].nrules;
                Leaf_Node_Count++;

            }
            else{  //non-leaf nodes
                NonLeaf_Node_Count++;
                nodeSet[v].child = (int *)realloc(nodeSet[v].child, nodeSet[v].ncuts[0] * nodeSet[v].ncuts[1] * sizeof(int));

                Total_Array_Size += (nodeSet[v].ncuts[0] * nodeSet[v].ncuts[1]);

                index = 0;

                r[0] = (nodeSet[v].field[0].high - nodeSet[v].field[0].low)/nodeSet[v].ncuts[0];
                lo[0] = nodeSet[v].field[0].low;
                hi[0] = lo[0] + r[0];
                for(i[0] = 0; i[0] < nodeSet[v].ncuts[0]; i[0]++){  //sip

                    r[1] = (nodeSet[v].field[1].high - nodeSet[v].field[1].low)/nodeSet[v].ncuts[1];
                    lo[1] = nodeSet[v].field[1].low;
                    hi[1] = lo[1] + r[1];
                    for(i[1] = 0; i[1] < nodeSet[v].ncuts[1]; i[1]++){ //dip

                        r[2] = (nodeSet[v].field[2].high - nodeSet[v].field[2].low)/nodeSet[v].ncuts[2];
                        lo[2] = nodeSet[v].field[2].low;
                        hi[2] = lo[2] + r[2];
                        for(i[2] = 0; i[2] < nodeSet[v].ncuts[2]; i[2]++){

                            r[3] = (nodeSet[v].field[3].high - nodeSet[v].field[3].low)/nodeSet[v].ncuts[3];
                            lo[3] = nodeSet[v].field[3].low;
                            hi[3] = lo[3] + r[3];
                            for(i[3] = 0; i[3] < nodeSet[v].ncuts[3]; i[3]++){

                                r[4] = (nodeSet[v].field[4].high - nodeSet[v].field[4].low)/nodeSet[v].ncuts[4];
                                lo[4] = nodeSet[v].field[4].low;
                                hi[4] = lo[4] + r[4];
                                for(i[4] = 0; i[4] < nodeSet[v].ncuts[4]; i[4]++){

                                    empty = 1;
                                    nr = 0;
                                    for(int j=0; j<nodeSet[v].nrules; j++){
                                        flag = 1;
                                        for(int t = 0; t < MAXDIMENSIONS; t++){
//                                        	printf("\n%d %d %d %d\n", v, j , nodeSet[v].ruleid[j],nodeSet[v].nrules);
                                        	if(rule[nodeSet[v].ruleid[j]]->range[t][LowDim] > hi[t] || rule[nodeSet[v].ruleid[j]]->range[t][HighDim] < lo[t]){
                                                flag = 0;
                                                break;
                                            }
                                        }
                                        if(flag == 1){
                                            empty = 0;
                                            nr++;
                                        }
                                    }

                                    if(!empty){
                                        nodeSet[v].child[index] = freelist;
                                        u=freelist;
                                        freelist = nodeSet[freelist].child[0];
                                        nodeSet[u].nrules = nr;
                                        nodeSet[u].layNo=nodeSet[v].layNo+1;
                                        if(nr <= binth){
                                            nodeSet[u].isleaf = 1;
                                            Total_Rule_Size+= nr;
                                            Leaf_Node_Count++;


                                        }
                                        else{
                                            nodeSet[u].isleaf = 0;
                                            nodeSet[u].flag=1;  //cut

                                            if(pass<nodeSet[u].layNo)
                                                pass=nodeSet[u].layNo;

                                            qNode.push(u);
                                        }

                                        for (int t=0; t<MAXDIMENSIONS; t++){   //update node ranges
                                            if(dim[t] == 1){
                                                nodeSet[u].field[t].low = lo[t];
                                                nodeSet[u].field[t].high= hi[t];
                                            }else{
                                                nodeSet[u].field[t].low = nodeSet[v].field[t].low;
                                                nodeSet[u].field[t].high= nodeSet[v].field[t].high;
                                            }
                                        }

                                        s = 0;
                                        nodeSet[u].ruleid = (int *)calloc(nodeSet[u].nrules, sizeof(int));
                                        for(int j=0; j<nodeSet[v].nrules; j++){
                                            flag = 1;
                                            for(int t = 0; t < MAXDIMENSIONS; t++){
                                                if(dim[t] == 1){
                                                    if(rule[nodeSet[v].ruleid[j]]->range[t][LowDim] > hi[t] || rule[nodeSet[v].ruleid[j]]->range[t][HighDim] < lo[t]){
                                                        flag = 0;
                                                        break;
                                                    }
                                                }
                                            }
                                            if(flag == 1){
                                                nodeSet[u].ruleid[s] = nodeSet[v].ruleid[j];
                                                s++;
                                            }
                                        }
                                    }
                                    else //empty
                                        nodeSet[v].child[index] = Null;

                                    index ++;

                                    lo[4] = hi[4] + 1;
                                    hi[4] = lo[4] + r[4];
                                }
                                lo[3] = hi[3] + 1;
                                hi[3] = lo[3] + r[3];
                            }
                            lo[2] = hi[2] + 1;
                            hi[2] = lo[2] + r[2];
                        }
                        lo[1] = hi[1] + 1;
                        hi[1] = lo[1] + r[1];
                    }
                    lo[0] = hi[0] + 1;
                    hi[0] = lo[0] + r[0];
                }
            }
        } else{ //HyperSplit stage
           if(nodeSet[v].nrules <= binth){  //leaf node
               nodeSet[v].isleaf = 1;

               Total_Rule_Size+= nodeSet[v].nrules;
               Leaf_Node_Count++;

           }
           else{
               NonLeaf_Node_Count++;
//               subset_node = new Rule[nodeSet[v].nrules];
               for(int m=0;m<nodeSet[v].nrules;m++){
            	   Rule* temp = new Rule;
            	   subset_node.push_back(temp);
                   subset_node[m]->id = nodeSet[v].ruleid[m];
                   for(int k = 0; k < MAXDIMENSIONS; k++){
                       subset_node[m]->range[k][LowDim] = rule[nodeSet[v].ruleid[m]]->range[k][LowDim];
                       subset_node[m]->range[k][HighDim] = rule[nodeSet[v].ruleid[m]]->range[k][HighDim];
                   }
               }

//               nodeSet[v].rootnode = (hs_node_t *) malloc(sizeof(hs_node_t));
               nodeSet[v].rootnode = new hs_node_t;
//               hstrie Ttmp(nodeSet[v].nrules,subset_node, binth, nodeSet[v].rootnode);
               hstrie* Ttmp = new hstrie(nodeSet[v].nrules,subset_node, binth, nodeSet[v].rootnode);
//			   Ttmp.hstrie1(nodeSet[v].nrules,subset_node, binth, nodeSet[v].rootnode);

               total_hs_memory_in_KB += Ttmp->result.total_mem_kb;

               nodeSet[v].layNo += Ttmp->result.wst_depth;

           }
       }
   }

  total_ficuts_memory_in_KB=double(Total_Rule_Size*PTR_SIZE+Total_Array_Size*PTR_SIZE+LEAF_NODE_SIZE*Leaf_Node_Count+TREE_NODE_SIZE*NonLeaf_Node_Count)/1024;
  total_memory_in_KB = total_ficuts_memory_in_KB + total_hs_memory_in_KB;

  // if(numrules>binth){
  //     if(k==1)
  //         printf("\t***SA Subset Tree(using FiCuts + HyperSplit):***\n");
  //     if(k==2)
  //         printf("\t***DA Subset Tree(using FiCuts + HyperSplit):***\n");
  //     if(k==3)
  //         printf("\t***SA_DA Subset Tree(using FiCuts + HyperSplit):***\n");
  //     printf("\tRESULTS:");
  //     printf("\n\tnumber of rules:%d",numrules);
  //     printf("\n\tworst case tree level: %d",pass);
  //     printf("\n\ttotal memory(Pre-Cutting): %f(KB)",total_ficuts_memory_in_KB);
  //     printf("\n\ttotal memory(Post_Splitting): %f(KB)",total_hs_memory_in_KB);
  //     printf("\n\ttotal memory: %f(KB)",total_memory_in_KB);
  //     printf("\n\tByte/rule: %f",double(total_memory_in_KB*1024)/numrules);
  //     printf("\n\t***SUCCESS in building %d-th CutSplit sub-tree(1_sa, 2_da, 3_sa&da)***\n\n",k);
  // }

}

unsigned int
trie::get_nbits(unsigned int n)
{
    int		k = 0;

    while (n >>= 1)
        k++;
    return	k;
}
unsigned int
trie::get_pow(unsigned int n)
{
    int		k = 1;

    while (n--)
        k*=2;
    return	k;
}

int trie::trieLookup(const Packet& p){

    int index[MAXDIMENSIONS];
    int cchild, cover;
    int cuts[MAXDIMENSIONS];
    int cvalue[MAXDIMENSIONS];
    int cnode = root;
    int match = 0;
    int nbits[MAXDIMENSIONS];
    int i,j,t;
    int flag_hs = 0;
    int match_id = -1;
	unsigned int numbit = 32;





	switch(speedUpFlag) // speedUpFlag=3:sa&da,1:sa, 2:da, others:for !5-tuple rules
	{
		case 1:{
			while(nodeSet[cnode].isleaf != 1){  //find matched leaf nodes: cnode
				numbit -= MAXCUTBITS1;
				cchild = (p[0]>>numbit) & ANDBITS1;
				cnode = nodeSet[cnode].child[cchild];  //level++
				queryCount[0]++;

				if(cnode == Null) break;
				if(nodeSet[cnode].flag == 2 && nodeSet[cnode].isleaf != 1) {
					flag_hs = 1;
					break;
				}
			}
			break;
		}
		case 2:{
			while(nodeSet[cnode].isleaf != 1){  //find matched leaf nodes: cnode
				numbit -= MAXCUTBITS1;
				cchild = (p[1]>>numbit) & ANDBITS1;
				cnode = nodeSet[cnode].child[cchild];
				queryCount[0]++;

				if(cnode == Null) break;
				if(nodeSet[cnode].flag == 2 && nodeSet[cnode].isleaf != 1) {
					flag_hs = 1;
					break;
				}
			}
			break;

		}
		case 3:{
			while(nodeSet[cnode].isleaf != 1){   //find matched leaf nodes: cnode
				numbit -= MAXCUTBITS2;
				cchild = ((p[0]>>numbit) & ANDBITS2)*nodeSet[cnode].ncuts[1] + ((p[1]>>numbit) & ANDBITS2);
				cnode = nodeSet[cnode].child[cchild];
				queryCount[0]++;
				if(cnode == Null) break;
				if(nodeSet[cnode].flag == 2 && nodeSet[cnode].isleaf != 1) {
					flag_hs = 1;
					break;
				}
			}
			break;

		}
		default: break;

	}






    if(cnode != Null && flag_hs == 1){
        match_id = LookupHSTree(nodeSet[cnode].rootnode, p, queryCount);
    }
    if(cnode != Null && flag_hs == 0 && nodeSet[cnode].isleaf == 1){
        for(i = 0; i < nodeSet[cnode].nrules; i++){
            queryCount[0]++;
            cover = 1;
            for(j = 0; j < MAXDIMENSIONS; j++){
                if(rule[nodeSet[cnode].ruleid[i]]->range[j][LowDim] > p[j] || rule[nodeSet[cnode].ruleid[i]]->range[j][HighDim] < p[j]){
                    cover = 0;
                    break;
                }
            }
            if(cover == 1){
                match = 1;
                break;
            }
        }
    }
    if(match == 1) {
        match_id = nodeSet[cnode].ruleid[i];
    }
//    else{
//    	std::cout << "no match" << std::endl;
//    	std::cout << p[0]<< " " << p[1]<< " " << p[2]<< " " << p[3]<< " " << p[4]<< " " << std::endl;
//    }

    return  match_id;

}

} /* namepace simulator */
