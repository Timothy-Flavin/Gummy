#pragma once
#include"Matrix.h"
struct recurrentRelation {
public:
	int inputLayer, outputLayer, timeDistance;
	Matrix weightMatrix, errorMatrix, errorMatrixBuffer, biasMatrix, errorBiasMatrix, errorBiasMatrixBuffer, mostRecentActivation;
	Matrix* activationHistory = nullptr;
};
