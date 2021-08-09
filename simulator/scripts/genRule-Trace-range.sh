source ./source.sh # sanity check for paths. Remove this line later

cd $CLASSBENCH/db_generator
make all

cd $CLASSBENCH/trace_generator
make all

if [ ! -d $SCRIPTS/rules ];then
	mkdir $SCRIPTS/rules
fi

if [ ! -d $SCRIPTS/traces ];then
	mkdir $SCRIPTS/traces
fi

############ ALERT!!!! ###########
# This script will generate a lot of files, could be unnecessary for simple experiments. Use "genRule-Trace-single.sh" for a single pair of ruleset-trace
#################################

PARAM_FILES=("fw1_seed" "fw2_seed" "fw3_seed" "fw4_seed" "fw5_seed" "acl1_seed" "acl2_seed" "acl3_seed" "acl4_seed" "acl5_seed")
SIZE=(10000 100000)
# SMOOTHNESS=(0 1 2 4 8 16 32 64)
SMOOTHNESS=(2)
ADDRESS_SCOPES=(0.5)
APP_SCOPES=(-0.1)
# ADDRESS_SCOPES=(-1 -0.5 0 0.5 1)
# APP_SCOPES=(-1 -0.5 0 0.5 1)

PARETO_A=(1)
PARETO_B=(0 0.001 0.01 0.1 1)

for PARAM_FILE in ${PARAM_FILES[@]};do

	for NUM_RULES in ${SIZE[@]};do

		SCALE=10

		for SMOOTH in ${SMOOTHNESS[@]};do

			for ADDRESS_SCOPE in ${ADDRESS_SCOPES[@]};do

				for APP_SCOPE in ${APP_SCOPES[@]};do

					for Pa in ${PARETO_A[@]};do

						for Pb in ${PARETO_B[@]};do
							
							echo "$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE.rules"

							cd $CLASSBENCH/db_generator

							./db_generator -bc $CLASSBENCH/parameter_files/$PARAM_FILE $NUM_RULES $SMOOTH $ADDRESS_SCOPE $APP_SCOPE $SCRIPTS/rules/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE.rules

							cd $CLASSBENCH/trace_generator

							./trace_generator $Pa $Pb $SCALE $SCRIPTS/rules/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE.rules

						done
					done
				done
			done
		done
	done
done


mv $SCRIPTS/rules/*_trace $SCRIPTS/traces/

####### FROM CLASSBENCH README ##########
# db_generator -hrb (-c <input parameter file>) <number of filters> <smoothness> <address scope> <application scope> <output filename>
# db_generator -bc MyParameters 10000 2 0.5 -0.1 MyFilters10k

# trace_generator <Pareto parameter a> <Pareto parameter b> <scale> <filter set filename>
# trace_generator 1 0.1 10 MyFilters

# Pareto cummulative density function: D(x) = 1 - (b/x)^a
# 	 	 Examples:
# 	 	 No locality of reference, a = 1, b = 0
# 	 	 Low locality of reference, a = 1, b = 0.0001
# 	 	 High locality of reference, a = 1, b = 1
# 	 Scale parameter limits the size of the trace
# 	 	 Threshold = (# of filters in filter set) * scale
# 	 	 Once the size of the trace exceeds the threshold, the generator terminates
# 	 	 Note that a large burst near the end of the process may cause the trace to be considerably
# 	 	 larger than the Threshold
#########################################