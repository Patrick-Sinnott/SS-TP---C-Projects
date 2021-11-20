#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <iostream> 
#include <algorithm> 

using namespace std; 

using std::cout;


template <class T>
class Field
{
  private:
    
    int              nx_;
    int              ny_;
    double         beta_;
    T            action_;
    T                 q_;
    std::vector<T> data_;

    int index_(int x, int y) const { return (x+nx_) % nx_ + nx_*((y+ny_) % ny_); }
    
    void update_ij_(int i, int j){

       
        T random_q = data_[index_(i,j)];
        while(random_q==data_[index_(i,j)]) random_q = 1 + ( std::rand() % ( q_ ) );
        
        T proposed_action = action_ij(            random_q,
                                      data_[index_(i+1,j  )],
                                      data_[index_(i-1,j  )],
                                      data_[index_(i  ,j+1)],
                                      data_[index_(i  ,j-1)] );
        
        T  current_action = action_ij(data_[index_(i  ,j  )],
                                      data_[index_(i+1,j  )],
                                      data_[index_(i-1,j  )],
                                      data_[index_(i  ,j+1)],
                                      data_[index_(i  ,j-1)] );
        
        double change_in_action = proposed_action - current_action;
        double accept_prob = exp( - beta_ * (change_in_action) );
        double random_prob = ((double) rand() / (RAND_MAX)); //Random number in (0,1)
        
       
        if(accept_prob >= 1 ){
            data_[ index_(i, j) ] = random_q;
            action_ += change_in_action;
            
        }
       
        else if (random_prob<=accept_prob){
            data_[ index_(i, j) ] = random_q;
            action_ += change_in_action;
        }
    }
    
    
    //Returns the count of the most frequently-occuring spin
    double f_() {
    
        std::vector<int> count(q_); // Keep track of occurences of each spin
        for(int i=0; i<q_; i++) count[i] = 0; //initialise to 0
        
        //For each grid point check what spin (i) it is and add it to 'count[i]'
        for(int j = 0; j < ny_; j++){ for(int i = 0; i < nx_; i++){
                for(int test_q = 1; test_q <= q_; test_q++){
                    if(test_q == data_[index_(i,j)] ){ count[test_q-1]++;}
                }
            }
        }
        
        //Return the max value 
        return *std::max_element(count.begin(), count.end())/(nx_*ny_+0.);
    }
    
    
    
    
  public:
    
    //Randomly initialising field
    Field(int nx, int ny, int q, double beta) : nx_(nx), ny_(ny), data_(nx_*ny_), q_(q), beta_(beta){
        int n=nx_*ny_;
        for (int i=0;i<n;i++) data_[i] = 1 + ( std::rand() % ( q ) ); //Random initialisation in [1,..,q]

        //Loop over entire grid, and calculate action
        for(int j =0; j<ny_; j++) {
            for(int i =0; i<nx_; i++)action_+=action_ij(data_[index_(i,  j  )],
                                                        data_[index_(i+1,j  )],
                                                        data_[index_(i-1,j  )],
                                                        data_[index_(i  ,j+1)],
                                                        data_[index_(i  ,j-1)]); }
    
    }
   
    
    //Creates an operator which calls the field like phi(a,b)
    T  operator() (int x, int y) const { return data_[index_(x,y)]; }
    T& operator() (int x, int y)       { return data_[index_(x,y)]; }

    
    //Return Field size nx , ny, action
    int     nx() const { return     nx_ ; }
    int     ny() const { return     ny_ ; }
    int action() const { return action_ ; }

    
    //Function which calculates the action at (i,j)
    T action_ij(T ij, T i_p1, T i_m1, T j_p1, T j_m1) const {
        
        T S = 0; // Action at (i,j)
        
        //if the spins dont agree -> add one
        if( ij != i_p1){ S++;}; if( ij != i_m1){ S++;}; if( ij != j_p1){ S++;}; if( ij != j_m1){ S++;}
        
        return S;
    }
    
 
    //Printing grid
    void print_grid() const {
        
        cout<<"\n\n";
        for(int j =0; j<ny_; j++){
            for(int i =0; i<nx_; i++) cout<<data_[index_(i,j)]<<" "<<std::flush;
            cout<<"\n";
        }
        cout<<"\n\n";
    }
    
    
    // Itterate over its
    void itterate(int its){
        
        //Loop over its and every site i,j
        for(int k =0; k<its; k++) for(int j =0; j<ny_; j++) for(int i =0; i<nx_; i++) update_ij_(i, j);
    }

        

    //fractional magnetisation of a spin configuration
    double frac_mag() { return (q_*f_() - 1. )/(q_ - 1.); }
    

};


