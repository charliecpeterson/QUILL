#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <filesystem>
#include "inp.h"
#include "geo.h"
#include "scf.h"
#include "int.h"
using namespace::std;

int main(int argc, char **argv) {
    cout << "Welcome to the QUILL program for quantum chemistry" << endl;
    int num_inp_routines;
    num_inp_routines = read_inp(argv[1]);
    //Running routines
    chdir("QUILL-tmp");
    for (int i =1 ; i <= num_inp_routines; i++){
        chdir(&to_string(i)[0]);
        ifstream INPUT("INPUT");
        string firstline;
        getline(INPUT,firstline);
        istringstream ss(firstline);
        if (firstline == "geometry"){
            cout << "\n\nRunning Geometry:\n" << endl;
            geo_make_mol();
        }
        if (firstline == "basis"){
            cout << "\n\nRunning intergals:\n" << endl;
            create_int();
        }
        if (firstline == "scf"){
            cout << "\n\nRunning SCF:\n" << endl;
            run_scf();
        }
        chdir("..");
    }
    chdir("..");
    string run_string = "rm -rf ./QUILL-tmp";
    char *run_char = new char[run_string.length() +1];
	strcpy(run_char,run_string.c_str());
	system(run_char);
    system(run_char);
    cout << "Thank you for using the QUILL program" << endl;
    return 0;
}

