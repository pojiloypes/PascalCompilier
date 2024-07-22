#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <stack>
#include <windows.h>
#include <map>
#include "SyntacticSemanticAnalyzer.h"

using namespace std; 

class Calculator
{
public:
    Calculator(map<string, VariableClass*> newTable);

    int calculateInteger(vector <string> expression);

    double calculateFloat(vector <string> expression);

    string calculateString(vector <string> expression);

    char calculateChar(vector <string> expression);

    string getResByString(vector <string> expression, int dataType);

protected:
    map<string, VariableClass*> varTable;

    int calc(int x, int y, char sym);

    double calc(double x, double y, char sym);

    char calc(char x, char y, char sym);

    string calc(string x, string y, char sym);

    bool isSign(string c);

    
};
