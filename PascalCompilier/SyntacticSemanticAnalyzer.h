#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <deque>
#include <windows.h>
#include <algorithm>
#include <map>
#include "VariableClass.h"



using namespace std;

class SyntacticSemanticAnalyzer
{
public:
    SyntacticSemanticAnalyzer(string fileName);

    void programme();

    int getErrorsCount();

    map<string, VariableClass*> getVarTable();

    ~SyntacticSemanticAnalyzer();

protected:
    int symbolCode;
    LexicalAnalyzer* lexer;

    vector<int> begpart;
    vector<int> st_varpart;
    vector<int> st_procfuncpart;
    vector<int> after_var;
    vector<int> id_starters;
    vector<int> after_proc;
    vector<int> assignmentStatementPtra;
    vector<int> identSemicolonPtra;
    vector<int> indentRightparSemicVarPtra;
    vector<int> rightparSemicVarPtra;
    vector<int> indentLeftparVarPtra;
    vector<int> indentVarPtra;
    vector<int> semicVarPtra;
    vector<int> identLeftparColonVarPtra;
    vector<int> leftparColonVarPtra;
    vector<int> colonSemicVarPtra;
    deque<pair<IdentifierClass*, int>> identifiersList;
    map<string, VariableClass*> varTable;

    void nextSym();

    void accept(int symbolExpected);

    void accept2(int symbolExpected, vector<int> followers);

    void skipto(vector<int> vec);

    void skipto2(vector<int> vec1, vector<int> vec2);

    void skipto2(int sym1, vector<int> vec2);

    bool belong(vector<int> vec);

    vector<int> set_disjunct(vector<int> vec1, vector<int> vec2);

    vector<int> set_disjunct(int sym, vector<int> vec2);

    int type(vector<int> followers);

    IdentifierClass* findIdentInList(string name, int highScope);

    void addNewIdentInList(IdentifierClass* newIdent, int scope, int highScope);

    void varDeclaration(vector<int> followers, int scope, int funcProcIdentIdx);

    bool isItConst();

    bool check—ompatibilityWithConstTypes(int expressionType, int currentType);

    bool check—ompatibilityWithIdentTypes(int expressionType, int currentType);

    vector<int> getAcceptableSigns(int expressionType);

    void readFunctionCall(IdentifierClass* curIdent, vector<int> followers);

    void expression(vector<int> followers, int expressionType);

    void assignmentStatement(vector<int> followers, int expressionType);

    void statement(vector<int> followers);

    void compoundStatement(vector<int> followers);

    void varPart(vector<int> followers);

    void procPart(vector<int> followers);

    void funcPart(vector<int> followers);

    void statementPart(vector<int> followers);

    long long int getHash(string s);

    void block(vector<int> followers);
};