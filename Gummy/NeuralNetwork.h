#pragma once
#pragma once
#include"RecurrentRelation.h"
#include"Matrix.h"
#include"CSV.h"
#include<iostream>
#include<fstream>
#include<cmath>

class NeuralNetwork {
protected:
	double totalError = 0;
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
	virtual void sigmoid(Matrix* A);
	virtual double sigmoidPrime(double a);
	NeuralNetwork();
	//int jerry = 0;
public:
	//NeuralNetwork(int nl, int*ll, bool so, char* nm);
	//NeuralNetwork(csv* file);
	virtual Matrix* feedForward(Matrix* inputs);
	virtual double calcError(Matrix* A);
	virtual double getError() { return totalError; }
	virtual void backProp(Matrix* A, double stepSize);
	virtual void backPropOld(Matrix* A, double stepSize);
	virtual void updateWeights(double stepSize, int batchSize);
	
	virtual void sortRelations();//recurrentRelation** relations, int numRelations);
	virtual void printRelations();//recurrentRelation** relations, int numRelations);
	
	virtual void print();
	virtual void printGradient();
	virtual int getNumInputs() { return layerList[0]; }
	virtual char* getName() { return name; }
	virtual void setName(char* nm);
	virtual int getNumOutputs() { return layerList[numLayers - 1]; }
	virtual void save();
};