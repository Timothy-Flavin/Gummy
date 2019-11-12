#pragma once
#include"RecurrentRelation.h"
#include"DenseNet.h"
#include"Matrix.h"
#include"CSV.h"
#include<iostream>
#include<fstream>
#include<cmath>
class RecurrentNet : public DenseNet {
protected:
	Matrix tWeight = Matrix();
	Matrix etWeight = Matrix();
	Matrix etWeightBuffer = Matrix();
	Matrix**activationHistory = nullptr; //This needs to be a history of all the hidden layers of length truncated timesteps
	//also if a hidden layer is used only every 'x' number of times it should hold truncated number of those so if 
	//one only looks at hidden layer 2 every 4 steps and truncated time = 200 then it should hold up to the layer 2
	//from 800 timesteps ago
	int truncatedTime = 0;
	int numRelations = 0, truncNum = 0;
	recurrentRelation** relations = nullptr;
	/*double totalError = 0;
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
	*/
public:
	RecurrentNet(int nl, int*ll, bool so, char* nm, int riln, int roln, int truncNum); // depricated
	RecurrentNet(int nl, int*ll, bool so, char* nm, recurrentRelation** relations, int numRelations, int truncNum);
	RecurrentNet(csv* file);
	
	Matrix* feedForward(Matrix* inputs);
	//double calcError(Matrix* A);
	//double getError() { return totalError; }
	void backProp(Matrix* A, double stepSize);
	//void backPropOld(Matrix* A, double stepSize);
	void updateWeights(double stepSize, int batchSize);
	void sortRelations(recurrentRelation** relations, int numRelations);
	void printRelations(recurrentRelation** relations, int numRelations);
	void print();
	void printGradient();
	//int getNumInputs() { return layerList[0]; }
	//char* getName() { return name; }
	//void setName(char* nm);
	//int getNumOutputs() { return layerList[numLayers - 1]; }
	void save();
};