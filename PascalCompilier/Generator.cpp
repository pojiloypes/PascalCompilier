#include "Generator.h"

Generator::Generator(string fileName)
{
    SyntacticSemanticAnalyzer* syntaxer = new SyntacticSemanticAnalyzer(fileName);
    syntaxer->programme();
    errorsFlag = syntaxer->getErrorsCount() > 0;

    if (!errorsFlag)
    {
        varTable = syntaxer->getVarTable();
        calculator = new Calculator(varTable);
        lexer = new LexicalAnalyzer(fileName, true);
        nextSym();
    }
    syntaxer->~SyntacticSemanticAnalyzer();
}

void Generator::run()
{
    if (!errorsFlag)
    {
        cout << endl << "Результат работы: " << endl;
        nextSym(); // "program"
        nextSym(); // "program name"
        nextSym(); // ";"

        if (symbolCode == Keywords::getKeyWordCode("var"))
            varPart();
        if (symbolCode == Keywords::getKeyWordCode("begin"))
            block();
    }

}

bool Generator::checkForNum(string str)
{
    bool output = true;
    for (int i = 0; i < str.length(); i++)
    {
        if (!(str[i] >= '0' && str[i] <= '9')) output = false;
    }
    return output;
}

int Generator::getPriority(string str)
{
    if (str == "+" || str == "-")
        return -2;
    else if (str == "*" || str == "/")
        return -1;
    else if (str == "(")
        return -3;
    else if (str == ")")
        return -4;
    else return 0;
}

vector<string> Generator::translateToPostFix(vector<string> expression)
{
    vector <string> postFixed;
    stack <string> operands;

    for (int i = 0; i < expression.size(); i++)
    {
        string curElem = expression[i];
        int curPrior = getPriority(curElem);

        if (!checkForNum(curElem))
        {
            if (curPrior == -4)
            {
                while (getPriority(operands.top()) != -3)
                {
                    postFixed.push_back(operands.top());
                    operands.pop();
                }
                operands.pop();
            }
            else
            {
                while (!operands.empty() && (getPriority(operands.top()) >= curPrior && curPrior > -3))
                {
                    postFixed.push_back(operands.top());
                    operands.pop();
                }
                operands.push(curElem);
            }
        }
        else
        {
            postFixed.push_back(curElem);
        }
    }
    while (!operands.empty())
    {
        postFixed.push_back(operands.top());
        operands.pop();
    }
    return postFixed;
}

void Generator::nextSym()
{
    do
    {
        lexer->nextSym();
        symbolCode = lexer->getSymbolCode();
    } while (!lexer->checkEof() && symbolCode == 0);
}

void Generator::varDeclaration() /* анализ конструкции <описание однотипных переменных> */
{
    vector<string> varNames = vector<string>();
    varNames.push_back(lexer->getIdentName());
    nextSym();
    while (symbolCode == Signs::comma)
    {
        nextSym(); // ","
        varNames.push_back(lexer->getIdentName());
        nextSym(); // var name
    }
    nextSym(); // ":"
    nextSym(); // data type
    if (symbolCode == Signs::assign)
    {
        nextSym();
        for (int i = 0; i < varNames.size(); i++)
        {
            if (varTable[varNames[i]]->getDataType() == TypeCodes::getTypeCode("integer"))
                varTable[varNames[i]]->setVal(lexer->getIntConst());
            else if (varTable[varNames[i]]->getDataType() == TypeCodes::getTypeCode("float"))
                varTable[varNames[i]]->setVal(lexer->getFLoatConst());
            else if (varTable[varNames[i]]->getDataType() == TypeCodes::getTypeCode("string"))
                varTable[varNames[i]]->setVal(lexer->getStringConst());
            else if (varTable[varNames[i]]->getDataType() == TypeCodes::getTypeCode("char"))
                varTable[varNames[i]]->setVal(lexer->getCharConst());
        }
        nextSym();
    }
}

void Generator::varPart()
{
    if (symbolCode == Keywords::getKeyWordCode("var"))
    {
        nextSym(); // "var"
        do
        {
            varDeclaration();
            nextSym();
        } while (symbolCode == Identifiers::ident);
    }
}

string Generator::getSign()
{
    if (symbolCode == Signs::plus)
        return "+";
    else if (symbolCode == Signs::minus)
        return "-";
    else if (symbolCode == Signs::star)
        return "*";
    else if (symbolCode == Signs::slash)
        return "/";
    else if (symbolCode == Signs::leftpar)
        return "(";
    else if (symbolCode == Signs::rightpar)
        return ")";
}

pair<vector <string>, int> Generator::getExpVector()
{
    pair<vector <string>, int> expression = pair<vector <string>, int>();
    expression.second = -1;

    while (symbolCode != Signs::semicolon)
    {
        string curElem = "";
        if (symbolCode == Identifiers::ident)
        {
            curElem += lexer->getIdentName();
            if (expression.second == -1)
                expression.second = varTable[curElem]->getDataType();
        }
        else if (symbolCode == Identifiers::charc)
        {
            curElem += lexer->getCharConst();
            if (expression.second == -1)
                expression.second = TypeCodes::getTypeCode("char");
        }
        else if (symbolCode == Identifiers::intc)
        {
            curElem += to_string(lexer->getIntConst());
            if (expression.second == -1)
                expression.second = TypeCodes::getTypeCode("integer");
        }  
        else if (symbolCode == Identifiers::floatc)
        {
            curElem += to_string(lexer->getFLoatConst());
            if (expression.second == -1)
                expression.second = TypeCodes::getTypeCode("float");
        }
        else if (symbolCode == Identifiers::stringc)
        {
            curElem += lexer->getStringConst();
            if (expression.second == -1)
                expression.second = TypeCodes::getTypeCode("string");
        }
        else
            curElem += getSign();
        expression.first.push_back(curElem);
        nextSym();
    }
    return expression;
}


void Generator::writelnFunc()
{
    pair<vector <string>, int> expression = getExpVector();
    expression.first.erase(expression.first.begin() + (expression.first.size() - 1));
    vector<string> postFixed = translateToPostFix(expression.first);
    cout << calculator->getResByString(postFixed, expression.second) << endl;
}

void Generator::block()
{
    nextSym(); // "begin"

    while (symbolCode != Keywords::getKeyWordCode("end"))
    {
        if (symbolCode == Keywords::getKeyWordCode("writeln"))
        {
            nextSym(); // "writeln"
            nextSym(); // "("
            writelnFunc();
            nextSym(); // ";"
        }
        else
        {
            string varName = lexer->getIdentName();
            nextSym(); // var name
            nextSym(); // ":="

            pair<vector <string>, int> expression = getExpVector();
            vector <string> postFixed = translateToPostFix(expression.first);

            if (varTable[varName]->getDataType() == TypeCodes::getTypeCode("integer"))
                varTable[varName]->setVal(calculator->calculateInteger(postFixed));
            else if (varTable[varName]->getDataType() == TypeCodes::getTypeCode("float"))
                varTable[varName]->setVal(calculator->calculateFloat(postFixed));
            else if (varTable[varName]->getDataType() == TypeCodes::getTypeCode("string"))
                varTable[varName]->setVal(calculator->calculateString(postFixed));
            else if (varTable[varName]->getDataType() == TypeCodes::getTypeCode("char"))
                varTable[varName]->setVal(calculator->calculateChar(postFixed));
            nextSym(); // ";"
        }
    }
    nextSym(); // "end"
    nextSym();
}
