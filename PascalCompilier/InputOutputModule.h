#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include <thread>
#include "Error.h"

using namespace std;

class InputOutputModule
{
private:
    ifstream programFile;
    TextPosition positionNow;
    vector<Error> errors;
    string curLine;
    int fixedErrorsCount;
    int ERRMAX;
    char curChar;
    bool endOfFileFlag;
    bool onlyReadFlag;

public:
    InputOutputModule(string fileName, bool readFlag);

    void nextCh();

    bool getEofFlag();

    char getCurChar();

    int getErrorsCount();

    TextPosition getCurPos();

    void addNewError(int errorCode, TextPosition pos);

    void end();

    void listErrors();

private:

    void generateRandomError();

    void readNextLine();

    void listThisLine();

};
