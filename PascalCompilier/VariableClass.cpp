#include "VariableClass.h"

VariableClass::VariableClass(string n, int dt)
{
	name = n;
	dataType = dt;
}

string VariableClass::getName() { return name; }

int VariableClass::getDataType() { return dataType; }

int VariableClass::getIntegerVal() { return integerVal; }

double VariableClass::getFloatVal() { return floatVal; }

string VariableClass::getStringVal() { return stringVal; }

char VariableClass::getCharVal() { return charVal; }


void VariableClass::setVal(int val)
{
	integerVal = int(val);
	floatVal = double(val);
	if (dataType == TypeCodes::getTypeCode("integer") || dataType == TypeCodes::getTypeCode("char"))
		charVal = int(val);
}

void VariableClass::setVal(double val)
{
	integerVal = int(val);
	floatVal = double(val);

	if (dataType == TypeCodes::getTypeCode("integer"))
		charVal = int(val);
}

void VariableClass::setVal(char val)
{
	integerVal = int(val);
	charVal = val;
	if (dataType == TypeCodes::getTypeCode("double"))
		floatVal = val;
}

void VariableClass::setVal(string val) { stringVal = val; }
