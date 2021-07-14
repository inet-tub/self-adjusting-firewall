# Directories

- `src/` folder contains the source code for this simulator including various algorithms, some re-implemented.
- `tuplemerge/` folder is the original reposipority of tuplemerge algorithm. Some files from tuplemerge are a dependency for this simulator.
	- git clone https://github.com/drjdaly/tuplemerge.git
- `examples/` folder contains experiment codes. Specifically a main function should go here.
- `Default/` is the makefile location. Run `make -j` in this directory to build the simulator. After a successful make, an executable `follow` is created in this directory. Make sure to clone `tuplemerge` repository within this folder.  


# Excecutable

- `follow` in the `Default/` folder can be used for now to run simple experiments to test follow algorithm. Syntax is as follows:

````
./follow --ruleset=<path-to-a-ruleset> --trace=<path-to-a-trace>

```