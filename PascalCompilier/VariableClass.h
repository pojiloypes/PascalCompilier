#include<iostream>
#include "IdentifiersTable.h"

using namespace std;

class VariableClass
{
	string stringVal;
	string name;
	double floatVal;
	int dataType;
	int integerVal;
	char charVal;

public:
	VariableClass(string n, int dt);

	string getName();

	int getDataType();

	int getIntegerVal();

	double getFloatVal();

	string getStringVal();

	char getCharVal();


	void setVal(int val);

	void setVal(double val);

	void setVal(char val);

	void setVal(string val);
};

