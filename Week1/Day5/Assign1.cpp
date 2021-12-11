// Input must be of the form:

// Matrix A dimensions
// Matrix B dimensions
//
// Matrix A
//
// Matrix B

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cassert>

using namespace std;

// Initialize a matrix by reading from cin
void createMatrix(const int numRows, const int numCols, vector< vector<int> > &mat) {
	for (int i = 0; i < numRows; ++i) {
		string tempLine;
		getline(cin, tempLine);
		stringstream ss;
		ss << tempLine;
		vector<int> tempVec;

		for (int j = 0; j < numCols; ++j) {
			int tempInt;
			ss >> tempInt;
			tempVec.push_back(tempInt);
		}

		mat.push_back(tempVec);
	}
}

// Multiply two matrices to create a third
void multiplyMatrices(const vector< vector<int> > &matA, const vector< vector<int> > &matB,
		vector< vector<int> > &matC) {

	int numRowA, numColA, numRowB, numColB;
	numRowA = matA.size();
	numColA = matA[0].size();
	numRowB = matB.size();
	numColB = matB[0].size();

	for(int i = 0; i < numRowA; ++i) {
		vector<int> tempVec;
		for(int j = 0; j < numColB; ++j) {
			int sum = 0;
			for(int k = 0; k < numRowB; ++k) {
				sum = sum + (matA[i][k] * matB[k][j]); 
			}
			tempVec.push_back(sum);
		}
		matC.push_back(tempVec);
	}
}

// Print matrix to cout
void printMatrix(const int numRows, const int numCols, const vector< vector<int> > &mat) {
	for (int i = 0; i < numRows; ++i) {
		for (int j = 0; j < numCols; ++j) {
			cout << mat.at(i).at(j) << " ";
		}
		cout << endl;
	}
}

int main() {
	string Adim;
	string Bdim;

	getline(cin, Adim);
	getline(cin, Bdim);

	int numRowA;
	int numColA;
	int numRowB;
	int numColB;
	int numRowC;
	int numColC;

	// Initialize dimensions of Matrix A and B
	stringstream s1(Adim);
	s1 >> numRowA;
	s1 >> numColA;

	stringstream s2(Bdim);
	s2 >> numRowB;
	s2 >> numColB;

	// Error check
	if (numColA != numRowB || numRowA == 0 || numColA == 0 || numRowB == 0 || numColB == 0) {
		cout << "One or more matrix is invalid" << endl;
		return -1;
	}

	numRowC = numRowA;
	numColC = numColB;

	vector< vector<int> > MatA;
	vector< vector<int> > MatB;
	vector< vector<int> > MatC;

	string tempLine;

	getline(cin, tempLine); // Skip empty line

	createMatrix(numRowA, numColA, MatA); // Initialize Matrix A

	getline(cin, tempLine); // Skip empty line
	
	createMatrix(numRowB, numColB, MatB); // Initialize Matrix B

	multiplyMatrices(MatA, MatB, MatC); // A x B = C	

	// Sanity check
	assert(MatC.size() == numRowC);
	assert(MatC[0].size() == numColC);

	// Print Matrices
	cout << "Matrix A:" << endl;
	printMatrix(numRowA, numColA, MatA);
	cout << endl;
	cout << "Matrix B:" << endl;
	printMatrix(numRowB, numColB, MatB);
	cout << endl;
	cout << "Matrix C:" << endl;
	printMatrix(numRowC, numColC, MatC);

	return 0;
}
