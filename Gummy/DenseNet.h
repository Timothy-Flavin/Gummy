#pragma once
#include"Matrix.h"
#include"CSV.h"
#include<iostream>
#include<fstream>
#include<cmath>
#include"NeuralNetwork.h"

class DenseNet : public NeuralNetwork {
protected:
	void sigmoid(Matrix* A);
	double sigmoidPrime(double a);
	DenseNet();
public:
	DenseNet(int nl, int*ll, bool so, char* nm);
	DenseNet(csv* file);
	Matrix* feedForward(Matrix* inputs);
	double calcError(Matrix* A);
	double getError() { return totalError; }
	void backProp(Matrix* A, double stepSize);
	void backPropOld(Matrix* A, double stepSize);
	void updateWeights(double stepSize, int batchSize);
	void print();
	void printGradient();
	int getNumInputs() { return layerList[0]; }
	char* getName() { return name; }
	void setName(char* nm);
	int getNumOutputs() { return layerList[numLayers - 1]; }
	void save();
};