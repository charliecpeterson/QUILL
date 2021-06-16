# Simple Hartree Fock code

This is a simple Hartree Fock code. This should only be used for testing. There are better SCF programs you should use (like NWChem, Molpro, Molcas, etc).


This code is a small C++ code that I built, based on Modern Quantum Chemistry bySzabo and Ostlund, as well as, notes from David Sherrill

Eigen was used for matrix equations and is needed to compile/run this code.

## Compiling

g++  -I/path/to/eigen/3.3.9/include/eigen3  -o scf.x scf.cpp

## Data

Data folder has intergal information for H2O/STO-3g

 
