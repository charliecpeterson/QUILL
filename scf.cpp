#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>
#include <Eigen/Eigenvalues>
#include <Eigen/Core>
#include <Eigen/Dense>
using namespace std;
using namespace Eigen;
using std::sqrt;
#define INDEX(i,j) i>j ? i*(i+1)/2 + j : j*(j+1)/2 + i
#define MAX_ITER 75
#define EDIFF_CONV 0.000000000002
#define RMS_CONV 0.000000000014
double rmsDensity(MatrixXd A, MatrixXd B, int mat_size){
    double result = 0;
    for (int i =0 ; i < mat_size; i++){
        for(int j =0; j < mat_size; j++){
            result += pow(A(i,j) - B(i,j),2);
        }
    }
    return pow(result,0.5);
}

int main() {
    cout << "Welcome to my Awesome and simple SCF program" << endl;
//creating variables/objects
    int num_basisf = 7;
    int occ = 5;
    int mat_length = num_basisf * (num_basisf + 1) / 2 ;
    int total_mat_length = num_basisf * num_basisf;
    double enuc, rms, Ediff;
    double e0, eX, eC, etotal =0;
    MatrixXd t_array(num_basisf,num_basisf);
    MatrixXd v_array(num_basisf,num_basisf);
    MatrixXd s_array(num_basisf,num_basisf);
    MatrixXd hcore(num_basisf,num_basisf);
    MatrixXd F0(num_basisf,num_basisf); 
    MatrixXd FC(num_basisf,num_basisf);
    MatrixXd FX(num_basisf,num_basisf);
    MatrixXd AO_eigenvectors(num_basisf,num_basisf);
    MatrixXd D0(num_basisf,num_basisf);
    MatrixXd DC(num_basisf,num_basisf);
    MatrixXd DX(num_basisf,num_basisf);
    MatrixXd FX_org(num_basisf,num_basisf);
    MatrixXd FC_org(num_basisf,num_basisf);
    VectorXd eri_array(1000); 

 // Reading variables
 // Nuclear repulsion energy: enuc.dat --> enuc
 // Kinetic energy inegrals: t.dat --> t_array
 // Nuclear attraction inegrals: v.dat --> v_array
 // AO Overlap: s.dat --> s_array
 // Core Hamiltonian: hcore

 // Reading enuc 
    ifstream enucfile ("data/enuc.dat");
    enucfile >> enuc;

//  Reading One-Electron Inegrals


    ifstream tfile ("data/t.dat");
    ifstream vfile ("data/v.dat");
    ifstream sfile ("data/s.dat");
    string newline;
    while(getline(tfile, newline)){
        int i,j;
        double temp_t; 
        istringstream ss(newline);
        ss >> i >> j >> temp_t;
        t_array(i-1,j-1) = temp_t;
        if(i != j ) t_array(j-1,i-1) = temp_t;
    }

    while(getline(vfile, newline)){
        int i,j;
        double temp_v; 
        istringstream ss(newline);
        ss >> i >> j >> temp_v;
        v_array(i-1,j-1) = temp_v;
        if(i != j ) v_array(j-1,i-1) = temp_v;
    }
    while(getline(sfile, newline)){
        int i,j;
        double temp_s; 
        istringstream ss(newline);
        ss >> i >> j >> temp_s;
        s_array(i-1,j-1) = temp_s;
        if(i != j ) s_array(j-1,i-1) = temp_s;
    }
//  Core Hamiltonian hcore;
//  Hcore = T + V
        hcore = t_array + v_array;

//  Read Two Electron repulsion data
//  eri.dat > eri_array
    ifstream erifile ("data/eri.dat");
    while(getline(erifile, newline)){
        int i, j, ij, k, l, kl, ijkl;
        double temp_eri; 
        istringstream ss(newline);
        ss >> i >> j >> k >> l >> temp_eri;
        ij = INDEX(i,j);
        kl = INDEX(k,l);
        ijkl = INDEX(ij,kl);
        eri_array(ijkl) = temp_eri;

    }

//  Orthogonlization matrix
// V * D^-1/2 * V^-1
    SelfAdjointEigenSolver<MatrixXd> s_eigen(s_array);

//  Initial Guess of Wavefunction
    F0 = s_eigen.operatorSqrt().inverse().transpose() * hcore * s_eigen.operatorSqrt().inverse();
    SelfAdjointEigenSolver<MatrixXd> F_eigen(F0);
    AO_eigenvectors = s_eigen.operatorSqrt().inverse() * F_eigen.eigenvectors();
    for ( int i = 0 ; i < num_basisf ; i++){
        for ( int j = 0 ; j < num_basisf; j++){
            for (int k = 0 ; k < occ ; k++){
               D0(i,j) +=  AO_eigenvectors.col(k).row(i) * AO_eigenvectors.col(k).row(j);
            }
        }
    } 
//  Compute Inital Hartree Fock Energy
    for (int i =0 ; i < num_basisf; i++){
        for (int j =0 ; j < num_basisf; j++){
            e0 += D0(i,j) * (hcore(i,j) + hcore(i,j));
        }
    }
    etotal = e0 + enuc;
    DC = DX = D0;
    eC = eX = e0;
    cout << setw(10) << "Iter";
    cout << setw(25)<< setprecision(15) << "E(elec)";
    cout <<setw(25) << setprecision(15) << "D(E)" ;
    cout << setw(25) << setprecision(15) << "RMS(D)" << endl;
    cout << setw(10) << "0";
    cout << setw(25)<< setprecision(15) << eC;
    cout <<setw(25) << setprecision(15) << "0.00";
    cout << setw(25) << setprecision(15) << "0.00" << endl;
// enter into SCF loop
for (int iter = 1 ; iter <= MAX_ITER; iter++){
// Build new Fock Matrix
    for ( int i =1; i <= num_basisf; i++){
        for (int j =1 ; j <= num_basisf; j++){
            FC(i-1,j-1) = hcore(i-1,j-1);
            for(int k =1; k <= num_basisf; k++){
                for( int l=1; l <= num_basisf; l++){
                    int ij = INDEX(i,j);
                    int kl = INDEX(k,l);
                    int ijkl = INDEX(ij,kl);
                    int ik = INDEX(i,k);
                    int jl = INDEX(j,l);
                    int ikjl = INDEX(ik,jl);
                    FC(i-1,j-1) += DC(k-1,l-1) * ( 2.0 * eri_array(ijkl) - eri_array(ikjl));   
                }
            }
        }
    }

//  Build Density Matrix
    FC_org = s_eigen.operatorSqrt().inverse().transpose() * FC * s_eigen.operatorSqrt().inverse();
    SelfAdjointEigenSolver<MatrixXd> FC_eigen(FC_org);
    MatrixXd AOC_eigenvectors(num_basisf,num_basisf);
    AOC_eigenvectors = s_eigen.operatorSqrt().inverse() * FC_eigen.eigenvectors();
    for ( int i = 0 ; i < num_basisf ; i++){
        for ( int j = 0 ; j < num_basisf; j++){
            DC(i,j)= 0;
            for (int k = 0 ; k < occ ; k++){
               DC(i,j) +=  AOC_eigenvectors.col(k).row(i) * AOC_eigenvectors.col(k).row(j);
            }
        }
    }  


//  Compute new Hartree Fock Energy
    eC = 0;
    for (int i =0 ; i < num_basisf; i++){
        for (int j =0 ; j < num_basisf; j++){
            eC += DC(i,j) * ( hcore(i,j) + FC(i,j));
        }
    }
    Ediff = eC - eX;
    rms = rmsDensity(DC,DX,num_basisf);
    cout << setw(10) << iter;
    cout << setw(25)<< setprecision(15) << eC;
    cout <<setw(25) << setprecision(15) << Ediff;
    cout << setw(25) << setprecision(15) << rms << endl;

    if (Ediff < EDIFF_CONV && rms < RMS_CONV){
        cout << "CONVERGENCE has been reached" << endl;
        break;
    }else if(iter == MAX_ITER){
        cout << "NO CONVERGENCE has been reached" << endl;
    }
    //Save current information for next loop
    eX = eC;
    DX = DC;

} //end of SCF Loop
    cout << "Thank you for using this SCF program" << endl;
    return 0;
} 


