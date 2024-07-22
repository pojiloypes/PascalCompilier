#include <float.h>
#include <map>
#include <fstream>
#include <string>
#include "CharacterIdentifier.h"
#include "PascalUtilitySymbols.h"
#include "InputOutputModule.h"

using namespace std;

class LexicalAnalyzer
{
public:
    LexicalAnalyzer(string fName, bool readFlag); // конструктор

    ~LexicalAnalyzer(); // Деструктор

    bool checkEof(); // проверка все ли числа прочитаны

    int getSymbolCode(); // считать код текущего символа

    void nextSym(); // считать новый символ

    void addNewError(int errorCode);

    int getErrorsCount();

    int getIntConst();

    double getFLoatConst();

    string getStringConst();

    char getCharConst();

    string getIdentName();

private:
    InputOutputModule* iom; 
    TextPosition token;
    int symbolCode;
    int intConst;
    double doubleConst;
    string strConst;
    string identName;
    char chConst;
    bool onlyReadFlag;

    bool checkStopReading(); // проверка дошли до конца файла или до новой строки, почти везде и то и то означает конец постоянного чтения

    int checkUtilityWord(string word);

    void readNum(bool positivityFalg); // читать число

    void readWord(); // считать слово

    void readSym(char curChar); // считать знаки и смволы

};

