#include <iostream>
#include <fstream>
using std::cout;
#include "Field.h"


int main(){
    

    double div= 20;
    double beta_0   = 0.5;
    double beta_f   = 1.5;
    int L =  20;
    int q =   3;
    int itts = 5000;
    
    
    
    double beta_itts   = (beta_f-beta_0)/div;
    Field<int> phi(L,L,q,0.5 );
 
    

    std::ofstream outfile ("beta_vs_M.csv");
    outfile << "beta" <<","<< "<M>"<< std::endl;

    for(double beta = beta_0; beta<=beta_f; beta+=beta_itts){

     
        Field<int> phi(L,L,q,beta);
        phi.itterate(itts);

      
        outfile << beta << "," <<phi.frac_mag() << std::endl;

    }
outfile.close();
    
    return 0;
    
}
