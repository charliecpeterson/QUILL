#include "int.h"
#include "inp.h"
#include "geo.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <Python.h>
#include <cstdlib>


using namespace::std;
void create_int(){


	//Reading mol file
	int natm;
	ifstream molfile("../mol/mol");
	string newline;
	getline(molfile,newline);
	stringstream ss(newline);
	ss >> natm;
	string mol_string = "";
    for(int i =0; i < natm; i++){
		getline(molfile,newline);
		mol_string.append(newline); mol_string.append(";");
    }
    molfile.close();
	//Reading basis file
	ifstream basisfile("INPUT");
	getline(basisfile,newline);
	string basis_string="";
	while(getline(basisfile,newline)){
		basis_string.append(newline); basis_string.append("\n");
	}

	string run_string ="python python_run.py";
	ofstream python_run("python_run.py");
	python_run << "import pyscf" << endl;
	python_run << "from pyscf import gto, scf" << endl;
	python_run << "mol = gto.Mole()" << endl;
	python_run << "mol.build( atom = '''" << mol_string << "''', unit = 'bohr')" << endl;
	python_run << "mol.basis = {'''\n" << basis_string << "'''}" << endl;
	python_run << "t = mol.intor('int1e_kin')" << endl;
	python_run << "tfile = open(\"../mol/t.dat\",\"a\")" << endl;
	python_run << "num_rows, num_cols = t.shape" << endl;
	python_run << "for i in range(num_rows):\n   for j in range(num_cols):" << endl;
	python_run << "     if j > i:"<< endl;
	python_run << "        continue" << endl;
	python_run << "     tfile.write(str(i+1) + \" \" + str(j+1) + \" \" + str(t[i][j]) + \"\\n\")" << endl;

	python_run << "s = mol.intor('int1e_ovlp')" << endl;
	python_run << "sfile = open(\"../mol/s.dat\",\"a\")" << endl;
	python_run << "num_rows, num_cols = s.shape" << endl;
	python_run << "for i in range(num_rows):\n   for j in range(num_cols):" << endl;
	python_run << "     if j > i:"<< endl;
	python_run << "        continue" << endl;
	python_run << "     sfile.write(str(i+1) + \" \" + str(j+1) + \" \" + str(s[i][j]) + \"\\n\")" << endl;

	python_run << "v = mol.intor('int1e_nuc')" << endl;
	python_run << "vfile = open(\"../mol/v.dat\",\"a\")" << endl;
	python_run << "num_rows, num_cols = v.shape" << endl;
	python_run << "for i in range(num_rows):\n   for j in range(num_cols):" << endl;
	python_run << "     if j > i:"<< endl;
	python_run << "        continue" << endl;
	python_run << "     vfile.write(str(i+1) + \" \" + str(j+1) + \" \" + str(v[i][j]) + \"\\n\")" << endl;

	python_run << "eri = mol.intor('int2e')" << endl;
	python_run << "erifile = open(\"../mol/eri.dat\",\"a\")" << endl;
	python_run << "for i in range(num_rows):" << endl;
	python_run << "   for j in range(num_cols):" << endl;
	python_run << "      for k in range(num_cols):" << endl;
	python_run << "         for l in range(num_cols):" << endl;
	python_run << "            if j > i and k > l:"<< endl;
	python_run << "                continue" << endl;
	python_run << "            erifile.write(str(i+1) + \" \" + str(j+1) + \" \" + str(k+1) + \" \" + str(l+1) + \" \" + str(eri[i][j][k][l]) + \"\\n\")" << endl;
	
	python_run << "enuc = mol.get_enuc()" << endl;
	python_run << "enucfile = open(\"../mol/enuc.dat\",\"a\")" << endl;
	python_run << "enucfile.write(str(enuc))" << endl;

	python_run << "num_functions = mol.nao_nr()" << endl;
	python_run << "numffile = open(\"../mol/num_nao.dat\",\"a\")" << endl;
	python_run << "numffile.write(str(num_functions))" << endl;

	char *run_char = new char[run_string.length() +1];
	strcpy(run_char,run_string.c_str());
	system(run_char);



}