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
	int gateType = 1;
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
	static void sigmoid(Matrix* A);
	static double sigmoidPrime(double a);
	static void tanh(Matrix* A);
	static double tanhPrime(double a);
	static void relu(Matrix* A);
	static double reluPrime(double a);
	static void leakyRelu(Matrix* A);
	static double leakyReluPrime(double a);
	NeuralNetwork();
	void (*gate)(Matrix*)=&tanh;// = &NeuralNetwork::sigmoid;
	double (*gatePrime)(double)=&tanhPrime;// = &NeuralNetwork::sigmoidPrime;
	//int jerry = 0;
public:
	//NeuralNetwork(int nl, int*ll, bool so, char* nm);
	//NeuralNetwork(csv* file);
	virtual void setGateType(int);
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