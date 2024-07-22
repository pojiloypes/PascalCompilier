#include "IdentifiersTable.h"

IdentifierClass::IdentifierClass(string name, int identType)
{
	this->name = name;
	this->identType = identType;
	if (identType != Identifiers::ident) parametrs = vector<IdentifierClass*>();
}

IdentifierClass::IdentifierClass(string name, int dataType, int identType)
{
	this->name = name;
	this->dataType = dataType;
	this->identType = identType;
	if (identType != Identifiers::ident) parametrs = vector<IdentifierClass*>();
}

void IdentifierClass::addParametr(IdentifierClass* newParam) { parametrs.push_back(newParam); }

string IdentifierClass::getName() { return name; }

int IdentifierClass::getDataType() { return dataType; }

int IdentifierClass::getIdentType() { return identType; }

vector<IdentifierClass*> IdentifierClass::getParametrs() { return parametrs; }

void IdentifierClass::setDataType(int dt) { this->dataType = dt; }
