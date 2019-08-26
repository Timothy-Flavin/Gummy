#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Matrix.h"

//creates a matrix pointer (Helpfull for if matrix is in an array)
Matrix::Matrix() {
	m = 0;
	n = 0;
	matrix = nullptr;
}
//creates a matrix with n1 rows and n2 columns
Matrix::Matrix(int n1, int n2) {
	m = n1;
	n = n2;
	matrix = new double[m*n];
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			matrix[i*n+j] = 0.0;
	}
}
//constructs a previously null or previously instantiated matrix
void Matrix::construct(int n1, int n2) {
	delete[] matrix;
	m = n1;
	n = n2;
	matrix = new double[m*n];
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			matrix[i*n+j] = 0;
	}
}
//fills the matrix with random doubles at integer values between min and max
void Matrix::fillRandInt(int min, int max) {
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			matrix[i*n+j] = (rand() % (max - min) + min) * 1.0;
}
//fills matrix with random double values between min and max
void Matrix::fillRandDouble(double min, double max) {
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			matrix[i*n+j] = (double)rand() / RAND_MAX *(max-min)+min;
}
//Multiplies this matrix with matrix A and returns a new matrix pointer
//be wary of memory leaks if this function is used for the same pointer
//more than once
Matrix* Matrix::multiply(Matrix* A) {
	Matrix *tempM = new Matrix(m, A->getN());
	if (n != A->getM()) {
		std::cout << "No matchy no multiply bruh";
	}
	else if (A->get(0, 0) == NULL) {
		std::cout << "null shit bruh";
	}
	else {
		for (int i = 0; i < m; i++)
			for (int j = 0; j < A->getN(); j++) {
				double val = 0;
				for (int k = 0; k < n; k++) {
					val += matrix[i*n+k] * A->get(k, j);
				}
				tempM->set(i, j, val);
			}
	}
	return tempM;
}
//safer method of this * A in which an already created matrix B is passed
//and modified. B must be A's M by Bs N as per matrix multiplication rules
void Matrix::multiply(Matrix* A, Matrix* B) {
	if (n != A->getM() || m!=B->getM()||A->getN()!=B->getN()) {
		std::cout << "No matchy no multiply bruh";
	}
	else if (A->get(0, 0) == NULL && A->get(0,0) != 0) {
		std::cout << "null shit bruh";
		A->print();
	}
	else {
		for (int i = 0; i < m; i++)
			for (int j = 0; j < A->getN(); j++) {
				double val = 0;
				for (int k = 0; k < n; k++) {
					val += matrix[i*n+k] * A->get(k, j);
				}
				B->set(i, j, val);
			}
	}
}
Matrix::~Matrix() {
	delete[] matrix;
}
//multiplies all matrix entries by d and returns a pointer to the new matrix
Matrix* Matrix::multiply(double d) {
	Matrix* tempM = new Matrix(m, n);
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			tempM->set(i, j, matrix[i*n+j] * d);
	return tempM;
}
//multiplies all matrix entries by n1 and returns a pointer to the new matrix
Matrix* Matrix::multiply(int n1) {
	Matrix* tempM = new Matrix(m, n);
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			tempM->set(i, j, matrix[i*n+j] * n1);
	return tempM;
}
//multiplies all matrix entries by f and returns a pointer to the new matrix
Matrix* Matrix::multiply(float f) {
	Matrix* tempM = new Matrix(m, n);
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			tempM->set(i, j, matrix[i*n+j] * f);
	return tempM;
}
//Adds 2 matrices together and returns a pointer to theresult
Matrix* Matrix::add(Matrix* A) {
	Matrix* tempM = new Matrix(m, n);
	if (m == A->getM() && n == A->getN()) {
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
				tempM->set(i, j, matrix[i*n+j] + A->get(i, j));
	}
	else {
		std::cout << "Not addable";
	}
	return tempM;
}
//adds 2 matrices together at the supplied pointer to protect from 
//memory leaks. both matrices must be m by n size
void Matrix::add(Matrix* A, Matrix* B) {
	if (m == A->getM() && n == A->getN()) {
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
				B->set(i, j, matrix[i*n+j] + A->get(i, j));
	}
	else {
		std::cout << "Not addable";
	}
}
//print's the matrix details
void Matrix::print() {
	std::cout << "\nm: " << m <<std::endl;
	std::cout << "n: " << n << std::endl;
	std::cout << "matrix: " << matrix << std::endl;
	if (matrix == NULL) {
		std::cout << "tf?";
	}
	else {
		for (int i = 0; i < m; i++) {
			std::cout << std::endl;
			for (int j = 0; j < n; j++)
				if(matrix[i*n+j] != NULL)
					std::cout << " " << matrix[i*n+j] << " ";
		}
		std::cout << std::endl;
	}
}

