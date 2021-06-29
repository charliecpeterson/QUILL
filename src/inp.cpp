/*
    This is to read input files and create tmp files to setup the main routines
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include "inp.h"
using namespace std;

int read_inp(string filename){
    int num_routines =3; // number of functions
    int num_inp_routines = 0; // number of functions read in by input
    string AVAIL_ROUTINES[3] = {"geometry","scf", "basis"};
    ifstream input(filename);
    string newline;

    mkdir("QUILL-tmp",0777); // create tmp dir

    while(getline(input, newline)){
        istringstream ss(newline);
        if (newline == "" ) continue; //skip blank lines and comments
        if (is_in_array(newline,AVAIL_ROUTINES, num_routines)){
            num_inp_routines++;
            string create_dir = "QUILL-tmp/" + to_string(num_inp_routines);
            mkdir(&create_dir[0],0777);
            chdir(&create_dir[0]);
            ofstream INPUT("INPUT");
            INPUT << newline << endl;
            while(getline(input, newline)){
                istringstream ss(newline);
                if (newline == "end") break;
                INPUT << newline << endl;
            }
            INPUT.close();
            chdir("../..");
        } else {
            cout << "Sorry but the routine command: " << newline;
            cout << " is NOT available\nInput Error\nExiting..." << endl;
            exit(1);
        }
    }
    input.close();
    if (num_inp_routines == 0) {
        cout << "You have no routines in your Input File!!\nExiting..." << endl;
        exit(1);
    }
    return num_inp_routines;
}

bool is_in_array( string check,  string *array, int num){
    for( int i=0; i < num; i++){
        if(check == array[i]){
            return true;
        }
    }
    return false;
}
