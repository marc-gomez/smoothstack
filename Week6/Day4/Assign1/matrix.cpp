#include <iostream>
#include <cstdlib>
#include <cassert>
#include <type_traits>

using namespace std;

#define MAX_ROWS 10
#define MAX_COLS 10
#define MAX_DIM MAX_ROWS * MAX_COLS

template<typename T>
void randomizeMatrix(T ** mat, const int dimension) {
	for (int i = 0; i < dimension; ++i) {
		for (int j = 0; j < dimension; ++j) {
			mat[i][j] = rand() % 10;
		}
	}
}

template<typename T>
void matrixMultiply(T ** matrixA, T ** matrixB, T ** matrixC, const int dimension) {
	assert(is_arithmetic_v<T>);

	T flatA[MAX_DIM];
	T flatB[MAX_DIM];

	#pragma omp parallel for
	for (int i = 0; i < dimension; i++) {
		for (int j = 0; j < dimension; j++) {
			flatA[i * dimension + j] = matrixA[i][j];
			flatB[j * dimension + i] = matrixB[i][j];
		}
	}

	#pragma omp parallel shared(matrixC) num_threads(40)
	{
		#pragma omp for schedule(static)
		for (int i = 0; i < dimension; i++) {
			int iOff = i * dimension;
			for (int j = 0; j < dimension; j++) {
				int jOff = j * dimension;
				T tot = 0;
				for (int k = 0; k < dimension; k++) {
					tot += flatA[iOff + k] * flatB[jOff + k];
				}
				matrixC[i][j] = tot;
			}
		}
	}
}

template<typename T>
void printMatrix(T ** mat, const int dimension) {
	for (int i = 0; i < dimension; ++i) {
		for (int j = 0; j < dimension; ++j) {
			cout << mat[i][j] << " ";
		}
		cout << endl;
	}
}

int main() {
	const int dim = 2;

	int ** matA = new int *[dim];
	int ** matB = new int *[dim];
	int ** matC = new int *[dim];
	
	for (int i = 0; i < dim; ++i) {
		matA[i] = new int[dim];
		matB[i] = new int[dim];
		matC[i] = new int[dim];
	}
	
	//randomizeMatrix(matA, dim);
	//randomizeMatrix(matB, dim);

	matA[0][0] = 3;
	matA[0][1] = 6;
	matA[1][0] = 7;
	matA[1][1] = 5;

	matB[0][0] = 3;
	matB[0][1] = 5;
	matB[1][0] = 6;
	matB[1][1] = 2;

	matrixMultiply(matA, matB, matC, dim);

	//printMatrix(matA, dim);
	//printMatrix(matB, dim);
	//printMatrix(matC, dim);

	assert(matC[0][0] == 45);
	assert(matC[0][1] == 27);
	assert(matC[1][0] == 51);
	assert(matC[1][1] == 45);

	for (int i = 0; i < dim; ++i) {
		delete[] matA[i];
		delete[] matB[i];
		delete[] matC[i];
	}

	delete[] matA;
	delete[] matB;
	delete[] matC;
}
