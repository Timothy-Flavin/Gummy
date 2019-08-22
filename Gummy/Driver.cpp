#include "Gummy.h"
#include<string>
#include<chrono>
#define testMatrix
#define testDenseNet
#define testGummy
class Timer {
public:
	Timer() {
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}
	~Timer() {
		Stop();
	}
	void Stop() {
		auto endTimePoint = std::chrono::high_resolution_clock::now();
		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

		auto duration = end - start;
		double ms = duration * 0.001;

		std::cout << duration << "microseconds, " << ms << "milliseconds"<<std::endl;
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
};
int main(){

	srand(time(0));
	std::cout<<"Default Driver debugging program, shows each part of Gummy working correctly comment out the parts you do not want to print"<<std::endl;

#ifdef testMatrix
	std::cout << "Constructing sample Matrices 'm' and 'n'. 'm' with constructor and 'n' with '.construct'" << std::endl;
	Matrix m(2, 3);
	Matrix n;
	n.construct(2, 3);
	std::cout << "m.print(): " << std::endl;
	m.print();
	std::cout << "n.print(): " << std::endl;
	n.print();

	m.fillRandDouble(0, 1);
	n.fillRandDouble(1, 2);
	std::cout << "'m' filled with doubles from 0 to 1: " << std::endl;
	m.print();
	std::cout << "'n' filled with doubles from 1 to 2: " << std::endl;
	n.print();
	std::cout << "reconstructing n to be a 3 by 2 and setting random double" << std::endl;
	n.construct(3, 2);
	n.fillRandDouble(1, 2);
	n.print();
	for (int i = 0; i < 10; i++) {
		{
			Timer timer;
			std::cout << "Testing matrix multiplication performance..." << std::endl;
			m.construct(400, 600);
			m.fillRandDouble(1, 5);
			std::cout << "m constructed 400x600" << std::endl;
			n.construct(600, 500);
			n.fillRandDouble(5, 10);
			std::cout << "n constructed 600x500" << std::endl;
		}
		{
			Timer timer;
			Matrix o(400, 500);
			m.multiply(&n, &o);
			std::wcout << "m*n = o" << std::endl;
		}
	}
#endif
	std::cout<<"done and authored by Timothy-Flavin"<<std::endl;
	std::cin.get();
    return 0;
}