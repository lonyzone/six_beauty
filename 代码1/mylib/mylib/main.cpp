// MyExecRefsLib.cpp
// compile with: /EHsc /link MathFuncsLib.lib

#include <iostream>

#include "match.h"
//#pragma comment( lib, "..\\mylib\\match.lib" ) 
using namespace std;

int main()
{
	double a = 7.4;
	int b = 99;

	cout << "a + b = " <<
		MathFuncs::MyMathFuncs::Add(a, b) << endl;
	cout << "a - b = " <<
		MathFuncs::MyMathFuncs::Subtract(a, b) << endl;
	cout << "a * b = " <<
		MathFuncs::MyMathFuncs::Multiply(a, b) << endl;
	cout << "a / b = " <<
		MathFuncs::MyMathFuncs::Divide(a, b) << endl;

	return 0;
}