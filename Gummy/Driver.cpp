#include "Gummy.h"
#include<string>
#define testMatrix
#define testDenseNet
#define testGummy
	
int main(){
	srand(time(0));
	std::cout<<"Default Driver debugging program, shows each part of Gummy working correctly comment out the parts you do not want to print"<<std::endl;

	#ifdef testMatrix

	std::cout<<"Constructing sample Matrices 'm' and 'n'. 'm' with constructor and 'n' with '.construct'"<<std::endl;
	Matrix m(2,3);
	Matrix n;
	n.construct(2,3);
	std::cout<<"m.print(): "<<std::endl;
	m.print();
	std::cout<<"n.print(): "<<std::endl;
	n.print();

	m.fillRandDouble(0,1);
	n.fillRandDouble(1,2);
	std::cout<<"'m' filled with doubles from 0 to 1: "<<std::endl;
	m.print();
	std::cout<<"'n' filled with doubles from 1 to 2: "<<std::endl;
	n.print();
	std::cout<<"reconstructing n to be a 3 by 2 and setting random double"<<std::endl;
	n.construct(3,2);
	n.fillRandDouble(1,2);
	n.print();

	#endif

	std::cout<<"done and authored by Timothy-Flavin"<<std::endl;
	std::cin.get();
    return 0;
}