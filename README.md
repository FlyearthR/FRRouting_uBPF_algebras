# FRRouting_uBPF_algebras
## RIPng\_integer\_uBPF

RIPng\_integer\_uBPF contains the implementation described in the master thesis.
To use it, follow the instruction of the FRRouting documentation.
A file `cust_conf` is supplied as example of modification needed to use uBPF.
Modify the lines 171, 175 and 176 in RIPng\_integer\_uBPF/ripngd/ripng\_main.c according to your working diretory.

The algebras are in RIPng\_integer\_uBPF/ripngd/algebra

The framework tests are in RIPng\_integer\_uBPF/ripngd/algebra/test.c
Use `make` to compile the tests and `./test` to run them.

The plugin to modify to use new algebras is in RIPng\_integer\_uBPF/ubpf\_tools/plugins/ripng
Use `make` to compile it.

## RIPng\_all\_uBPF\_WIP

RIPng\_all\_uBPF\_WIP contains the work-in-progress RIPng implementation which supports complex algebras.

## Scripts

Scripts contains all the scripts used for check convegence, collect data, etc.
In Scripts/data you can find the results (for the RIPng base implementation and RIPng with ADD algebra without uBPF) of scripts collecting time of convergence as raw data or as graphs.
