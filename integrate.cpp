// Written by Mikhail Smirnov
// Solves for charge distribution on 5x5 pixel grid with origin at (0,0)

#include<iostream>
#include<tgmath.h>
#include<math.h>
#include<cmath>
#include<fstream>

using namespace std;

const double PI = 3.14159265358979323846264338327950288;
const double z_0 = 1;
const double Q_0 = 1;

// Function that is being integrated
double func(double sigma, int A, int B, int C, int E, int n) {
	double alpha_n = (n + 0.5) * PI;

	return (0.5 * sigma) * 4.0 * exp((-1 * pow(alpha_n, 2)) / 4 * sigma * sigma) * ((erf(B / sigma) - erf(A / sigma)) * (erf(E / sigma) - erf(C / sigma)));
}

// Midpoint Rule
double midpointRule(double intBegin, double intEnd, int A, int B, int C, int E, int n, int count, double step) {
	double integralMidpoint = 0;
	for (int i = 1; i <= count; i++) {
		// Midpoint rule
		integralMidpoint += step * func(intBegin + (i - 1.0) * step, A, B, C, E, n);
	}
	return integralMidpoint;
}

// Trapezoidal Rule
double trapRule(double intBegin, double intEnd, int A, int B, int C, int E, int n, int count, double step) {

	// Sum of first and last terms
	double integralTrap = func(intBegin, A, B, C, E, n) + func(intEnd, A, B, C, E, n);

	// Sum of middle terms
	for (int i = 1; i < count; i++) {
		integralTrap += 2 * func(intBegin + i * step, A, B, C, E, n);
	}

	// Multiplying by dx / 2
	integralTrap = (step / 2) * integralTrap;

	return integralTrap;
}

// Simpson's Rule
double simpRule(double intBegin, double intEnd, int A, int B, int C, int E, int n, int count, double step) {

	// c++ does not allow non-constant values (count) to initialize arrays, so I had to make the array dynamic
	//float x[count + 1]; // this did not work
	double* x = new double[count + 1];

	double sum = 0;

	x[0] = intBegin;

	for (int i = 1; i <= count; i++) {
		x[i] = intBegin + step * i;
	}

	for (int i = 1; i <= count / 2; i++) {
		sum += func(x[2 * i - 2], A, B, C, E, n) + 4 * func(x[2 * i - 1], A, B, C, E, n) + func(x[2 * i], A, B, C, E, n);
	}

	double integralSimp = sum * step / 3;

	delete[] x; // This dynamic array has to be deleted each time
	return integralSimp;
}



int main()
{
	double intervalBegin, intervalEnd, step;
	int count;
	double intMid, intTrap, intSimp;
	double integralArray[5][5];

	// Intervals for sigma
	cout << "Lower integral limit: ";
	cin >> intervalBegin;

	cout << "Upper integral limit: ";
	cin >> intervalEnd;

	cout << "Number of iterations for integration: ";
	cin >> count;

	step = (intervalEnd - intervalBegin) / count;

	//--------------------------------------------------------------------------------------------------------------------------------------------
	// This for-loop goes through each pixel by starting from top right pixel and going across and down.
	// so it starts at ( For each iteration of x going -5 ---> -3 ---> -1 ---> 1 ---> 3 ---> 5, as
	// y goes 5 ---> 3 ---> 1 ---> 3 ---> 5).
	// For each iteration in the nested for-loop, func gets integrated from 0 to infinity (intervalEnd).
	int A, B, C, E; // A -> B is the x coordinates, C -> E is the y coordinates
	int N; //Variable for limit of summation

	cout << "Limit for summation: ";
	cin >> N;

	const int arrSize = 25; // 5x5 grid means 25 elements
	double resultArrMid[arrSize]; // Midpoint rule array
	double resultArrTrap[arrSize]; // Trapezoidal rule array
	double resultArrSimp[arrSize]; // Simpson's rule array
	int index = -1; // Used for storing integral results arrays
	double summationMid;
	double summationTrap;
	double summationSimp;
	// This part cycles through all of the pixels by varying the A, B, C, and E values and integrating over time
	// for each iteration for the 5x5 grid.
	// It then stores each integrated result into a one-dimensional array for each respective integration method.
	// NOTE: The array's values are stored in the order of starting from the top left pixel and then going across and down and repeating
	for (int j = 4; j >= -4; j = j - 2) { // y pixel coordinate on grid
		for (int i = -4; i <= 4; i = i + 2) { // x pixel coordinate on grid

			A = i - 1;
			B = i + 1;
			C = j - 1;
			E = j + 1;
			
			// Printing out A,B,C,E coordinates
			cout << "A = " << A << ", B = " << B << ", C = " << C << ", E = " << E << endl;
			// This is the array in which the 25 integral results are stored in (from index 0-24)
			index++; // index for storing each element integral result
			
			// For-loop for summation up until N by varying n_i
			summationMid = 0;
			summationTrap = 0;
			summationSimp = 0;
			int alpha_n_i;
			double sumTerms;
			for (int n_i = 0; n_i <= N; n_i++) {
				alpha_n_i = ((n_i + 0.5) * PI);
				//alpha_n_i = ((2*n_i - 0.5) * (PI / 2.0));
				sumTerms = (Q_0 / 2.0) * (0.25) * alpha_n_i * sin(alpha_n_i * z_0);
				summationMid += sumTerms * midpointRule(intervalBegin, intervalEnd, A, B, C, E, n_i, count, step);
				summationTrap += sumTerms * trapRule(intervalBegin, intervalEnd, A, B, C, E, n_i, count, step);
				summationSimp += sumTerms * simpRule(intervalBegin, intervalEnd, A, B, C, E, n_i, count, step);
			}

			// Uncomment for unsummed values
			//int n_i = 10;
			//alpha_n_i = ((n_i + 0.5) * PI);
			//sumTerms = (Q_0 / 2.0) * (0.25) * alpha_n_i * sin(alpha_n_i * z_0);
			//summationMid = sumTerms * midpointRule(intervalBegin, intervalEnd, A, B, C, E, n_i, count, step);
			//summationTrap = sumTerms * trapRule(intervalBegin, intervalEnd, A, B, C, E, n_i, count, step);
			//summationSimp = sumTerms * simpRule(intervalBegin, intervalEnd, A, B, C, E, n_i, count, step);

			// Storing summation for that pixel
			resultArrMid[index] = summationMid;
			resultArrTrap[index] = summationTrap;
			resultArrSimp[index] = summationSimp;

			// Printing out integral results for that pixel
			cout << "Result from midpoint rule: " << resultArrMid[index] << endl;
			cout << "Result from trapezoidal rule: " << resultArrTrap[index] << endl;
			cout << "Result from Simpson's rule: " << resultArrSimp[index] << endl;

		}
	} // Pixel for-loop ends

	//Normalizing by the center pixel's value
	double centerPixelMid = resultArrMid[12]; // center pixel is 13th element
	double centerPixelTrap = resultArrTrap[12];
	double centerPixelSimp = resultArrSimp[12];
	for(int i = 0; i < arrSize; i++){
		resultArrMid[i] = resultArrMid[i] * (1 / centerPixelMid);
		resultArrTrap[i] = resultArrTrap[i] * (1 / centerPixelTrap);
		resultArrSimp[i] = resultArrSimp[i] * (1 / centerPixelSimp);
	}

	cout << "Midpoint rule center pixel: " << centerPixelMid << endl;
	cout << "Trapezoidal rule center pixel: " << centerPixelTrap << endl;
	cout << "Simpson's rule center pixel: " << centerPixelSimp << endl;

	//------------------------------------------------------------------------------------------------------------------------------------------
	// Printing array results to .txt file
	ofstream myfile("Integral_Results.txt");
	if (myfile.is_open())
	{
		myfile << "double resultArrMid[25]" << " = {";
		for (int i = 0; i < arrSize; i++) {
			myfile << resultArrMid[i] << ", ";
		}
		myfile << " };\n";

		myfile << "double resultArrTrap[25]" << " = {";
		for (int i = 0; i < arrSize; i++) {
			myfile << resultArrTrap[i] << ", ";
		}
		myfile << " };\n";

		myfile << "double resultArrSimp[25]" << " = {";
		for (int i = 0; i < arrSize; i++) {
			myfile << resultArrSimp[i] << ", ";
		}
		myfile << " };\n";


		myfile.close();
	}
	else cout << "Unable to open file";



	int a;
	cin >> a;
	return 0;
}