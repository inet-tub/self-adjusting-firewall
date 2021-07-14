git clone https://github.com/drjdaly/tuplemerge.git
mv src/Tuplemerge/TupleMergeOnline.h.changed tuplemerge/TupleMerge/TupleMergeOnline.h

cd tuplemerge
make -j main

cd ./../Default/
make -j
