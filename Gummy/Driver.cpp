#include "Gummy.h"
#include "Timer.h"
#include<string>
//#define testMatrix
#define testDenseNet
//#define testRecurrentNet
inline bool exists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}
int main(){
	char yn = 'n';
	srand(time(0));
	std::cout<<"Default Driver debugging program, shows each part of Gummy working correctly comment out the parts you do not want to print"<<std::endl;

#ifdef testMatrix
	std::cout << "Constructing sample Matrices 'm' and 'n'. 'm' with constructor and 'n' with '.construct'" << std::endl;
	Matrix m(2, 3);
	Matrix n;
	n.construct(3, 3);
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
	std::cout << "Checking that multiplication is working as expected: " << std::endl;
	m.print();
	n.print();
	Matrix result(2, 2);
	m.multiply(&n, &result);
	result.print();
	std::cout << "Press enter to continue...";
	std::cin.get();
	for (int i = 0; i < 10; i++) {
		{
			Timer timer;
			std::cout << "Testing matrix multiplication performance..." << std::endl;
			m.construct(100, 100*(i+1));
			m.fillRandDouble(1, 5);
			std::cout << "m constructed 100x"<<i+1<<"00" << std::endl;
			n.construct(100*(i+1), 100);
			n.fillRandDouble(5, 10);
			std::cout << "n constructed "<<i+1<<"00x100" << std::endl;
		}
		{
			Timer timer;
			Matrix o(100, 100);
			m.multiply(&n, &o);
			std::wcout << "m*n = o" <<o.get(0,1)<< std::endl;
		}
	}
	std::cin.get();
#endif
#ifdef testDenseNet
	std::cout << "testing Dense Net with test cases"<<std::endl;
	Gummy gummy;

	char* csvFileName = new char[20];
	char* netFileName = new char[20];
	std::cout << "Enter test csv file name (t.csv): ";
	std::cin.getline(csvFileName, 20);
	bool fileError = exists(csvFileName);
	if (!fileError) {
		std::cout << "File does not exist fatal error, exiting program...";
		std::cin.get();
		return 0;
	}
	std::cout << "Enter test net file name: ";
	std::cin.getline(netFileName, 20);
	int type = 1, numLayers = 4;
	int layerSizes[] = { 2, 3, 2, 1 };
	std::cout << "creating a test neural net with 4 layers: 2, 3, 2, 1 and sigmoid output" << std::endl;
	NeuralNetwork* tnet = gummy.manualInitDense(csvFileName, netFileName, 1, numLayers, layerSizes, true);
	std::cout << "print net matrices? (y/n)" << std::endl;
	std::cin >> yn;
	if (yn == 'y') {
		tnet->print();
	}
	std::cout << "print gradient matrices? (y/n)" << std::endl;
	std::cin >> yn;
	if (yn == 'y') {
		tnet->printGradient();
	}
	gummy.saveNet(tnet);
	fileError = exists(netFileName);
	std::cout << "file exists: " << fileError;
	if (!fileError) {
		std::cout << "File does not exist fatal error, exiting program...";
		std::cin.get();
		return 0;
	}
	tnet = gummy.loadNet(netFileName);
	std::cout << "saved and re leaded test net" << std::endl;
	std::cout << "print new net matrices? (y/n)" << std::endl;
	std::cin >> yn;
	if (yn == 'y') {
		tnet->print();
	}
	std::cout << "print new gradient matrices? (y/n)" << std::endl;
	std::cin >> yn;
	if (yn == 'y') {
		tnet->printGradient();
	}
	{
		Timer timer;
		gummy.updateTrainingData(true);
		gummy.setNumIterations(200000);
		gummy.setStepSize(1);
		gummy.train(tnet);
	}
#endif

#ifdef testRecurrentNet
	std::cout << "Testing recurrent net with test cases" << std::endl;
	Gummy gummy;
	char* csvFileName = new char[20];
	char* netFileName = new char[20];
	std::cout << "Enter test csv file name (t.csv): ";
	std::cin.getline(csvFileName, 20);
	bool fileError = exists(csvFileName);
	if (!fileError) {
		std::cout << "File does not exist fatal error, exiting program...";
		std::cin.get();
		return 0;
	}
	gummy.setCsvFileName(csvFileName);
	NeuralNetwork* testNet = gummy.userInit(); //This net is recurrent and can be checked by it's type number
	testNet->printRelations();
#endif
	std::cout<<"done and authored by Timothy-Flavin"<<std::endl;
	std::cin.get();
	std::cin.get();
    return 0;
}

