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
  

# PARAM_FILE=$1
# NUM_RULES=$2
# SMOOTH=$3
# ADDRESS_SCOPE=$4
# APP_SCOPE=$5
# $RULESET_OUT=$6

# $Pa=$7
# $pb=$8
# $SCALE=$9

PARAM_FILE="fw1_seed"
NUM_RULES=10000
SMOOTH=2
ADDRESS_SCOPE=0.5
APP_SCOPE=-0.1

Pa=1
Pb=0.1
SCALE=10

cd $CLASSBENCH/db_generator

./db_generator -bc $CLASSBENCH/parameter_files/$PARAM_FILE $NUM_RULES $SMOOTH $ADDRESS_SCOPE $APP_SCOPE $SCRIPTS/rules/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE.rules

cd $CLASSBENCH/trace_generator

./trace_generator $Pa $Pb $SCALE $SCRIPTS/rules/$PARAM_FILE-$NUM_RULES-$SMOOTH-$ADDRESS_SCOPE-$APP_SCOPE.rules

mv $SCRIPTS/rules/*_trace $SCRIPTS/traces/

####### FROM CLASSBENCH README ##########
# db_generator -hrb (-c <input parameter file>) <number of filters> <smoothness> <address scope> <application scope> <output filename>
# db_generator -bc MyParameters 10000 2 0.5 -0.1 MyFilters10k

# trace_generator <Pareto parameter a> <Pareto parameter b> <scale> <filter set filename>
# trace_generator 1 0.1 10 MyFilters
#########################################