#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<cmath>
#include"Matrix.h"
#include"DenseNet.h"
#include"RecurrentNet.h"
#include"NeuralNetwork.h"
#include"CSV.h"
#include"RecurrentRelation.h"

class Gummy {
    private:
        csv* trainingData;
        int numIterations=0;
        double stepSize=0;
        int type=0;
        char* netFileName;
        char* csvFileName;
    public:
        Gummy();
        NeuralNetwork* userInit();
        DenseNet* manualInitDense(char* fileName, char* nFileName, int ntype, int numLayers, int* layerSizes, bool sigmoid);
		RecurrentNet* manualInitRecurrent(char* fileName, char* nFileName, int numLayers, int* layerSizes, bool sigmoid, recurrentRelation** relations, int numRelations, int truncNum);
        void train(NeuralNetwork* net, int batchSize);
		void train(NeuralNetwork* net);
        void saveNet(NeuralNetwork* net);
        DenseNet* loadNet(char* fileName);
        void setNetFileName(char* nfn){netFileName = nfn;}
        void setCsvFileName(char* cfn){csvFileName = cfn;}
        csv* readCSV(char* fileName);
        void csvToDouble(csv* file);
        void updateTrainingData(bool);
        void setNumIterations(int n){numIterations=n;}
        void setStepSize(double n){stepSize=n;}
};