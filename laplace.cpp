#include <iomanip>
#include <vector>
#include <math.h>
#include <tgmath.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


class Field
{
private:
	int							qx_;
	int							qy_;
	std::vector<double>         data_; 
	int index_(int x, int y) const { return x + qx_*y;}
public:
	Field(int nx, int ny) : qx_(nx+1), qy_(ny+1), data_(qx_*qy_)
	{
		int n=qx_*qy_;
		for (int i=0;i<n;i++) data_[i] = 0.0;
	}
	
	double& operator()(int x, int y) { return data_[index_(x,y)];}
	double operator()(int x, int y) const { return data_[index_(x,y)];}

	int nx() const { return qx_ -1;}
	int ny() const { return qy_ -1;}
};

double derivative(Field phi, int x, int y)
	{
		return  (phi(x,y+1)  - phi(x,y-1) )/2;
	}

int main()
{
	Field phi(100,100); 
	double omega = 1.75;

	
	
	          ///Code for writing a CSV output file of trajectories
    std:: ofstream outfile ("Heat.txt"); //Writing a CSV file for plotting
    

	for (int x=0; x<101; x++)	//These are all Boundary conditions
	{
		for (int y=0; y<101; y++)
		{
			phi(x,y)=0;
		}
	}



	for(int x=50; x<81; x++)
	{
		phi(x,70) = -1;
	}

	for(int y=30; y<71; y++)
	{
		phi(80,y) = -1;
	}

	for(int x=20;x<41;x++)
	{
		phi(x,10) = 2;
		phi(x,30) = 2;
	}

	for (int y=10;y<31;y++)
	{
		phi(20,y) = 2;
		phi(40,y) = 2;
	}


	
for(int i=0;i<1000;i++)  //This is where we update the remaining points on the grid
{
	for (double y=1;y<100;y++) 
	{
		for(double x=1;x<100;x++)
		{
			if ( !(x>49 && y==70 && x<81) && !(x==80 && y>29 && y<71 ) && !(x==20 && y>9 && y<31) && !(x==40 && y>9 && y<31) && !(x>19 && x<41 && y==10) && !(x>19 && x<41 && y==30))

			{
				phi(x,y)=(1-omega)*phi(x,y) + omega*0.25*(phi(x+1,y)+phi(x,y+1)+phi(x-1,y)+phi(x,y-1));
			}


		cout << "phi(x,y) = " << phi(x,y) << "\n";
		
	



		}
	}
	
}


	for (int y=0; y<101; y++)	
	{
		for (int x=0; x<101; x++)
		{
			outfile << phi(x,y) << " ";
		}
		outfile << ";" << endl;
	}

cout << "phi(20,70) = " << phi(20,70) << "\n";
cout << "derivative at (20,70) = " << derivative(phi,20,70) << "\n";

outfile.close();
return 0;

}