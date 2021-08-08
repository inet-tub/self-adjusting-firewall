source source.sh

DIR=$(pwd)
RULESDIR=$(pwd)/rules

if [ ! -d $RULES ];then
	echo "no rules folder"
	exit
fi


# PARAM_FILES=(acl1_seed acl2_seed acl3_seed acl4_seed acl5_seed fw1_seed fw2_seed fw3_seed fw4_seed fw5_seed ipc1_seed ipc2_seed)
PARAM_FILES=(acl1_seed)
# RULES=(64 128 256 512 1024 2048 4096 8192 16384 32768 65536)
RULES=(64 128 256 512 1024 2048 4096 8192)

SMOOTHS=(2)
ADDRESS_SCOPES=(0.5)
APP_SCOPES=(-0.1)

Pas=(10)
Pbs=(1 10 100 1000 10000)
SCALES=(1000)

SCALE=1000

RUNS=32

# echo "RESULT ALG RULESETSIZE SEED SMOOTH ADDRESS_SCOPE APP_SCOPE PA PB SCALE AVG MIN_AVG32 MAX_AVG32 MEM_AVG MINMEM_AVG32 MAXMEM_AVG32"
echo "RULESETSIZE SEED DEPTH DEGREE ANCESTORS"
for NUM_RULES in ${RULES[@]};do

	for PARAM_FILE in ${PARAM_FILES[@]};do
	
		for SMOOTH in ${SMOOTHS[@]};do

			for ADDRESS_SCOPE in ${ADDRESS_SCOPES[@]};do

				for APP_SCOPE in ${APP_SCOPES[@]};do

					# for Pa in ${Pas[@]};do

						# for Pb in ${Pbs[@]};do
							FILE=/tmp/output
							rm $FILE > /dev/null 2> /dev/null
							i=0
							while [[ $i -lt $RUNS ]];do
								(python3 $DIR/ruleset-analysis.py $SCRIPTS/rules/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE-$i.rules >> $FILE) &
								i=$(( $i+1  ))
							done
							while [[ $(ps aux | grep "ruleset-analysis" | wc -l) -gt 1 ]];do
								sleep 1
							done
							cat $FILE | awk '{print $4}' > /tmp/output-depth
							cat $FILE | awk '{print $2}' > /tmp/output-degree
							cat $FILE | awk '{print $8}' > /tmp/output-ancestors
							depth=$(python3 $SCRIPTS/mean.py /tmp/output-depth | grep "mean" | awk '{print $2}')
							degree=$(python3 $SCRIPTS/mean.py /tmp/output-degree | grep "mean" | awk '{print $2}')
							ancestors=$(python3 $SCRIPTS/mean.py /tmp/output-ancestors | grep "mean" | awk '{print $2}')
							echo "$NUM_RULES $PARAM_FILE $depth $degree $ancestors"
						# done
					# done
				done
			done
		done
	done
done