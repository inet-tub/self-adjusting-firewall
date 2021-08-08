# build

Run the `setup.sh` script to automate the build process.
```bash

./setup.sh
```

# Directories

- `src/` folder contains the source code for this simulator including various algorithms, some re-implemented.
- `examples/` folder contains experiment codes. Specifically a main function should go here.
- `Default/` is the makefile location. Run `make -j` in this directory to build the simulator. After a successful make, an executable `follow` is created in this directory.  


# Excecutable

- `follow` in the `Default/` folder can be used for now to run simple experiments to test follow algorithm. Syntax is as follows:

```bash
# Follow algorithm : List based firewall
./follow %ruleset=<path-to-a-ruleset> %trace=<path-to-a-trace> %alg=<follow | followfast | cutsplit | cuts>
```

```bash
# Efficuts/Hicuts/Hypercuts
./follow %ruleset=<path-to-a-ruleset> %trace=<path-to-a-trace> %alg=<follow | followfast | cutsplit | cuts>
```