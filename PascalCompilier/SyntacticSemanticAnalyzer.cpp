#include "SyntacticSemanticAnalyzer.h"

SyntacticSemanticAnalyzer::SyntacticSemanticAnalyzer(string fileName)
{
    lexer = new LexicalAnalyzer(fileName, false);
    lexer->nextSym();
    symbolCode = lexer->getSymbolCode();

    identifiersList = deque<pair<IdentifierClass*, int>>();
    varTable = map<string, VariableClass*>();
    begpart = vector<int>{ Keywords::getKeyWordCode("var"), Keywords::getKeyWordCode("function"), Keywords::getKeyWordCode("procedure"), Keywords::getKeyWordCode("begin") };
    st_varpart = vector<int>{ Keywords::getKeyWordCode("var"), Keywords::getKeyWordCode("function"), Keywords::getKeyWordCode("procedure"), Keywords::getKeyWordCode("begin") };
    st_procfuncpart = vector<int>{ Keywords::getKeyWordCode("function"), Keywords::getKeyWordCode("procedure"), Keywords::getKeyWordCode("begin") };
    indentRightparSemicVarPtra = vector<int>{ Identifiers::ident, Signs::rightpar, Signs::semicolon, Keywords::getKeyWordCode("var") };
    identLeftparColonVarPtra = vector<int>{ Identifiers::ident, Signs::leftpar, Signs::colon, Keywords::getKeyWordCode("var") };
    indentLeftparVarPtra = vector<int>{ Identifiers::ident, Signs::leftpar, Keywords::getKeyWordCode("var") };
    rightparSemicVarPtra = vector<int>{ Signs::rightpar, Signs::semicolon, Keywords::getKeyWordCode("var") };
    leftparColonVarPtra = vector<int>{ Signs::leftpar, Signs::colon, Keywords::getKeyWordCode("var") };
    colonSemicVarPtra = vector<int>{ Signs::colon, Signs::semicolon, Keywords::getKeyWordCode("var") };
    assignmentStatementPtra = vector<int>{ Identifiers::ident, Signs::semicolon, Signs::assign };
    indentVarPtra = vector<int>{ Identifiers::ident, Keywords::getKeyWordCode("var") };
    semicVarPtra = vector<int>{ Signs::semicolon, Keywords::getKeyWordCode("var") };
    identSemicolonPtra = vector<int>{ Identifiers::ident, Signs::semicolon };
    after_var = vector<int>{ Signs::semicolon };
    after_proc = vector<int>{ Signs::semicolon };   
}

map<string, VariableClass*> SyntacticSemanticAnalyzer::getVarTable() { return varTable; }

long long int SyntacticSemanticAnalyzer::getHash(string s)
{
    int baza = 31;
    long long int* bazaPows = new long long int[s.length() + 1];
    bazaPows[0] = 1;
    for (int i = 1; i < s.length() + 1; i++)
        bazaPows[i] = bazaPows[i - 1] * baza;
    long long int hash = 1;

    for (int i = 0; i < s.length(); i++)
        hash = hash * bazaPows[i + 1] + s[i];

    return hash;
}

void SyntacticSemanticAnalyzer::SyntacticSemanticAnalyzer::nextSym()
{
    do
    {
        lexer->nextSym();
        symbolCode = lexer->getSymbolCode();
    } while (!lexer->checkEof() && symbolCode == 0);

}

void SyntacticSemanticAnalyzer::accept(int symbolExpected)
{
    if (symbolCode == symbolExpected) nextSym();
    else lexer->addNewError(993);
}

void SyntacticSemanticAnalyzer::accept2(int symbolExpected, vector<int> followers)
{
    if (symbolCode != symbolExpected)
    {
        lexer->addNewError(symbolExpected);
        skipto(followers);
    }
    if (symbolCode == symbolExpected) nextSym();
}

void SyntacticSemanticAnalyzer::skipto(vector<int> vec)
{
    while (!lexer->checkEof() && (find(vec.begin(), vec.end(), symbolCode) == vec.end()))
    {
        nextSym();
        symbolCode = lexer->getSymbolCode();
    }
}

void SyntacticSemanticAnalyzer::skipto2(vector<int> vec1, vector<int> vec2)
{
    while (!lexer->checkEof() && (find(vec1.begin(), vec1.end(), symbolCode) == vec1.end() &&
            find(vec2.begin(), vec2.end(), symbolCode) == vec2.end()))
    {
        nextSym();
        symbolCode = lexer->getSymbolCode();
    }
}

void SyntacticSemanticAnalyzer::skipto2(int sym1, vector<int> vec2)
{
    while (!lexer->checkEof() && (symbolCode != sym1 &&
        find(vec2.begin(), vec2.end(), symbolCode) == vec2.end()))
    {
        nextSym();
        symbolCode = lexer->getSymbolCode();
    }
}

bool SyntacticSemanticAnalyzer::belong(vector<int> vec)
{
    bool ans = false;
    for (int i = 0; i < vec.size(); i++) if (symbolCode == vec[i]) ans = true;
    return ans;
}

vector<int> SyntacticSemanticAnalyzer::set_disjunct(vector<int> vec1, vector<int> vec2)
{
    vector<int> res;
    res.reserve(vec1.size() + vec2.size());
    res.insert(res.end(), vec1.begin(), vec1.end());
    res.insert(res.end(), vec2.begin(), vec2.end());
    return res;
}

vector<int> SyntacticSemanticAnalyzer::set_disjunct(int sym, vector<int> vec2)
{
    vector<int> res = vec2;
    res.push_back(sym);
    return res;
}

int SyntacticSemanticAnalyzer::type(vector<int> followers)
{
    int dataType = -1;
    map<string, int> tc = TypeCodes::getTypeCodes();
    id_starters.clear();
    for (map<string, int>::iterator it = tc.begin(); it != tc.end(); it++) id_starters.push_back(it->second);

    if (!belong(id_starters))
    {
        lexer->addNewError(993); 
        skipto2(id_starters, followers);
    }

    if (belong(id_starters))
    {
        dataType = symbolCode;
        nextSym();
        if (!belong(followers))
        {
            lexer->addNewError(6);
            skipto(followers);
        }
    }
    return dataType;
}

IdentifierClass* SyntacticSemanticAnalyzer::findIdentInList(string name, int highScope)
{
    IdentifierClass* finded = nullptr;

    IdentifierClass* curIdent;
    if (highScope == -1) 
        highScope = 0;

    for (int i = identifiersList.size() - 1; i >= highScope && finded == nullptr; i--)
    {
        curIdent = identifiersList[i].first;
        if (curIdent->getName() == name) finded = curIdent;
    }
    return finded;
}

void SyntacticSemanticAnalyzer::addNewIdentInList(IdentifierClass* newIdent, int scope, int highScope)
{
    if (findIdentInList(newIdent->getName(), highScope) == nullptr) 
        identifiersList.push_back({ newIdent , scope });
    else
        lexer->addNewError(1000);
}

void SyntacticSemanticAnalyzer::varDeclaration(vector<int> followers, int scope, int funcProcIdentIdx)
{
    IdentifierClass* newIdent;
    int dataType;
    int varsCount = 0;
    id_starters.clear();
    id_starters.push_back(Identifiers::ident);

    if (!belong(id_starters))
    {
        lexer->addNewError(2);
        skipto2(id_starters, followers);
    }
    if (symbolCode == Identifiers::ident)
    {
        newIdent = new IdentifierClass(lexer->getIdentName(), 0, Identifiers::ident);
        addNewIdentInList(newIdent, scope, funcProcIdentIdx);
        varsCount++;

        nextSym();
        while (!lexer->checkEof() && symbolCode == Signs::comma)
        {
            nextSym();
            if (symbolCode == Identifiers::ident)
            {
                newIdent = new IdentifierClass(lexer->getIdentName(), 0, Identifiers::ident);
                addNewIdentInList(newIdent, scope, funcProcIdentIdx);
                varsCount++;
            }
            accept(Identifiers::ident);
            
        }
        accept(Signs::colon);
        dataType = type(followers);
        if (dataType == -1) while (!identifiersList.empty() && identifiersList.back().first->getDataType() == 0) identifiersList.pop_back();
        else
        {
            vector<IdentifierClass*> parametrs = vector<IdentifierClass*>();
            int i = identifiersList.size() - 1;
            for (; i >= 0 && identifiersList.size()-1 - i < varsCount; i--)
            {
                identifiersList[i].first->setDataType(dataType);
                if (funcProcIdentIdx != -1) identifiersList[funcProcIdentIdx].first->addParametr(identifiersList[i].first);
                varTable[identifiersList[i].first->getName()] = new VariableClass(identifiersList[i].first->getName(), identifiersList[i].first->getDataType());
            }
        }
        

        if (!belong(followers))
        {
            lexer->addNewError(6);
            skipto(followers);
        }
    }
}


bool SyntacticSemanticAnalyzer::isItConst()
{
    return symbolCode == Identifiers::booleanc || symbolCode == Identifiers::charc ||
        symbolCode == Identifiers::floatc || symbolCode == Identifiers::intc ||
        symbolCode == Identifiers::stringc;
}


bool SyntacticSemanticAnalyzer::check—ompatibilityWithConstTypes(int expressionType, int currentType)
{
    bool res = false;
    if (expressionType == TypeCodes::getTypeCode("integer"))
    {
        if (currentType == Identifiers::floatc ||
            currentType == Identifiers::realc ||
            currentType == Identifiers::intc) res = true;
    }
    else if (expressionType == TypeCodes::getTypeCode("string"))
    {
        if (currentType == Identifiers::stringc ||
            currentType == Identifiers::charc) res = true;
    }
    else if (expressionType == TypeCodes::getTypeCode("boolean"))
    {
        if (currentType == Identifiers::booleanc) res = true;
    }
    else if (expressionType == TypeCodes::getTypeCode("float"))
    {
        if (currentType == Identifiers::floatc ||
            currentType == Identifiers::realc ||
            currentType == Identifiers::intc) res = true;
    }
    else if (expressionType == TypeCodes::getTypeCode("real"))
    {
        if (currentType == Identifiers::floatc ||
            currentType == Identifiers::realc ||
            currentType == Identifiers::intc) res = true;
    }
    else if (expressionType == TypeCodes::getTypeCode("char"))
    {
        if (currentType == Identifiers::charc ||
            currentType == Identifiers::intc) res = true;
    }
    return res;
}


bool SyntacticSemanticAnalyzer::check—ompatibilityWithIdentTypes(int expressionType, int currentType)
{
    bool res = false;
    if (expressionType == TypeCodes::getTypeCode("integer"))
    {
        if ( currentType == TypeCodes::getTypeCode("integer")) res = true;
    }
    else if (expressionType == TypeCodes::getTypeCode("string"))
    {
        if (currentType == TypeCodes::getTypeCode("string") ||
            currentType == TypeCodes::getTypeCode("char")) res = true;
    }
    else if (expressionType == TypeCodes::getTypeCode("boolean"))
    {
        if (currentType == TypeCodes::getTypeCode("boolean")) res = true;
    }
    else if (expressionType == TypeCodes::getTypeCode("float"))
    {
        if (currentType == TypeCodes::getTypeCode("float") ||
            currentType == TypeCodes::getTypeCode("real") ||
            currentType == TypeCodes::getTypeCode("integer")) res = true;
    }
    else if (expressionType == TypeCodes::getTypeCode("real"))
    {
        if (currentType == TypeCodes::getTypeCode("float") ||
            currentType == TypeCodes::getTypeCode("real") ||
            currentType == TypeCodes::getTypeCode("integer")) res = true;
    }
    else if (expressionType == TypeCodes::getTypeCode("char"))
    {
        if (currentType == TypeCodes::getTypeCode("char") ||
            currentType == TypeCodes::getTypeCode("integer")) res = true;
    }
    return res;
}


vector<int> SyntacticSemanticAnalyzer::getAcceptableSigns(int expressionType)
{
    vector<int> res;
    if (expressionType == TypeCodes::getTypeCode("integer") || expressionType == TypeCodes::getTypeCode("float") || expressionType == TypeCodes::getTypeCode("real"))
    {
        res = vector<int>{Signs::star, Signs::slash, Signs::plus, Signs::minus};
    }
    else if (expressionType == TypeCodes::getTypeCode("string"))
    {
        res = vector<int>{Signs::plus};
    }
    else if (expressionType == TypeCodes::getTypeCode("boolean"))
    {
        res = vector<int>{ Signs::later, Signs::greater, Signs::laterequal, Signs::greaterequal, Signs::latergreater };
    }
    else if (expressionType == TypeCodes::getTypeCode("char"))
    {
        res = vector<int>{ Signs::plus, Signs::minus };
    }

    return res;
}

void SyntacticSemanticAnalyzer::readFunctionCall(IdentifierClass* curIdent, vector<int> followers)
{
    if (symbolCode != Signs::leftpar)
    {
        lexer->addNewError(18);
        skipto2(Signs::leftpar, followers);
    }
    if (symbolCode == Signs::leftpar)
    {
        vector<IdentifierClass*> params = curIdent->getParametrs();
        int readedParamsCount = 0;

        accept(Signs::leftpar);
        while (symbolCode == Identifiers::ident)
        {
            readedParamsCount++;
            IdentifierClass* newIdent = findIdentInList(lexer->getIdentName(), 0);
            if (curIdent == nullptr) lexer->addNewError(1001);
            else
            {
                if (readedParamsCount < params.size()  && !check—ompatibilityWithIdentTypes(params[readedParamsCount - 1]->getDataType(), newIdent->getDataType()))
                {
                    lexer->addNewError(1002);
                }
            }
            accept2(Identifiers::ident, set_disjunct(vector<int>{Signs::comma, Signs::rightpar}, followers));
            if (symbolCode != Signs::rightpar) accept2(Signs::comma, set_disjunct(vector<int>{Identifiers::ident}, followers));
        }

        if (readedParamsCount != params.size())   lexer->addNewError(1004);
        //accept2(Signs::rightpar, followers);
        if (!belong(set_disjunct(Signs::rightpar, followers)))
        {
            lexer->addNewError(6);
            skipto(followers);
        }
    }
}

void SyntacticSemanticAnalyzer::expression(vector<int> followers, int expressionType)
{
    if (symbolCode != Identifiers::ident && !isItConst() && symbolCode != Signs::leftpar) 
    {
        lexer->addNewError(18);
        vector<int> identifiers = vector<int>{ Identifiers::ident,  Identifiers::booleanc, Identifiers::charc,
                                                Identifiers::floatc, Identifiers::intc, Identifiers::stringc };
        skipto2(identifiers, followers);
    }
    else
    {
        vector<int> acceptableSigns = getAcceptableSigns(expressionType);
        vector<int> unAcceptableSigns = vector<int>{ Signs::star, Signs::slash, Signs::plus, Signs::minus, Signs::later, 
            Signs::greater, Signs::laterequal, Signs::greaterequal, Signs::latergreater };
        for (int i = 0; i < unAcceptableSigns.size(); i++)
        {
            if (find(acceptableSigns.begin(), acceptableSigns.end(), unAcceptableSigns[i]) != acceptableSigns.end())
                unAcceptableSigns.erase(unAcceptableSigns.begin()+i);
        }

        bool operatorNotFoundFlag = false;
        while (!lexer->checkEof() && !operatorNotFoundFlag && (symbolCode == Identifiers::ident || isItConst() || symbolCode == Signs::leftpar))
        {

            if (symbolCode == Signs::leftpar)
            {
                accept(Signs::leftpar);
                expression(set_disjunct(Signs::rightpar, followers), expressionType);
                accept(Signs::rightpar);
            }
            else
            {
                if (isItConst())
                {
                    if (expressionType != -1 && !check—ompatibilityWithConstTypes(expressionType, symbolCode)) lexer->addNewError(1002);
                }
                else
                {
                    IdentifierClass* curIdent = findIdentInList(lexer->getIdentName(), 0);
                    if (curIdent == nullptr) lexer->addNewError(1001);
                    else
                    {
                        if (curIdent->getIdentType() != Identifiers::ident)
                        {
                            nextSym();
                            readFunctionCall(curIdent, set_disjunct(acceptableSigns, followers));
                            if (symbolCode != Signs::rightpar) lexer->addNewError(Signs::rightpar);
                        }
                        if (expressionType != -1 && !check—ompatibilityWithIdentTypes(expressionType, curIdent->getDataType())) lexer->addNewError(1002);
                    }
                }
                nextSym();
            }

            if (belong(acceptableSigns)) nextSym();
            else if (belong(unAcceptableSigns))
            {
                lexer->addNewError(1003);
                nextSym();
            }
            else operatorNotFoundFlag = true;

        }
        if (!belong(followers))
        {
            lexer->addNewError(6); /* Á‡ÔÂ˘ÂÌÌ˚È ÒËÏ‚ÓÎ */
            skipto(followers);
        }
    }
}
void SyntacticSemanticAnalyzer::assignmentStatement(vector<int> followers, int expressionType)
{
    
    vector<int> ptra = assignmentStatementPtra;

    accept2(Identifiers::ident, ptra); 
    if (symbolCode != Signs::assign)
    {
        lexer->addNewError(Signs::assign);
        skipto2(Signs::assign, followers);
    }

    if (symbolCode == Signs::assign)
    {

        accept(Signs::assign);
        expression(followers, expressionType);
        //if (symbolCode == Signs::rightpar) accept(Signs::rightpar);
        accept(Signs::semicolon);
        if (!belong(followers))
        {
            lexer->addNewError(6);
            skipto(followers);
        }
    }
}

void SyntacticSemanticAnalyzer::statement(vector<int> followers)
{
    IdentifierClass *newIdent;
    vector<int> ptra;
    string identName;
    int dataType = -1;

    switch (symbolCode)
    {
    case Identifiers::ident:
        
        identName = lexer->getIdentName();
        newIdent = findIdentInList(identName, 0);
        if (newIdent == nullptr)  lexer->addNewError(1001);

        else dataType = newIdent->getDataType();
        if (newIdent->getIdentType() == Identifiers::ident)
        {
            ptra = set_disjunct(identSemicolonPtra, followers);
            ptra = set_disjunct(Keywords::getKeyWordCode("begin"), ptra);
            ptra.push_back(Keywords::getKeyWordCode("writeln"));
            assignmentStatement(ptra, dataType);
        }
        else
        {
            nextSym();
            readFunctionCall(newIdent, set_disjunct(Signs::semicolon, followers));
            accept2(Signs::rightpar, set_disjunct(Signs::semicolon, followers));
            accept2(Signs::semicolon, followers);
        }

        break;
    case 113:
        compoundStatement(followers);
        break;
    case 135: // ÍÓ‰ writeln
        skipto(vector<int>{Signs::semicolon});
        accept(Signs::semicolon); // ˝ÚÓÚ ÍÛÒÓÍ ÔÎÓıÓ Ì‡ÔËÒ‡Ì Ë ÌÂ Ò‡·ÓÚ‡ÂÚ ÂÒÎË ·Û‰ÂÚ Ó¯Ë·Í‡
    default:

        break;
    }
    ptra = set_disjunct(vector<int> {Identifiers::ident}, followers);
    ptra.push_back(Keywords::getKeyWordCode("writeln"));
    if (!belong(set_disjunct(Keywords::getKeyWordCode("begin"), ptra)))
    {
        lexer->addNewError(6);
        skipto(followers);
    }
}

void SyntacticSemanticAnalyzer::compoundStatement(vector<int> followers)
{
    vector<int> ptra;

    if (symbolCode != Keywords::getKeyWordCode("begin"))
    {
        lexer->addNewError(Keywords::getKeyWordCode("begin"));
        skipto2(Keywords::getKeyWordCode("begin"), followers);
    }
    if (symbolCode == Keywords::getKeyWordCode("begin"))
    {
        ptra = set_disjunct(Keywords::getKeyWordCode("end"), followers);

        accept(Keywords::getKeyWordCode("begin"));
        statement(ptra);

        while (!lexer->checkEof() && !belong(ptra))
        {
            statement(ptra);
        }
        ptra = set_disjunct(Keywords::getKeyWordCode("end"), followers);
        accept2(Keywords::getKeyWordCode("end"), ptra);
        //accept(Keywords::getKeyWordCode("end"));

        if (!belong(followers))
        {
            lexer->addNewError(6);
            skipto(followers);
        }
    }
}

void SyntacticSemanticAnalyzer::varPart(vector<int> followers)
{
    vector<int> ptra;
    int scope;
    if (identifiersList.empty()) scope = 0;
    else scope = identifiersList[identifiersList.size() - 1].second;

    if (!belong(st_varpart))
    {
        lexer->addNewError(18);
        skipto2(st_varpart, followers);
    }

    if (symbolCode == Keywords::getKeyWordCode("var"))
    {
        nextSym();
        ptra = set_disjunct(vector<int> {Identifiers::ident, Signs::semicolon, Signs::assign}, followers);
        do
        {
            varDeclaration(ptra, scope, -1);
            if (symbolCode == Signs::assign)
            {
                accept(Signs::assign);
                if (!(symbolCode == Identifiers::charc || symbolCode == Identifiers::stringc || 
                    symbolCode == Identifiers::floatc || symbolCode == Identifiers::intc))
                    lexer->addNewError(6);
                else
                    nextSym();
            }
            accept(Signs::semicolon);
        } while (!lexer->checkEof() && symbolCode == Identifiers::ident);
        if (!belong(followers))
        {
            lexer->addNewError(6);
            skipto(followers);
        }
    }
}

void SyntacticSemanticAnalyzer::procPart(vector<int> followers)
{
    vector<int> ptra;

    int procIdent = -1;
    int scope;
    if (identifiersList.empty()) scope = 0;
    else scope = identifiersList[identifiersList.size() - 1].second + 1;

    if (!belong(st_procfuncpart))
    {
        lexer->addNewError(18);
        skipto2(st_varpart, followers);
    }

    if (symbolCode == Keywords::getKeyWordCode("procedure"))
    {
        accept(Keywords::getKeyWordCode("procedure"));

        ptra = set_disjunct(indentLeftparVarPtra, followers);
        if (symbolCode == Identifiers::ident)
        {
            addNewIdentInList(new IdentifierClass(lexer->getIdentName(), TypeCodes::getTypeCode("void"), Keywords::getKeyWordCode("procedure")), scope++, 0);
            procIdent = identifiersList.size()-1;
        }
        accept2(Identifiers::ident, ptra); 

        ptra = set_disjunct(indentVarPtra, followers);
        accept2(Signs::leftpar, ptra);

        ptra = set_disjunct(indentRightparSemicVarPtra, followers);
        do
        {
            varDeclaration(ptra, scope, procIdent);
            if (symbolCode != Signs::rightpar) accept(Signs::semicolon);
        } while (!lexer->checkEof() && symbolCode == Identifiers::ident);

        ptra = set_disjunct(rightparSemicVarPtra, followers);
        accept2(Signs::rightpar, ptra);

        ptra = set_disjunct(semicVarPtra, followers);
        accept2(Signs::semicolon, ptra);

        ptra = set_disjunct(Keywords::getKeyWordCode("var"), followers);
        varPart(ptra);

        ptra = set_disjunct(Signs::semicolon, followers);
        statementPart(ptra);

        accept2(Signs::semicolon, followers);

        if (!belong(followers))
        {
            lexer->addNewError(6);
            skipto(followers);
        }
        while (identifiersList.back().second == scope) identifiersList.pop_back();
    }
}

void SyntacticSemanticAnalyzer::funcPart(vector<int> followers)
{
    vector<int> ptra;

    int dataType;
    int funcIdentIdx = -1;
    int scope;
    if (identifiersList.empty()) scope = 0;
    else scope = identifiersList[identifiersList.size() - 1].second + 1;

    if (!belong(st_procfuncpart))
    {
        lexer->addNewError(18);
        skipto2(st_varpart, followers);
    }

    if (symbolCode == Keywords::getKeyWordCode("function"))
    {
        accept(Keywords::getKeyWordCode("function"));

        ptra = set_disjunct(identLeftparColonVarPtra, followers);
        if (symbolCode == Identifiers::ident)
        {
            addNewIdentInList(new IdentifierClass(lexer->getIdentName(), TypeCodes::getTypeCode("void"), Keywords::getKeyWordCode("procedure")), scope++, 0);
            funcIdentIdx = identifiersList.size()-1;
        }
            
        accept2(Identifiers::ident, ptra);

        ptra = set_disjunct(leftparColonVarPtra, followers);
        accept2(Signs::leftpar, ptra);

        ptra = set_disjunct(indentRightparSemicVarPtra, followers);
        do
        {
            varDeclaration(ptra, scope, funcIdentIdx);
            if (symbolCode != Signs::rightpar) accept(Signs::semicolon);
        } while (!lexer->checkEof() && symbolCode == Identifiers::ident);

        ptra = set_disjunct(rightparSemicVarPtra, followers);
        accept2(Signs::rightpar, ptra);

        ptra = set_disjunct(colonSemicVarPtra, followers);
        accept2(Signs::colon, ptra);

        ptra = set_disjunct(Signs::semicolon, followers);
        dataType = type(ptra);
        if (funcIdentIdx != -1)
        {
            addNewIdentInList(new IdentifierClass("Result", dataType, 2), scope, funcIdentIdx);
            identifiersList[funcIdentIdx].first->setDataType(dataType);
        }

        ptra = set_disjunct(semicVarPtra, followers);
        accept2(Signs::semicolon, ptra);

        ptra = set_disjunct(Keywords::getKeyWordCode("var"), followers);
        varPart(ptra);

        ptra = set_disjunct(Signs::semicolon, followers);
        statementPart(ptra);

        accept2(Signs::semicolon, followers);

        if (!belong(followers))
        {
            lexer->addNewError(6);
            skipto(followers);
        }
        while (identifiersList.back().second == scope) identifiersList.pop_back();
    }
}


void SyntacticSemanticAnalyzer::statementPart(vector<int> followers)
{
    compoundStatement(followers);
}


void SyntacticSemanticAnalyzer::block(vector<int> followers)
{
    vector<int> ptra;

    if (!belong(begpart))
    {
        lexer->addNewError(18);
        skipto2(begpart, followers);
    }

    if (belong(begpart))
    {
        ptra = set_disjunct(st_procfuncpart, followers);

        varPart(ptra);
        procPart(ptra);
        funcPart(ptra);

        statementPart(followers);
        if (!belong(followers))
        {
            lexer->addNewError(6);
            skipto(followers);
        }
    }
}


void SyntacticSemanticAnalyzer::programme()
{
    accept(Keywords::getKeyWordCode("program"));
    accept(Identifiers::ident);
    accept(Signs::semicolon);
    block(vector<int>{Signs::point});
    accept(Signs::point);
}

int SyntacticSemanticAnalyzer::getErrorsCount() { return lexer->getErrorsCount(); }

SyntacticSemanticAnalyzer::~SyntacticSemanticAnalyzer()
{
    lexer->~LexicalAnalyzer();
}

