#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <stack>
#include <windows.h>
#include "Calculator.h"

using namespace std;

class Generator
{
public:
    Generator(string fileName);

    void run();

protected:
    map<string, VariableClass*> varTable;
    Calculator* calculator;
    LexicalAnalyzer* lexer;
    int symbolCode;
    bool errorsFlag;

    bool checkForNum(string str);

    int getPriority(string str);

    vector<string> translateToPostFix(vector<string> expression);

    void nextSym();

    void varDeclaration();

    void varPart();

    string getSign();

    pair<vector <string>, int> getExpVector();

    void writelnFunc();

    void block();
};

