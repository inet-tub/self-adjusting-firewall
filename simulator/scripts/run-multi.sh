source source.sh

RESULTS=$(pwd)/results
OUTPUT=$(pwd)/output

if [ ! -d $RESULTS ];then
	mkdir $RESULTS
fi

if [ ! -d $OUTPUT ];then
	mkdir $OUTPUT
fi

FOLLOW=$FIREWALL/tools/classify
CUTSPLIT="/home/vamsi/src/phd/self-adjusting-firewall/CutLatest/main"

# ./main -b 8 -t 20 -r ./../classbench/db_generator/MyFilters10k -e ./../classbench/db_generator/MyFilters10k_trace -c 1 -u 0 -o ./output.dat
# ./follow --alg="follow" --output=./output.dat


# PARAM_FILES=(acl1_seed acl2_seed acl3_seed acl4_seed acl5_seed fw1_seed fw2_seed fw3_seed fw4_seed fw5_seed ipc1_seed ipc2_seed)
PARAM_FILES=(acl1_seed)
# RULES=(64 128 256 512 1024 2048 4096 8192 16384 32768 65536)
# RULES=(64 128 256 512 1024 2048 4096 8192)
# RULES=(1024 2048)
RULES=(1000 10000 100000 1000000)
# RULES=(64)

SMOOTHS=(2)
ADDRESS_SCOPES=(0.5)
APP_SCOPES=(-0.1)

Pas=(10)
Pbs=(10)
SCALES=(1000)

SCALE=1000
RUNS=32

echo "RESULT ALG RULESETSIZE SEED PA PB AvgCostPerReq AvgReqPerCost MaxCostPerReq MaxReqPerCost"
# std::cout << "AverageCostPerReq " << 1.0/avgReqPerCost << " AverageReqPerCost " << avgReqPerCost << " MaxAverageCostPerReq " << maxAvgTraversed << " MaxAverageReqPerCost " << 1.0/maxAvgTraversed  << std::endl;

EXP=$1

if [[ $EXP == "follow" || $EXP == "all" ]];then

	for NUM_RULES in ${RULES[@]};do

		for PARAM_FILE in ${PARAM_FILES[@]};do
		
			for SMOOTH in ${SMOOTHS[@]};do

				for ADDRESS_SCOPE in ${ADDRESS_SCOPES[@]};do

					for APP_SCOPE in ${APP_SCOPES[@]};do

						for Pa in ${Pas[@]};do

							for Pb in ${Pbs[@]};do

								ulimit -s 1638400

								i=0
								rm /tmp/output-runs > /dev/null 2> /dev/null
								rm /tmp/output-mems > /dev/null 2> /dev/null
								while [[ $i -lt $RUNS ]];do
									$SCRIPTS/genRule-Trace-single.sh $PARAM_FILE $NUM_RULES $SMOOTH $ADDRESS_SCOPE $APP_SCOPE $Pa $Pb $SCALE $i> /dev/null 2> /dev/null
								 	(taskset --cpu-list $i $FOLLOW %alg="follow" %ruleset=$SCRIPTS/rules/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$i.rules %trace=$SCRIPTS/traces/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$Pa-$Pb-$SCALE-$i.trace > /tmp/output_$i; cat /tmp/output_$i | grep 'AverageCostPerReq' >> /tmp/output-runs) &
									i=$(( $i+1  ))
								done
								while [[ $(ps aux | grep "tools/classify" | wc -l) -gt 1 ]];do
									echo "waiting..."
									sleep 1
								done
								# cat /tmp/output-runs
								avgcostpereq=$(python3 ./mean.py /tmp/output-runs | grep "mean" | awk '{print $2}')
								minavgcostpereq=$(python3 ./mean.py /tmp/output-runs | grep "min" | awk '{print $2}')
								maxavgcostpereq=$(python3 ./mean.py /tmp/output-runs | grep "max" | awk '{print $2}')

								avgreqpercost=$(python3 ./mean.py /tmp/output-runs | grep "mean" | awk '{print $4}')
								minavgreqpercost=$(python3 ./mean.py /tmp/output-runs | grep "min" | awk '{print $4}')
								maxavgreqpercost=$(python3 ./mean.py /tmp/output-runs | grep "max" | awk '{print $4}')

								maxcostpereq=$(python3 ./mean.py /tmp/output-runs | grep "mean" | awk '{print $6}')
								minmaxcostpereq=$(python3 ./mean.py /tmp/output-runs | grep "min" | awk '{print $6}')
								maxmaxcostpereq=$(python3 ./mean.py /tmp/output-runs | grep "max" | awk '{print $6}')

								maxreqpercost=$(python3 ./mean.py /tmp/output-runs | grep "mean" | awk '{print $8}')
								minreqpercost=$(python3 ./mean.py /tmp/output-runs | grep "min" | awk '{print $8}')
								maxreqpercost=$(python3 ./mean.py /tmp/output-runs | grep "max" | awk '{print $8}')

								echo "RESULT follow $NUM_RULES $PARAM_FILE $Pa $Pb $avgcostpereq $avgreqpercost $maxcostpereq $maxreqpercost"
							done
						done
					done
				done
			done
		done
	done
fi


if [[ $EXP == "followfast" || $EXP == "all" ]];then
	
	for NUM_RULES in ${RULES[@]};do

		for PARAM_FILE in ${PARAM_FILES[@]};do
		
			for SMOOTH in ${SMOOTHS[@]};do

				for ADDRESS_SCOPE in ${ADDRESS_SCOPES[@]};do

					for APP_SCOPE in ${APP_SCOPES[@]};do

						for Pa in ${Pas[@]};do

							for Pb in ${Pbs[@]};do

								ulimit -s 1638400

								i=0
								rm /tmp/output-runs > /dev/null 2> /dev/null
								rm /tmp/output-mems > /dev/null 2> /dev/null
								while [[ $i -lt $RUNS ]];do
									$SCRIPTS/genRule-Trace-single.sh $PARAM_FILE $NUM_RULES $SMOOTH $ADDRESS_SCOPE $APP_SCOPE $Pa $Pb $SCALE $i> /dev/null 2> /dev/null
								 	(taskset --cpu-list $i $FOLLOW %alg="followfast" %ruleset=$SCRIPTS/rules/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$i.rules %trace=$SCRIPTS/traces/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$Pa-$Pb-$SCALE-$i.trace > /tmp/output_$i; cat /tmp/output_$i | grep 'AverageCostPerReq' >> /tmp/output-runs) &
									i=$(( $i+1  ))
								done
								while [[ $(ps aux | grep "tools/classify" | wc -l) -gt 1 ]];do
									echo "waiting..."
									sleep 1
								done
								# cat /tmp/output-runs
								avgcostpereq=$(python3 ./mean.py /tmp/output-runs | grep "mean" | awk '{print $2}')
								minavgcostpereq=$(python3 ./mean.py /tmp/output-runs | grep "min" | awk '{print $2}')
								maxavgcostpereq=$(python3 ./mean.py /tmp/output-runs | grep "max" | awk '{print $2}')

								avgreqpercost=$(python3 ./mean.py /tmp/output-runs | grep "mean" | awk '{print $4}')
								minavgreqpercost=$(python3 ./mean.py /tmp/output-runs | grep "min" | awk '{print $4}')
								maxavgreqpercost=$(python3 ./mean.py /tmp/output-runs | grep "max" | awk '{print $4}')

								maxcostpereq=$(python3 ./mean.py /tmp/output-runs | grep "mean" | awk '{print $6}')
								minmaxcostpereq=$(python3 ./mean.py /tmp/output-runs | grep "min" | awk '{print $6}')
								maxmaxcostpereq=$(python3 ./mean.py /tmp/output-runs | grep "max" | awk '{print $6}')

								maxreqpercost=$(python3 ./mean.py /tmp/output-runs | grep "mean" | awk '{print $8}')
								minreqpercost=$(python3 ./mean.py /tmp/output-runs | grep "min" | awk '{print $8}')
								maxreqpercost=$(python3 ./mean.py /tmp/output-runs | grep "max" | awk '{print $8}')

								echo "RESULT followfast $NUM_RULES $PARAM_FILE $Pa $Pb $avgcostpereq $avgreqpercost $maxcostpereq $maxreqpercost"
							done
						done
					done
				done
			done
		done
	done
fi

if [[ $EXP == "list" || $EXP == "all" ]];then
	
	for NUM_RULES in ${RULES[@]};do

		for PARAM_FILE in ${PARAM_FILES[@]};do
		
			for SMOOTH in ${SMOOTHS[@]};do

				for ADDRESS_SCOPE in ${ADDRESS_SCOPES[@]};do

					for APP_SCOPE in ${APP_SCOPES[@]};do

						for Pa in ${Pas[@]};do

							for Pb in ${Pbs[@]};do

								ulimit -s 1638400

								i=0
								rm /tmp/output-runs > /dev/null 2> /dev/null
								rm /tmp/output-mems > /dev/null 2> /dev/null
								while [[ $i -lt $RUNS ]];do
									$SCRIPTS/genRule-Trace-single.sh $PARAM_FILE $NUM_RULES $SMOOTH $ADDRESS_SCOPE $APP_SCOPE $Pa $Pb $SCALE $i> /dev/null 2> /dev/null
								 	(taskset --cpu-list $i $FOLLOW %alg="list" %ruleset=$SCRIPTS/rules/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$i.rules %trace=$SCRIPTS/traces/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$Pa-$Pb-$SCALE-$i.trace > /tmp/output_$i; cat /tmp/output_$i | grep 'AverageCostPerReq' >> /tmp/output-runs) &
									i=$(( $i+1  ))
								done
								while [[ $(ps aux | grep "tools/classify" | wc -l) -gt 1 ]];do
									echo "waiting..."
									sleep 1
								done
								# cat /tmp/output-runs
								avgcostpereq=$(python3 ./mean.py /tmp/output-runs | grep "mean" | awk '{print $2}')
								minavgcostpereq=$(python3 ./mean.py /tmp/output-runs | grep "min" | awk '{print $2}')
								maxavgcostpereq=$(python3 ./mean.py /tmp/output-runs | grep "max" | awk '{print $2}')

								avgreqpercost=$(python3 ./mean.py /tmp/output-runs | grep "mean" | awk '{print $4}')
								minavgreqpercost=$(python3 ./mean.py /tmp/output-runs | grep "min" | awk '{print $4}')
								maxavgreqpercost=$(python3 ./mean.py /tmp/output-runs | grep "max" | awk '{print $4}')

								maxcostpereq=$(python3 ./mean.py /tmp/output-runs | grep "mean" | awk '{print $6}')
								minmaxcostpereq=$(python3 ./mean.py /tmp/output-runs | grep "min" | awk '{print $6}')
								maxmaxcostpereq=$(python3 ./mean.py /tmp/output-runs | grep "max" | awk '{print $6}')

								maxreqpercost=$(python3 ./mean.py /tmp/output-runs | grep "mean" | awk '{print $8}')
								minreqpercost=$(python3 ./mean.py /tmp/output-runs | grep "min" | awk '{print $8}')
								maxreqpercost=$(python3 ./mean.py /tmp/output-runs | grep "max" | awk '{print $8}')

								echo "RESULT list $NUM_RULES $PARAM_FILE $Pa $Pb $avgcostpereq $avgreqpercost $maxcostpereq $maxreqpercost"
							done
						done
					done
				done
			done
		done
	done
fi