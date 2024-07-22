#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <windows.h>
#include "LexicalAnalyzer.h"

using namespace std;

class IdentifierClass
{
	string name;
	int dataType;
	int identType;
	vector<IdentifierClass*> parametrs;

public:
	IdentifierClass(string name, int identType);

	IdentifierClass(string name, int dataType, int identType);

	void addParametr(IdentifierClass* newParam);

	string getName();

	int getDataType();

	int getIdentType();

	vector<IdentifierClass*> getParametrs();

	void setDataType(int dt);
};
