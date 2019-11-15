#include"NeuralNetwork.h"
/*Creates a dense neural net with a number of layers "nl" and a pointer to an
array or layer sizes "ll". bool so decides whether the output is sigmoid (0-1)
or raw (Any double). nm is the name which the neural net will use when saving
itself to a file
*/

//given a csv file loads a dense net into ram for use.
NeuralNetwork::NeuralNetwork() {
	std::cout << "made a virtual default Neural Net" << std::endl;
}

Matrix* NeuralNetwork::feedForward(Matrix* inputs) {
	std::cout << "This is virtual feedforward from NeuralNetwork"<<std::endl;
	return inputs;
}
// make a back prop that does stocastic gradient descent with samples of data rather than 1 as an option
void NeuralNetwork::backProp(Matrix* A, double stepSize) {
	std::cout << "This is virtualBackprop from NeuralNetwork" << std::endl;
}
void NeuralNetwork::updateWeights(double stepSize, int batchSize) {
	std::cout << "This is virual Update weights from NerualNetwork" << std::endl;
}
void NeuralNetwork::backPropOld(Matrix* A, double stepSize) {
	std::cout << "This is old back prop virtual in NeuralNetwork" << std::endl;
}
double NeuralNetwork::calcError(Matrix* A) {
	std::cout << "This is the virtual calcError in NeuralNetwork" << std::endl;
	return 0.0;
}
/*void NeuralNetwork::sigmoid(Matrix* A) {
	std::cout << "This is the virtual sigmoid in NeuralNetwork" << std::endl;
}
double NeuralNetwork::sigmoidPrime(double a) {
	std::cout << "This is the virtual sigmoid prime in NeuralNetwork" << std::endl;
	return 0.0;
}*/
void NeuralNetwork::setGateType(int gateType) {
	if (gateType == 0) {
		gate = &sigmoid;
		gatePrime = &sigmoidPrime;
	}
	else if (gateType == 1) {
		gate = &tanh;
		gatePrime = &tanhPrime;
	}
	else if (gateType == 2) {
		gate = &relu;
		gatePrime = &reluPrime;
	}
	else if (gateType == 3) {
		gate = &leakyRelu;
		gatePrime = &leakyReluPrime;
	}
}
void NeuralNetwork::sigmoid(Matrix* A) {
	//derivative of sigmoid S() is S()*(1-S())
	for (int i = 0; i < A->getM(); i++) {
		A->set(i, 0, 1.0 / (1.0 + exp(-1 * (A->get(i, 0)))));
	}
}
double NeuralNetwork::sigmoidPrime(double a) {
	return a * (1 - a);
}
void NeuralNetwork::tanh(Matrix* A) {
	//std::cout << "tanh bitch" << std::endl;
	for (int i = 0; i < A->getM(); i++) {
		A->set(i, 0, 2.0 / (1.0 + exp(-2 * (A->get(i, 0))))-1);
	}
}
double NeuralNetwork::tanhPrime(double a) {
	return 1 - a * a;
}
void NeuralNetwork::relu(Matrix* A) {
	for (int i = 0; i < A->getM(); i++) {
		A->set(i, 0, A->get(1,0)<0?0:A->get(i,0)); //0 if a negative and 1 if positive
	}
}
double NeuralNetwork::reluPrime(double a) {
	return a < 0 ? 0 : 1;
}
void NeuralNetwork::leakyRelu(Matrix* A) {
	for (int i = 0; i < A->getM(); i++) {
		A->set(i, 0, A->get(1, 0)<0 ? 0.001*A->get(i, 0) : A->get(i, 0)); //0 if a negative and 1 if positive
	}
}
double NeuralNetwork::leakyReluPrime(double a) {
	return a < 0 ? 0.001 : 1;
}
void NeuralNetwork::print() {
	std::cout << "This is virtual print in NeuralNetwork" << std::endl;
}
void NeuralNetwork::printGradient() {
	std::cout << "This is the virtual print Gradient in NeuralNetwork" << std::endl;
}
void NeuralNetwork::save() {
	std::cout << "This is the virtual save in NeuralNetwork" << std::endl;
}
void NeuralNetwork::setName(char* nm) {
	std::cout << "This is the virtual setName in NeuralNetwork" << std::endl;
}
void NeuralNetwork::sortRelations(){//recurrentRelation** relations, int numRelations) {
	std::cout << "This is the virtual SortRelations in NeuralNetwork" << std::endl;
}
void NeuralNetwork::printRelations(){//recurrentRelation** relations, int numRelations) {
	std::cout << "This is the virtual printRelations in NeuralNetwork" << std::endl;
}