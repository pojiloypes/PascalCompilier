#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>

#include "LexicalAnalyzer.h"

using namespace std;

class SyntacticAnalyzer
{
protected:
    int symbolCode;
    LexicalAnalyzer* lexer;

    vector<int> begpart;
    vector<int> st_varpart;
    vector<int> st_procfuncpart;
    vector<int> after_var;
    vector<int> id_starters;
    vector<int> after_proc;

    

    void nextSym() ;

    void accept(int symbolExpected) ;

    void accept2(int symbolExpected, vector<int> followers) ;

    void skipto(vector<int> vec) ;

    void skipto2(vector<int> vec1, vector<int> vec2) ;

    bool belong(vector<int> vec) ;

    vector<int> set_disjunct(vector<int> vec1, vector<int> vec2) ;

    void type(vector<int> followers) ;

    void varDeclaration(vector<int> followers) ;

    bool isItConst() ;

    void expression(vector<int> followers) ;

    void assignmentStatement(vector<int> followers) ;

    void statement(vector<int> followers) ;

    void compoundStatement(vector<int> followers) ;

    void varPart(vector<int> followers) ;

    void procPart(vector<int> followers) ;

    void funcPart(vector<int> followers) ;


    void statementPart(vector<int> followers) ;


    void block(vector<int> followers) ;

public:
    SyntacticAnalyzer(string fileName);

    void programme() ;

    ~SyntacticAnalyzer() ;
};




