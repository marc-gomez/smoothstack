#include <iostream>
#include <cassert>

using namespace std;

int main() {

	// Assignment 2.1
	// Create an array of void pointers so that the compiler treats the array as the same data type.
	
	void * arr[2];
	int a = 2;
	float b = 3;
	arr[0] = &a;
	arr[1] = &b;

	assert(*(int *)arr[0] == int(2));
	assert(*(float *)arr[1] == float(3));

	cout << *(int *)arr[0] << " " << *(float *)arr[1] << endl;


	// Assignment 2.2
	
	// The data types cannot be gathered automatically because the compiler only knows of the void pointer.
	// This is the reason why we have to cast the void pointer to a float or int in the cout.
	// Dereferencing the pointer without casting would not work.
	
	// cout << *arr[0] << " " << *arr[1] << endl;
	// int c = *arr[0];
	// int * d = arr[0];
	// ^ these three examples all give errors 
}
