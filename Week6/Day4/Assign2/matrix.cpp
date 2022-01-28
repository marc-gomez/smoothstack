#include <iostream>
#include <cstdlib>
#include <cassert>
#include <type_traits>

using namespace std;

#define MAX_ROWS 10
#define MAX_COLS 10
#define MAX_DIM MAX_ROWS * MAX_COLS

template<typename T>
void randomizeMatrix(T ** mat, const int numRow, const int numCol) {
	for (int i = 0; i < numRow; ++i) {
		for (int j = 0; j < numCol; ++j) {
			mat[i][j] = rand() % 10;
		}
	}
}

template<typename T>
void matrixMultiply(T ** matrixA, T ** matrixB, T ** matrixC,
		const int numRowA, const int numColA, const int numRowB, const int numColB) {
	assert(is_arithmetic_v<T>);
	
	T flatA[MAX_DIM];
	T flatB[MAX_DIM];

	#pragma omp parallel for
	for (int i = 0; i < numRowA; i++) {
		for (int j = 0; j < numColA; j++) {
			flatA[i * numColA + j] = matrixA[i][j];
		}
	}

	#pragma omp parallel for
	for (int i = 0; i < numRowB; ++i) {
		for (int j = 0; j < numColB; ++j) {
			flatB[j * numRowB + i] = matrixB[i][j];
		}
	}

	#pragma omp parallel shared(matrixC) num_threads(40)
	{
		#pragma omp for schedule(static)
		for (int i = 0; i < numRowA; i++) {
			int iOff = i * numColA;
			for (int j = 0; j < numColB; j++) {
				int jOff = j * numRowB;
				T tot = 0;
				for (int k = 0; k < numRowB; k++) {
					tot += flatA[iOff + k] * flatB[jOff + k];
				}
				matrixC[i][j] = tot;
			}
		}
	}
}

template<typename T>
void printMatrix(T ** mat, const int numRow, const int numCol) {
	for (int i = 0; i < numRow; ++i) {
		for (int j = 0; j < numCol; ++j) {
			cout << mat[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

int main() {
	const int numRowA = 2;
	const int numColA = 3;
	const int numRowB = 3;
	const int numColB = 2;

	int ** matA = new int *[numRowA];
	int ** matB = new int *[numRowB];
	int ** matC = new int *[numRowA];
	
	for (int i = 0; i < numRowA; ++i) {
		matA[i] = new int[numColA];
	}
	for (int i = 0; i < numRowB; ++i) {
		matB[i] = new int[numColB];
	}
	for (int i = 0; i < numRowA; ++i) {
		matC[i] = new int[numColB];
	}

	//randomizeMatrix(matA, numRowA, numColA);
	//randomizeMatrix(matB, numRowB, numColB);

	matA[0][0] = 3;
	matA[0][1] = 6;
	matA[0][2] = 7;
	matA[1][0] = 5;
	matA[1][1] = 3;
	matA[1][2] = 5;

	matB[0][0] = 6;
	matB[0][1] = 2;
	matB[1][0] = 9;
	matB[1][1] = 1;
	matB[2][0] = 2;
	matB[2][1] = 7;
	
	matrixMultiply(matA, matB, matC, numRowA, numColA, numRowB, numColB);

	//printMatrix(matA, numRowA, numColA);
	//printMatrix(matB, numRowB, numColB);
	//printMatrix(matC, numRowA, numColB);

	assert(matC[0][0] == 86);
	assert(matC[0][1] == 61);
	assert(matC[1][0] == 67);
	assert(matC[1][1] == 48);

	for (int i = 0; i < numRowA; ++i) {
		delete[] matA[i];
	}
	for (int i = 0; i < numRowB; ++i) {
		delete[] matB[i];
	}
	for (int i = 0; i < numRowA; ++i) {
		delete[] matC[i];
	}

	delete[] matA;
	delete[] matB;
	delete[] matC;
}
