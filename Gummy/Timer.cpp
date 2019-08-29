#include"Timer.h"
#include<iostream>
Timer::Timer() {
m_StartTimepoint = std::chrono::high_resolution_clock::now();
}
Timer::~Timer(){
	Stop();
}
void Timer::Stop(){
	auto endTimePoint = std::chrono::high_resolution_clock::now();
	auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

	auto duration = end - start;
	double ms = duration * 0.001;

	std::cout << duration << "microseconds, " << ms << "milliseconds" << std::endl;
}

/*
void DenseNet::backPropOld(Matrix* A, double stepSize) {
	//error*S()*(1-S())
	
	//1: set last row of eActivations through calcError
	//Loop for num layers{
	//2: set ebias with inverseSigmoid(activations[i]) * eActivation[i]
	//3: set each w[i] by corrisponding ebias[i]*activations[i] (row before the weights row)
	//4: set next eActivations via transpose multiply weights[i]*activations[i+1]
	//}
	
	//std::cout << "\n-----PRINTING BACK PROP DEBUG------\n";
	for (int i = 0; i < numLayers; i++) {
		for (int j = 0; j < eActivation[i].getM(); j++)
			eActivation[i].set(j, 0, 0);
	}
	double error = calcError(A);
	//std::cout << "\n-----ERROR SET, PERFORMING BACKPROP------\n";
	for (int curLayer = numLayers - 2; curLayer >= 0; curLayer--) {
		//std::cout << "\nsetting eBias for layer: " << curLayer << std::endl;
		for (int i = 0; i < eBias[curLayer].getM(); i++) {
			if (curLayer == numLayers - 2 && !sigmoidOutput) {
				eBias[curLayer].set(i, 0, eActivation[curLayer + 1].get(i, 0));
			}
			else {
				eBias[curLayer].set(i, 0, eActivation[curLayer + 1].get(i, 0)*sigmoidPrime(activations[curLayer + 1].get(i, 0)));
			}
		}
		//eBias[curLayer].print();
		//std::cout << "\nsetting weights and prevActivations for layer: " << curLayer << std::endl;
		//weights column applied to previous activations row
		for (int i = 0; i < eWeights[curLayer].getM(); i++) {
			for (int j = 0; j < eWeights[curLayer].getN(); j++) {
				//std::cout << "\nline 75";
				eWeights[curLayer].set(i, j, eBias[curLayer].get(i, 0) * activations[curLayer].get(j, 0));
				//std::cout << "\nline 77";
				eActivation[curLayer].set(j, 0, eActivation[curLayer].get(j, 0) + weights[curLayer].get(i, j) * eBias[curLayer].get(i, 0));
				//std::cout << "\nline 79";
			}
		}
	}
	//double stepSize = 0.0005;
	for (int l = 0; l < numLayers - 1; l++) {
		for (int i = 0; i < weights[l].getM(); i++) {
			for (int j = 0; j < weights[l].getN(); j++) {
				weights[l].set(i, j, weights[l].get(i, j) - stepSize * (eWeights[l].get(i, j)));
			}
		}
		for (int i = 0; i < bias[l].getM(); i++) {
			bias[l].set(i, 0, bias[l].get(i, 0) - stepSize * (eBias[l].get(i, 0)));
		}
	}
	//std::cout << "\nE: " << error;
}
*/