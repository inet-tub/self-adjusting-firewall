# bash

#  This script plays (actually changes them) with seed files on the parameter_files2
#  call it with ./play_with_seeds.sh
#
# It will ask for a value for both snest and dnest and it will generate 1k ruleset
# filter sets for all seeds, storing the results in /tmp/results/
# You can run the script many times with different nest values and new files will
# be created on the same folder.
# Finally execute:
#  cat /tmp/results/* > /tmp/res
# To get a CSV-like file with all results including seed, numbre of rules, nest value, depth.



echo "Please insert nest value:"
read value

path_cb="../../classbench"
tmp_path="/tmp/results"
filterset_path="/tmp/filterset"
#rm -rf ${tmp_path}/*
mkdir -p ${tmp_path}
#rm -rf ${tmp_path}/*
rm -rf ${filterset_path}/*
mkdir -p ${filterset_path}
rm -rf ${filterset_path}/*

for seed in $(ls ${path_cb}/parameter_files2/ ); do
    echo $seed
    perl -0777 -i -pe "s/nest\n[0-9]+\n#/nest\n${value}\n#/igs" ${path_cb}/parameter_files2/${seed}

    ${path_cb}/db_generator/db_generator -bc ${path_cb}/parameter_files2/${seed} 1000 1 0.5 0.5 ${filterset_path}/FilterSet_test_${seed}

    # python3 ruleset-analysis.py ./../../classbench/db_generator/MyTest1knest50

    # perl -0777 -i.original -pe 's/nest\n[0-9]+\n#/nest\n999\n#/igs' parameter_files2/acl1_seed
done
#
# echo "When files are ready, please hit enter to continue"
# read varname
# echo "Nice, resuming tasks..."



# cd ../follow/scripts/
pwd

for seed in $(ls ${path_cb}/parameter_files2/ ); do
    echo $seed

    python3 ruleset-analysis.py ${filterset_path}/FilterSet_test_${seed} ${tmp_path}/${seed}_${value} ${value}&

    # perl -0777 -i.original -pe 's/nest\n[0-9]+\n#/nest\n999\n#/igs' parameter_files2/acl1_seed
done

# perl -0777 -i.original -pe 's/nest\n[0-9]+\n#/nest\n999\n#/igs' parameter_files2/acl1_seed

# ./db_generator -bc ../parameter_files2/test 1000 2 0.5 -0.1 MyTest1knest50

# python3 ruleset-analysis.py ./../../classbench/db_generator/MyTest1knest50
