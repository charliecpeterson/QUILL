#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>
#include "geo.h"
#include <sys/stat.h>
#include <unistd.h>

using namespace::std;
void geo_make_mol(){
    mkdir("../mol",0777);
    cout << "Creating new geometry information:\n" << endl;
    int natom=0;
    int *atoms = new int[50];
    double *x = new double[50];
    double *y = new double[50];
    double *z = new double[50];
    ifstream input("INPUT");
    ofstream molout("../mol/mol");
    string newline;
    getline(input,newline);
    istringstream ss(newline);
    while(getline(input,newline)){
        istringstream ss(newline);
        string atoms_temp;
        ss >> atoms_temp >> x[natom] >> y[natom] >> z[natom];
        atoms[natom] = con_atom2Z(atoms_temp);
        natom++;
    }
    //get bond lengths
    if (natom > 1){
        cout << "Getting bond lengths \n" << endl;
        for (int i=1; i <= natom;i++){
            for (int j =1; j <=i;j++){
                if (i == j) continue;
                cout << "Atom " << i << " - " << "Atom " << j << ": " << get_bond_length(x[i-1],y[i-1],z[i-1],x[j-1],y[j-1],z[j-1]) << " AU" << endl;
            }
        }
    }
    //Writing to file
    molout << natom << endl;
    for ( int i =0 ; i < natom ; i++){
        molout << atoms[i] << " " << x[i] << " " << y[i] << " " << z[i] << endl;
    }
}

double get_bond_length(double ax, double ay, double az, double bx, double by, double bz){
    return sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by) + (az - bz) * (az - bz));
}
int con_atom2Z(string atom){
    if (atom == "H") return 1;
    if (atom == "He") return 2;
    if (atom == "Li") return 3;
    if (atom == "Be") return 4;
    if (atom == "B") return 5;
    if (atom == "C") return 6;
    if (atom == "N") return 7;
    if (atom == "O") return 8;
    if (atom == "F") return 9;
    if (atom == "Ne") return 10;
    return 0;
}