# Quill Program for computational chemistry

This is a simple Hartree Fock code. This should only be used for testing. There are better SCF programs you should use (like NWChem, Molpro, Molcas, etc).


This code is a small C++ code that I built, based on Modern Quantum Chemistry bySzabo and Ostlund, as well as, notes from David Sherrill

Eigen was used for matrix equations and is needed to compile/run this code.

The PySCF/libcint libraries were used to create the 1e/2e integrals

## Compiling

Required:
- Python with [PySCF](https://github.com/pyscf/pyscf)
- [Eigen3](https://eigen.tuxfamily.org/dox/)

```
mkdir build ; cd build
cmake ..
make
```

## Data

There is a sample input file (inputThere is a sample input file (input.inp).

## Running QUILL

I built this mostly to test stuff out. There are three functions in the input

- geometry
- basis
- scf

### geometry

This is very similar to NWChem input formats. You will need the atom and XYZ coords in **AU/BOHR**

### basis

This is basic NWChem basis set format.

### scf

This is ONLY **RHF**. So no open shells (for now). You will need to add the option **num_elec** line in this block to input the number of electrons

### Runing code

The binary is `QUILL.x` and it will need an input file.

```
./QUILL.x input.inp
```

It will create a tmp/scratch directory from where you are running `QUILL.x`.
 
## Accuarcy

I tested these on a few compounds and it is about 10^-2 AU in energy from the results from PySCF. Obviously, PySCF would be better. 

## Next Steps

This code was mostly so I can play around with programming chemistry functions and getting my hands dirty. Possibly I would expand it to be more accurate and and more function. 

Also, I wanted some program to play with other libraries and functions that others have made so I have a program to test.

## Contact

Feel free to contact me about this code or anything else!

Charles Peterson - charles.c.peterson3@gmail.com

## Version updates

- v1.0.0

First major release of the code. Support a free flow input file

- v0.9.0

A begining test of the program with pre computed integrals

 


