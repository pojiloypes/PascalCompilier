#include "InputOutputModule.h"

InputOutputModule::InputOutputModule(string fileName, bool readFlag)
{
    programFile = ifstream(fileName);
    if (!programFile.is_open())
    {
        cout << "Ошибка, исходный файл не найден" << endl;
        endOfFileFlag = true;
    }
    else
    {
        endOfFileFlag = false;
        readNextLine();
        positionNow.charNum = -1;
        positionNow.lineNum = 0;
        fixedErrorsCount = 0;
        ERRMAX = 9;
        onlyReadFlag = readFlag;
    }

}

bool InputOutputModule::getEofFlag() { return endOfFileFlag; /*return programFile.eof() && positionNow.charNum == curLine.length();*/ }

char InputOutputModule::getCurChar() { return curChar; }

TextPosition InputOutputModule::getCurPos() { return positionNow; }

int InputOutputModule::getErrorsCount() { return fixedErrorsCount; }

void InputOutputModule::nextCh()
{
    if (positionNow.charNum == curLine.length())
    {
        if (!onlyReadFlag)
            listThisLine();
        if (errors.size() > 0 && !onlyReadFlag) listErrors();

        readNextLine();
        positionNow.charNum = 0;
        positionNow.lineNum++;

    }
    else positionNow.charNum++;

    curChar = curLine[positionNow.charNum];
}

void InputOutputModule::addNewError(int errorCode, TextPosition pos)
{
    if (errors.size() <= ERRMAX) errors.push_back(Error(pos, errorCode));
}

void InputOutputModule::generateRandomError()
{
    if (rand() % 2 == 0) addNewError(rand() % 30, TextPosition(positionNow.lineNum, rand() % (curLine.length() + 1)));
    this_thread::sleep_for(chrono::milliseconds(10));
}

void InputOutputModule::readNextLine()
{
    if (!programFile.eof())
    {
        getline(programFile, curLine);
        errors.clear();
        //generateRandomError();
    }
    else
    {
        endOfFileFlag = true;
        //end();
    }
}

void InputOutputModule::end()
{
    if (!onlyReadFlag)
        cout << "Компиляция завершена. \nВсего ошибок: " << to_string(fixedErrorsCount) << endl;
}

void InputOutputModule::listErrors()
{
    string s;
    int pos = 6 - to_string(positionNow.charNum).length();

    for (vector<Error>::iterator err = errors.begin(); errors.size()>0; err = errors.erase(err))
    {
        fixedErrorsCount++;
        s = "**";
        if (fixedErrorsCount < 10) s += "0";
        s += to_string(fixedErrorsCount);

        while (s.length()-1 < pos + err->position.charNum) s += " ";
        s += "^ ошибка код " + to_string(err->errorCode) + ". ";
        cout << s;
        cout << err->decodeError();
        
    }
}

void InputOutputModule::listThisLine() { cout << "      " << curLine << endl; }

