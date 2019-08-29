#pragma once
#include"Matrix.h"
#include"CSV.h"
#include<iostream>
#include<fstream>
#include<cmath>

class DenseNet {
private:
    char* name = NULL;
	bool sigmoidOutput = true;
	int numLayers = NULL;
	int*layerList = NULL;
	Matrix* weights = NULL;
	Matrix* bias = NULL;
	Matrix* activations = NULL;
	Matrix* eWeights = NULL;
	Matrix* eBias = NULL;
	Matrix* eBiasBuffer = NULL;
	Matrix* eActivation = NULL;
	double* errorList = NULL;
	void sigmoid(Matrix* A);
	double sigmoidPrime(double a);
public:
	DenseNet(int nl, int*ll, bool so, char* nm);
	DenseNet(csv* file);
	Matrix* feedForward(Matrix* inputs);
	double calcError(Matrix* A);
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