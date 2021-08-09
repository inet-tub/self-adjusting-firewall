# Build

```bash

make -j classify
```

An executable `classify` will be created in tools directory

# Directories

- `src/` folder contains the source code for this simulator including various algorithms, some re-implemented.
- `examples/` folder contains experiment codes. Specifically a main function should go here.

# Excecutable

- `classify` in the `tools/` folder can be used for now to run simple experiments to test follow algorithm. Syntax is as follows:

```bash
# Follow algorithm : List based firewall
./classify %ruleset=<path-to-a-ruleset> %trace=<path-to-a-trace> %alg="follow"
```
```bash
# CutSplit
./classify %ruleset=<path-to-a-ruleset> %trace=<path-to-a-trace> %alg="cutsplit"
```

```bash
# Efficuts
./classify %ruleset=<path-to-a-ruleset> %trace=<path-to-a-trace> %alg="cuts" %spfac=8 %hypercuts=1 %compress=1 %binON=1 %mergingON=1
```

```bash
# Hypercuts
./classify %ruleset=<path-to-a-ruleset> %trace=<path-to-a-trace> %alg="cuts" %spfac=8 %hypercuts=1 %compress=0 %binON=0 %mergingON=0
```

```bash
# Hicuts
./classify %ruleset=<path-to-a-ruleset> %trace=<path-to-a-trace> %alg="cuts" %spfac=8 %hypercuts=0 %compress=0 %binON=0 %mergingON=0
```