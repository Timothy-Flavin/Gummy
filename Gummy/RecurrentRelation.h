#pragma once
#include"Matrix.h"
struct recurrentRelation {
public:
	int inputLayer, outputLayer, timeDistance;
	Matrix weightMatrix, errorMatrix, errorMatrixBuffer;
};
