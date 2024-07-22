#include "LexicalAnalyzer.h"

using namespace std;


LexicalAnalyzer::LexicalAnalyzer(string fName, bool readFlag)
{
    iom = new InputOutputModule(fName, readFlag);
    if (!iom->getEofFlag()) iom->nextCh();
    Keywords::run("Keywords.txt"); // считать ключевые слова из файла в класс KeyWords
    TypeCodes::run("TypeCodes.txt");
    identName = "";
    onlyReadFlag = readFlag;
}

bool LexicalAnalyzer::checkEof() { return iom->getEofFlag(); } // дублирует проверку на конец чтения из InputOutputModule

int LexicalAnalyzer::getSymbolCode() { return symbolCode; } 

int LexicalAnalyzer::getErrorsCount() { return iom->getErrorsCount(); }

void LexicalAnalyzer::addNewError(int errorCode) { iom->addNewError(errorCode, iom->getCurPos()); }

int LexicalAnalyzer::getIntConst() { return intConst; }

double LexicalAnalyzer::getFLoatConst() { return doubleConst; }

string LexicalAnalyzer::getStringConst() { return strConst; }

char LexicalAnalyzer::getCharConst() { return chConst; }

void LexicalAnalyzer::nextSym()
{
    while (!iom->getEofFlag() && (iom->getCurChar() == ' ' || iom->getCurChar() == '\0' || iom->getCurChar() == '\t')) iom->nextCh();
    token = iom->getCurPos();
    char curChar = iom->getCurChar();

    switch (CharacterIdentifier::getCharCode(curChar))
    {
    case 1:
        readNum(true);
        break;

    case 2:
        readWord();
        break;

    case 3:
        readSym(curChar);
        break;
    }
}

string LexicalAnalyzer::getIdentName() { return identName; }

bool LexicalAnalyzer::checkStopReading() 
{ 
    return !iom->getEofFlag() && token.lineNum == iom->getCurPos().lineNum; // false - не дошли ни до конца строки ни до конца файла.
} 

int LexicalAnalyzer::checkUtilityWord(string word)
{
    return  Keywords::getKeyWordCode(word) + TypeCodes::getTypeCode(word);
}

void LexicalAnalyzer::readNum(bool positivityFalg)
{
    string num = "";

    while (checkStopReading() &&
        CharacterIdentifier::isDigit(iom->getCurChar()))
    {
        num += iom->getCurChar();
        iom->nextCh();
    }

    if (checkStopReading() && iom->getCurChar() == '.')
    {
        num += iom->getCurChar(); iom->nextCh();
        while (checkStopReading() &&
            CharacterIdentifier::isDigit(iom->getCurChar()))
        {
            num += iom->getCurChar();
            iom->nextCh();
        }
        try
        {
            doubleConst = stod(num);
            if (!positivityFalg) doubleConst *= -1;
            symbolCode = Identifiers::floatc;
        }
        catch (...)
        {
            iom->addNewError(301, iom->getCurPos());
            doubleConst = 0;
        }
    }
    else
    {
        try
        {
            intConst = stoi(num);
            if (!positivityFalg) intConst *= -1;
            symbolCode = Identifiers::intc;
        }
        catch (...)
        {
            iom->addNewError(302, iom->getCurPos());
            intConst = 0;
        }
    }
}

void LexicalAnalyzer::readWord()
{
    string word = "";
    while (checkStopReading() &&
        (CharacterIdentifier::isDigit(iom->getCurChar()) ||
            CharacterIdentifier::isLetter(iom->getCurChar())))
    {
        word += iom->getCurChar();
        iom->nextCh();
    }
    if (checkUtilityWord(word) == 0)
    {
        symbolCode = Identifiers::ident;
        identName = word;
    }
    else if (word == "True" or word == "False") symbolCode = Identifiers::booleanc;
    else symbolCode = checkUtilityWord(word);
}

void LexicalAnalyzer::readSym(char curChar)
{
    switch (curChar)
    {
    case '<':
        iom->nextCh();
        curChar = iom->getCurChar();

        if (checkStopReading() && curChar == '=')
        {
            symbolCode = Signs::laterequal;
            iom->nextCh();
        }
        else if (token.lineNum == iom->getCurPos().lineNum && curChar == '>')
        {
            symbolCode = Signs::latergreater;
            iom->nextCh();
        }
        else symbolCode = Signs::later;
        break;

    case '>':
        iom->nextCh();
        curChar = iom->getCurChar();

        if (checkStopReading() && curChar == '=')
        {
            symbolCode = Signs::greaterequal;
            iom->nextCh();
        }
        else symbolCode = Signs::greater;
        break;

    case ':':
        iom->nextCh();
        curChar = iom->getCurChar();

        if (checkStopReading() && curChar == '=')
        {
            symbolCode = Signs::assign;
            iom->nextCh();
        }
        else symbolCode = Signs::colon;
        break;

    case '+':
        symbolCode = Signs::plus;
        iom->nextCh();
        break;

    case '-':
        symbolCode = Signs::minus;
        iom->nextCh();
        break;

    case '*':
        symbolCode = Signs::star;
        iom->nextCh();
        break;

    case '=':
        symbolCode = Signs::equal;
        iom->nextCh();
        break;
        
    case '/':
        iom->nextCh();
        curChar = iom->getCurChar();

        if (checkStopReading() && curChar == '/')
        {
            while (checkStopReading()) iom->nextCh();
            symbolCode = 0;
            //iom->nextCh();
        }

        else symbolCode = Signs::slash;
        //nextSym();
        break;
        
    case '\'':
        iom->nextCh();
        chConst = iom->getCurChar();
        iom->nextCh();

        if (iom->getCurChar() == '\'') symbolCode = Identifiers::charc;
        else
        {
            strConst = "";
            strConst += chConst;
            while (checkStopReading() && iom->getCurChar() != '\'')  
            {
                strConst += iom->getCurChar();
                iom->nextCh();
            }
            if (!checkStopReading())
            {
                iom->addNewError(80, iom->getCurPos());
                iom->listErrors();
                symbolCode = 0;
            }
            else symbolCode = Identifiers::stringc;
        }
        iom->nextCh();
        break;

    case ';':
        symbolCode = Signs::semicolon;
        iom->nextCh();
        break;

    case '.':
        iom->nextCh();
        if (checkStopReading() && iom->getCurChar() == '.')
        {
            symbolCode = Signs::twopoints;
            iom->nextCh();
        }
        else symbolCode = Signs::point;
        break;

    case ',':
        symbolCode = Signs::comma;
        iom->nextCh();
        break;

    case '(':
        iom->nextCh();
        if (checkStopReading() && iom->getCurChar() == '*')
        {
            iom->nextCh();
            char prevChar = iom->getCurChar();
            iom->nextCh();

            while (!iom->getEofFlag() && !(iom->getCurChar() == ')' && prevChar == '*'))
            {
                prevChar = iom->getCurChar();
                iom->nextCh();
            }
            if (!(iom->getCurChar() == ')' && prevChar == '*'))
            {
                iom->addNewError(95, iom->getCurPos());
                iom->listErrors();
            }
            else iom->nextCh();
            symbolCode = 0;
        }
        else
        {
            symbolCode = Signs::leftpar;
            //iom->nextCh();
        }
        break;

    case ')':
        symbolCode = Signs::rightpar;
        iom->nextCh();
        break;

    case '[':
        symbolCode = Signs::lbracket;
        iom->nextCh();
        break;

    case ']':
        symbolCode = Signs::rbracket;
        iom->nextCh();
        break;

    case '{':
            iom->nextCh();
            while (!iom->getEofFlag() && iom->getCurChar() != '}') iom->nextCh();

            if (iom->getCurChar() != '}')
            {
                iom->addNewError(95, iom->getCurPos());
                iom->listErrors();
            }
            else iom->nextCh();
            symbolCode = 0;
        
        break;

    case '}':
        symbolCode = Signs::frpar;
        iom->nextCh();
        break;

    case '?':
        iom->addNewError(12, iom->getCurPos());
        iom->nextCh();
        symbolCode = 0;
        break;

    case '&':
        iom->addNewError(12, iom->getCurPos());
        iom->nextCh();
        symbolCode = 0;
        break;

    case '%':
        iom->addNewError(12, iom->getCurPos());
        iom->nextCh();
        symbolCode = 0;
        break;

    case '$':
        iom->addNewError(12, iom->getCurPos());
        iom->nextCh();
        symbolCode = 0;
        break;

    case '!':
        iom->addNewError(12, iom->getCurPos());
        iom->nextCh();
        symbolCode = 0;
        break;
    }
}

LexicalAnalyzer::~LexicalAnalyzer()
{
    iom->end();
}

