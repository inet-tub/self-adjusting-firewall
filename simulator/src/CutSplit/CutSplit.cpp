/*
 * CutSplit.cpp
 *
 *  Created on: Aug 1, 2021
 *      Author: vamsi
 */

#include "CutSplit.h"

namespace simulator {

CutSplit::CutSplit() {

	mem_acc = new double;
	*mem_acc = 0;

	bucketSize = 8;   // leaf threashold
	threshold = 16;   // Assume T_SA=T_DA=threshold

	totalNodesTraversed=0; // May overflow
	totalAccess=0;

	numRules=0;
	totalMemoryBytes=0;

	total_query=0;
	bucketSize=0;
	threshold=0;

	number_rule=0;

	num_subset_4[0]=0;
	num_subset_4[1]=0;
	num_subset_4[2]=0;
	num_subset_4[3]=0;

	threshold_value[0]=0;
	threshold_value[1]=0;

	T_sa=NULL;
	T_sa_da=NULL;
	T_da=NULL;
	big_node=NULL;
	T=NULL;

	vector<Rule*>* temp = new vector<Rule*>;
	subset_4.push_back(*temp);
	temp = new vector<Rule*>;
	subset_4.push_back(*temp);
	temp = new vector<Rule*>;
	subset_4.push_back(*temp);
	temp = new vector<Rule*>;
	subset_4.push_back(*temp);
	subset_4[0].clear();
	subset_4[1].clear();
	subset_4[2].clear();
	subset_4[3].clear();
}

CutSplit::~CutSplit() {
	// TODO
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  count_length
 *  Description:  record length of field and correponding size
 * =====================================================================================
 */
void CutSplit::count_length1(int number_rule,vector<Rule*>& rule,field_length *field_length_ruleset)
{
	unsigned temp_size=0;
	unsigned temp_value=0;

	for(int i=0;i<number_rule;i++)
	{
		for(int j=0;j<5;j++)  //record field length in field_length_ruleset[i]
		{
			field_length_ruleset[i].length[j]=rule[i]->range[j][HighDim]-rule[i]->range[j][LowDim];
			//			printf("%lu %lu %lu\n",rule[i]->range[j][HighDim], rule[i]->range[j][LowDim],rule[i]->range[j][HighDim]-rule[i]->range[j][LowDim]);
			if(field_length_ruleset[i].length[j]==0xffffffff)
				field_length_ruleset[i].size[j]=32; //for address *
			else
			{
				temp_size=0;
				temp_value=field_length_ruleset[i].length[j]+1;
				while((temp_value=temp_value/2)!=0)
					temp_size++;
				if((field_length_ruleset[i].length[j]+1 - pow(2,temp_size))!=0)
					temp_size++;

				field_length_ruleset[i].size[j]=temp_size;
			}
		}
	}
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  partition_v3 (version3)
 *  Description:  partition ruleset into subsets based on address field(2 dim.)
 * =====================================================================================
 */
void
CutSplit::partition_V3(vector<Rule*>& rule, vector<vector<Rule*>>& subset,int number_rule,field_length *field_length_ruleset,int threshold_value[2])
{  // 0:sa,da; 1:sa; 2:da; 3:big
	int num_small_tmp[number_rule];

	for(int i=0;i<number_rule;i++){
		num_small_tmp[i]=0;
		for(int k=0;k<2;k++){
			//			printf("%d %d\n",field_length_ruleset[i].size[k],threshold_value[k]);
			if(field_length_ruleset[i].size[k] <= threshold_value[k])
				num_small_tmp[i]++;
		}
	}
	printf("number_rule %d\n",number_rule);

	int count_sa_da=0;
	int count_sa=0;
	int count_da=0;
	int count_big=0;
	for(int i=0;i<number_rule;i++){
		//		printf("num_small_tmp %d\n",num_small_tmp[i]);
		if(num_small_tmp[i]==0)
			subset[3][count_big++]=rule[i];
		else if(num_small_tmp[i]==2){
			subset[0][count_sa_da++]=rule[i];
//			std::cout << subset[0][count_sa_da-1]->id << std::endl;
		}
		else if(num_small_tmp[i]==1){
			if(field_length_ruleset[i].size[0]<=threshold_value[0]) //sip:small
				subset[1][count_sa++]=rule[i];
			else if(field_length_ruleset[i].size[1]<=threshold_value[1]) //dip:small
				subset[2][count_da++]=rule[i];
		}
	}

	num_subset_4[0]=count_sa_da;
	num_subset_4[1]=count_sa;
	num_subset_4[2]=count_da;
	num_subset_4[3]=count_big;

	// printf("\t#SA&DA_subset:%d  #SA_subset:%d  #DA_subset:%d  #Big_subset:%d\n\n",count_sa_da,count_sa,count_da,count_big);

}


int
CutSplit::CreateClassifier(CommandLine* cmd){

	string f = "/home/vamsi/src/phd/self-adjusting-firewall/classbench/db_generator/MyFilters10k";
	cmd->Get("ruleset", f);
	rules= InputReader::ReadFilterFile(f);

	numRules=rules.size();
	number_rule=numRules;

	subset_4.clear();
	vector<Rule*> *temp = new vector<Rule*>;
	subset_4.push_back(*temp);
	subset_4[0].clear();
	temp = new vector<Rule*>;
	subset_4.push_back(*temp);
	subset_4[1].clear();
	temp = new vector<Rule*>;
	subset_4.push_back(*temp);
	subset_4[2].clear();
	temp = new vector<Rule*>;
	subset_4.push_back(*temp);
	subset_4[3].clear();

	for (Rule& r: rules){
		rule.push_back(&r);
		//		r.Print();
		subset_4[0].push_back(NULL);
		subset_4[1].push_back(NULL);
		subset_4[2].push_back(NULL);
		subset_4[3].push_back(NULL);
	}

	bucketSize = 8;   // leaf threashold
	threshold = 16;   // Assume T_SA=T_DA=threshold

	// printf("\tBucket Size =  %d\n", bucketSize);
	// printf("\tThreshold (T_SA, T_DA) = <2^%d, 2^%d>\n", threshold,threshold);

	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<double> elapsed_seconds;
	std::chrono::duration<double,std::milli> elapsed_milliseconds;

	field_length field_length_ruleset[number_rule];
	count_length1(number_rule,rule,field_length_ruleset);

	num_subset_4[0]=0;num_subset_4[1]=0;num_subset_4[2]=0;num_subset_4[3]=0;
	threshold_value[0]=threshold;threshold_value[1]=threshold;   //T_SA=T_DA=threshold

	partition_V3(rule,subset_4,number_rule,field_length_ruleset,threshold_value);


	start = std::chrono::steady_clock::now();
	float hsMem = 0;
	//construct classifier
	T_sa_da = new trie(num_subset_4[0],bucketSize,rule,subset_4[0],threshold,3);
	T_sa = new trie(num_subset_4[1],bucketSize,rule,subset_4[1],threshold,1);
	T_da = new trie(num_subset_4[2],bucketSize,rule,subset_4[2],threshold,2);
	big_node = new hs_node_t ;
	if(num_subset_4[3] > 0){
		T = new hstrie(num_subset_4[3],subset_4[3], bucketSize, big_node);
		hsMem = T->result.total_mem_kb;
	}
	end = std::chrono::steady_clock::now();
	elapsed_milliseconds = end - start;

	// if(num_subset_4[3] > 0){
	// 	printf("\t***Big Subset Tree(using HyperSplit):***\n");
	// 	printf("\tRESULTS:");
	// 	printf("\n\tnumber of rules:%d",num_subset_4[3]);
	// 	printf("\n\tTotal memory consumption: %f(KB) \n", T->result.total_mem_kb);
	// }
	initDelay = elapsed_milliseconds.count();
	// printf("\n\tConstruction time: %f ms\n", elapsed_milliseconds.count());
	totalMemoryBytes=(T_sa_da->total_memory_in_KB + T_sa->total_memory_in_KB + T_da->total_memory_in_KB + hsMem)*1024;


	return 0;
}


unsigned int
CutSplit::InsertRule(const Rule& r){
	// TODO
	return 0;
}

unsigned int
CutSplit::DeleteRule(const Rule& r){

	// TODO
	return 0;
}


unsigned int
CutSplit::AccessRule(const Packet& p){

    int match_sa_da, match_sa, match_da, match_big, fid;
    int matchid;

	int match_miss = 0;
	int match_pri = -2;

    unsigned int proto_mask;



			matchid = match_sa_da = match_sa = match_da = match_big = -1;

			match_sa_da = T_sa_da->trieLookup(p); 
			mem_acc[0]+=T_sa_da->GetQueried(); T_sa_da->ClearQueried();
            match_sa = T_sa->trieLookup(p);
            mem_acc[0]+=T_sa->GetQueried(); T_sa->ClearQueried(); 
            match_da = T_da->trieLookup(p);
            mem_acc[0]+=T_da->GetQueried(); T_da->ClearQueried();
            if(num_subset_4[3] > 0)
            	match_big = LookupHSTree(big_node,p,mem_acc);


			if(match_sa_da != -1) matchid = match_sa_da;
            if((matchid == -1) || (match_sa != -1 && match_sa < matchid)) matchid = match_sa;
            if((matchid == -1) || (match_da != -1 && match_da < matchid)) matchid = match_da;
            if((matchid == -1) || (match_big != -1 && match_big < matchid)) matchid = match_big;

			if(matchid == -1){
//				std::cout << "Packet could not be classified!" << std::endl;
				match_miss++;

            }else if(matchid == p[6]){
                //printf("\nmatch_id = %d   fid = %d   -----   packet %d match rule %d\n", matchid, packets[i][5], i, matchid);
            }else if(matchid > p[6]){
//                printf("? packet matched a lower priority rule %d, should be %d\n", matchid, p[6]);
                match_miss++;
            }else{
                //printf("! packet %d match higher priority rule %d, should be %d\n", i, matchid, packets[i][5]);
			}

//        printf("\t%d packets are classified, %d of them are misclassified\n", number_pkt * trials, match_miss);
//		printf("\tTotal classification time: %f s\n", sum_time.count() / trials);
//		printf("\tAverage classification time: %f us\n", sum_time.count() * 1e6 / (trials * packets.size()));
//		printf("\tThroughput: %f Mpps\n", 1 / (sum_time.count() * 1e6 / (trials * packets.size())));

	totalAccess++;
	return match_miss;
}

unsigned int
CutSplit:: GetNumRules() const{
	return numRules;
}

uint64_t
CutSplit::GetMemorysize() const{

	return totalMemoryBytes;
}

double
CutSplit::GetInitDelay() const{
	return initDelay;
}





} /* namespace simulator */

