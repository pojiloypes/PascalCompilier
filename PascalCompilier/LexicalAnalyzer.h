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
    LexicalAnalyzer(string fName, bool readFlag); // �����������

    ~LexicalAnalyzer(); // ����������

    bool checkEof(); // �������� ��� �� ����� ���������

    int getSymbolCode(); // ������� ��� �������� �������

    void nextSym(); // ������� ����� ������

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

    bool checkStopReading(); // �������� ����� �� ����� ����� ��� �� ����� ������, ����� ����� � �� � �� �������� ����� ����������� ������

    int checkUtilityWord(string word);

    void readNum(bool positivityFalg); // ������ �����

    void readWord(); // ������� �����

    void readSym(char curChar); // ������� ����� � ������

};

