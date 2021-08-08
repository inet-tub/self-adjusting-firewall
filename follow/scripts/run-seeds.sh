source source.sh

RESULTS=$(pwd)/results
OUTPUT=$(pwd)/output

if [ ! -d $RESULTS ];then
	mkdir $RESULTS
fi

if [ ! -d $OUTPUT ];then
	mkdir $OUTPUT
fi

FOLLOW=$FIREWALL/Default/follow
CUTSPLIT="/home/vamsi/src/phd/self-adjusting-firewall/cutsplit/CutSplit"

# ./main -b 8 -t 20 -r ./../classbench/db_generator/MyFilters10k -e ./../classbench/db_generator/MyFilters10k_trace -c 1 -u 0 -o ./output.dat
# ./follow --alg="follow" --output=./output.dat


PARAM_FILES=(acl1_seed acl2_seed acl3_seed acl4_seed acl5_seed fw1_seed fw2_seed fw3_seed fw4_seed fw5_seed ipc1_seed ipc2_seed)
# PARAM_FILES=(acl1_seed)
# RULES=(64 128 256 512 1024 2048 4096 8192 16384 32768 65536)
RULES=(256)
# RULES=(1024 2048)
# RULES=(4096 8192)
# RULES=(64)

SMOOTHS=(2)
ADDRESS_SCOPES=(0.5)
APP_SCOPES=(-0.1)

Pas=(10)
Pbs=(1 10 100 1000 10000)
SCALES=(1000)

SCALE=1000

RUNS=32

echo "RESULT ALG RULESETSIZE SEED SMOOTH ADDRESS_SCOPE APP_SCOPE PA PB SCALE AVG MIN_AVG32 MAX_AVG32 MEM_AVG MINMEM_AVG32 MAXMEM_AVG32"

EXP=$1

if [[ $EXP == "follow" || $EXP == "all" ]];then
	################### FOLLOW #################################
	for NUM_RULES in ${RULES[@]};do

		for PARAM_FILE in ${PARAM_FILES[@]};do
		
			for SMOOTH in ${SMOOTHS[@]};do

				for ADDRESS_SCOPE in ${ADDRESS_SCOPES[@]};do

					for APP_SCOPE in ${APP_SCOPES[@]};do

						for Pa in ${Pas[@]};do

							for Pb in ${Pbs[@]};do

								# SCALE=$(python3 -c "print(1000*1000*1000*10/$NUM_RULES)")

								ulimit -s 1638400

								# echo "$PARAM_FILE $NUM_RULES $SMOOTH $ADDRESS_SCOPE $APP_SCOPE $Pa $Pb $SCALE"
								
								# sleep 1000
								# $CUTSPLIT -b 8 -t 24 -r $SCRIPTS/rules/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE.rules -e $SCRIPTS/traces/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$Pa-$Pb-$SCALE.trace  # > $OUTPUT/cutsplit-$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$Pa-$Pb-$SCALE.output
								
								i=0
								rm /tmp/output-runs > /dev/null 2> /dev/null
								rm /tmp/output-mems > /dev/null 2> /dev/null
								while [[ $i -lt $RUNS ]];do
									$SCRIPTS/genRule-Trace-single.sh $PARAM_FILE $NUM_RULES $SMOOTH $ADDRESS_SCOPE $APP_SCOPE $Pa $Pb $SCALE $i> /dev/null 2> /dev/null
								 	(taskset --cpu-list $i $FOLLOW %alg="follow" %ruleset=$SCRIPTS/rules/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$i.rules %trace=$SCRIPTS/traces/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$Pa-$Pb-$SCALE-$i.trace > /tmp/output_$i; nodes=$(cat /tmp/output_$i | grep "AverageTraversed" | awk '{print $2}'); mem=$(cat /tmp/output_$i | grep "TotalMemoryBytes" | awk '{print $2}'); echo $mem >> /tmp/output-mems; echo $nodes >> /tmp/output-runs) &
									i=$(( $i+1  ))
								done
								while [[ $(ps aux | grep "Default/follow" | wc -l) -gt 1 ]];do
									echo "waiting..."
									sleep 1
								done
								# cat /tmp/output-runs
								nodes=$(python3 ./mean.py /tmp/output-runs | grep "mean" | awk '{print $2}')
								min=$(python3 ./mean.py /tmp/output-runs | grep "min" | awk '{print $2}')
								max=$(python3 ./mean.py /tmp/output-runs | grep "max" | awk '{print $2}')
								mem=$(python3 ./mean.py /tmp/output-mems | grep "mean" | awk '{print $2}')
								memmin=$(python3 ./mean.py /tmp/output-mems | grep "min" | awk '{print $2}')
								memmax=$(python3 ./mean.py /tmp/output-mems | grep "max" | awk '{print $2}')
								
								echo "RESULT follow $NUM_RULES $PARAM_FILE $SMOOTH $ADDRESS_SCOPE $APP_SCOPE $Pa $Pb $SCALE $nodes $min $max $mem $memmin $memmax"
							done
						done
					done
				done
			done
		done
	done
fi

if [[ $1 == "followfast" || $1 == "all" ]];then
	################### FOLLOWFAST #################################
	for NUM_RULES in ${RULES[@]};do

		for PARAM_FILE in ${PARAM_FILES[@]};do
		
			for SMOOTH in ${SMOOTHS[@]};do

				for ADDRESS_SCOPE in ${ADDRESS_SCOPES[@]};do

					for APP_SCOPE in ${APP_SCOPES[@]};do

						for Pa in ${Pas[@]};do

							for Pb in ${Pbs[@]};do

								# SCALE=$(python3 -c "print(1000*1000*1000*10/$NUM_RULES)")

								ulimit -s 1638400

								# echo "$PARAM_FILE $NUM_RULES $SMOOTH $ADDRESS_SCOPE $APP_SCOPE $Pa $Pb $SCALE"
								
								# sleep 1000
								# $CUTSPLIT -b 8 -t 24 -r $SCRIPTS/rules/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE.rules -e $SCRIPTS/traces/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$Pa-$Pb-$SCALE.trace  # > $OUTPUT/cutsplit-$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$Pa-$Pb-$SCALE.output
								
								i=0
								rm /tmp/output-runs > /dev/null 2> /dev/null
								rm /tmp/output-mems > /dev/null 2> /dev/null
								while [[ $i -lt $RUNS ]];do
									$SCRIPTS/genRule-Trace-single.sh $PARAM_FILE $NUM_RULES $SMOOTH $ADDRESS_SCOPE $APP_SCOPE $Pa $Pb $SCALE $i> /dev/null 2> /dev/null
								 	(taskset --cpu-list $i $FOLLOW %alg="followfast" %ruleset=$SCRIPTS/rules/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$i.rules %trace=$SCRIPTS/traces/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$Pa-$Pb-$SCALE-$i.trace > /tmp/output_$i; nodes=$(cat /tmp/output_$i | grep "AverageTraversed" | awk '{print $2}'); mem=$(cat /tmp/output_$i | grep "TotalMemoryBytes" | awk '{print $2}'); echo $mem >> /tmp/output-mems; echo $nodes >> /tmp/output-runs) &
									
									i=$(( $i+1  ))
								done
								while [[ $(ps aux | grep "Default/follow" | wc -l) -gt 1 ]];do
									echo "waiting..."
									sleep 1
								done
								# cat /tmp/output-runs
								nodes=$(python3 ./mean.py /tmp/output-runs | grep "mean" | awk '{print $2}')
								min=$(python3 ./mean.py /tmp/output-runs | grep "min" | awk '{print $2}')
								max=$(python3 ./mean.py /tmp/output-runs | grep "max" | awk '{print $2}')
								mem=$(python3 ./mean.py /tmp/output-mems | grep "mean" | awk '{print $2}')
								memmin=$(python3 ./mean.py /tmp/output-mems | grep "min" | awk '{print $2}')
								memmax=$(python3 ./mean.py /tmp/output-mems | grep "max" | awk '{print $2}')
								
								echo "RESULT followfast $NUM_RULES $PARAM_FILE $SMOOTH $ADDRESS_SCOPE $APP_SCOPE $Pa $Pb $SCALE $nodes $min $max $mem $memmin $memmax"
							done
						done
					done
				done
			done
		done
	done
fi

if [[ $1 == "efficuts" || $1 == "all" ]];then
	################### EFFICUTS #################################
	for NUM_RULES in ${RULES[@]};do

		for PARAM_FILE in ${PARAM_FILES[@]};do
		
			for SMOOTH in ${SMOOTHS[@]};do

				for ADDRESS_SCOPE in ${ADDRESS_SCOPES[@]};do

					for APP_SCOPE in ${APP_SCOPES[@]};do

						for Pa in ${Pas[@]};do

							for Pb in ${Pbs[@]};do

								# SCALE=$(python3 -c "print(1000*1000*1000*10/$NUM_RULES)")
								ulimit -s 1638400

								# echo "$PARAM_FILE $NUM_RULES $SMOOTH $ADDRESS_SCOPE $APP_SCOPE $Pa $Pb $SCALE"
								
								# sleep 1000
								# $CUTSPLIT -b 8 -t 24 -r $SCRIPTS/rules/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE.rules -e $SCRIPTS/traces/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$Pa-$Pb-$SCALE.trace  # > $OUTPUT/cutsplit-$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$Pa-$Pb-$SCALE.output
								
								i=0
								rm /tmp/output-runs > /dev/null 2> /dev/null
								rm /tmp/output-mems > /dev/null 2> /dev/null
								while [[ $i -lt $RUNS ]];do
									$SCRIPTS/genRule-Trace-single.sh $PARAM_FILE $NUM_RULES $SMOOTH $ADDRESS_SCOPE $APP_SCOPE $Pa $Pb $SCALE $i > /dev/null 2> /dev/null
									
									(taskset --cpu-list $i $FOLLOW %alg="cuts" %ruleset=$SCRIPTS/rules/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$i.rules %trace=$SCRIPTS/traces/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$Pa-$Pb-$SCALE-$i.trace %spfac=8 %hypercuts=1 %compress=1 %binON=1 %mergingON=1 > /tmp/output_$i; nodes=$(cat /tmp/output_$i | grep "AverageTraversed" | awk '{print $2}'); mem=$(cat /tmp/output_$i | grep "TotalMemoryBytes" | awk '{print $2}'); echo $mem >> /tmp/output-mems; echo $nodes >> /tmp/output-runs)
									
									i=$(( $i+1  ))
								done
								while [[ $(ps aux | grep "Default/follow" | wc -l) -gt 1 ]];do
									echo "waiting..."
									sleep 1
								done
								# cat /tmp/output-runs
								nodes=$(python3 ./mean.py /tmp/output-runs | grep "mean" | awk '{print $2}')
								min=$(python3 ./mean.py /tmp/output-runs | grep "min" | awk '{print $2}')
								max=$(python3 ./mean.py /tmp/output-runs | grep "max" | awk '{print $2}')
								mem=$(python3 ./mean.py /tmp/output-mems | grep "mean" | awk '{print $2}')
								memmin=$(python3 ./mean.py /tmp/output-mems | grep "min" | awk '{print $2}')
								memmax=$(python3 ./mean.py /tmp/output-mems | grep "max" | awk '{print $2}')
								echo "RESULT efficuts $NUM_RULES $PARAM_FILE $SMOOTH $ADDRESS_SCOPE $APP_SCOPE $Pa $Pb $SCALE $nodes $min $max $mem $memmin $memmax"

							done
						done
					done
				done
			done
		done
	done
fi


if [[ $EXP == "cutsplit" || $EXP == "all" ]];then
	################### FOLLOW #################################
	for NUM_RULES in ${RULES[@]};do

		for PARAM_FILE in ${PARAM_FILES[@]};do
		
			for SMOOTH in ${SMOOTHS[@]};do

				for ADDRESS_SCOPE in ${ADDRESS_SCOPES[@]};do

					for APP_SCOPE in ${APP_SCOPES[@]};do

						for Pa in ${Pas[@]};do

							for Pb in ${Pbs[@]};do

								# SCALE=$(python3 -c "print(1000*1000*1000*10/$NUM_RULES)")

								ulimit -s 1638400

								# echo "$PARAM_FILE $NUM_RULES $SMOOTH $ADDRESS_SCOPE $APP_SCOPE $Pa $Pb $SCALE"
								
								# sleep 1000
								# $CUTSPLIT -b 8 -t 24 -r $SCRIPTS/rules/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE.rules -e $SCRIPTS/traces/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$Pa-$Pb-$SCALE.trace  # > $OUTPUT/cutsplit-$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$Pa-$Pb-$SCALE.output
								
								i=0
								rm /tmp/output-runs > /dev/null 2> /dev/null
								rm /tmp/output-mems > /dev/null 2> /dev/null
								while [[ $i -lt $RUNS ]];do
									$SCRIPTS/genRule-Trace-single.sh $PARAM_FILE $NUM_RULES $SMOOTH $ADDRESS_SCOPE $APP_SCOPE $Pa $Pb $SCALE $i> /dev/null 2> /dev/null
								 	# (taskset --cpu-list $i $CUTSPLIT -b 8 -t 24 -r $SCRIPTS/rules/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$i.rules -e $SCRIPTS/traces/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$Pa-$Pb-$SCALE-$i.trace > /tmp/output_$i; nodes=$(cat /tmp/output_$i | grep "AverageTraversed" | awk '{print $2}'); mem=$(cat /tmp/output_$i | grep "TotalMemoryBytes" | awk '{print $2}'); echo $mem >> /tmp/output-mems; echo $nodes >> /tmp/output-runs) &
								 	(taskset --cpu-list $i $FOLLOW %alg="cutsplit" %ruleset=$SCRIPTS/rules/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$i.rules %trace=$SCRIPTS/traces/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$Pa-$Pb-$SCALE-$i.trace > /tmp/output_$i; nodes=$(cat /tmp/output_$i | grep "AverageTraversed" | awk '{print $2}'); mem=$(cat /tmp/output_$i | grep "TotalMemoryBytes" | awk '{print $2}'); echo $mem >> /tmp/output-mems; echo $nodes >> /tmp/output-runs) &
									i=$(( $i+1  ))
								done
								while [[ $(ps aux | grep "CutSplit" | wc -l) -gt 1 ]];do
									echo "waiting..."
									sleep 1
								done
								# cat /tmp/output-runs
								nodes=$(python3 ./mean.py /tmp/output-runs | grep "mean" | awk '{print $2}')
								min=$(python3 ./mean.py /tmp/output-runs | grep "min" | awk '{print $2}')
								max=$(python3 ./mean.py /tmp/output-runs | grep "max" | awk '{print $2}')
								mem=$(python3 ./mean.py /tmp/output-mems | grep "mean" | awk '{print $2}')
								memmin=$(python3 ./mean.py /tmp/output-mems | grep "min" | awk '{print $2}')
								memmax=$(python3 ./mean.py /tmp/output-mems | grep "max" | awk '{print $2}')
								
								echo "RESULT cutsplit $NUM_RULES $PARAM_FILE $SMOOTH $ADDRESS_SCOPE $APP_SCOPE $Pa $Pb $SCALE $nodes $min $max $mem $memmin $memmax"
							done
						done
					done
				done
			done
		done
	done
fi
