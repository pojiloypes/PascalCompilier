#include "PascalUtilitySymbols.h"

//------------КЛЮЧЕВЫЕ СЛОВА-------------------------------------------
map<int, map<string, int>> Keywords::kw = map<int, map<string, int>>();

int Keywords::getKeyWordCode(string word) 
{ 
    int code = 0;
    if (kw.count(word.length()) > 0 && kw[word.length()].count(word) > 0) code = kw[word.length()][word];

    return code;
}

void Keywords::run(string fileName)
{
    kw = map<int, map<string, int>>();
    ifstream kwFile = ifstream(fileName);
    if (!kwFile.is_open()) cout << "Возникли проблемы в файле с кодами всех символов" << endl;
    else
    {
        string name;
        int code;
        while (!kwFile.eof())
        {
            kwFile >> name >> code;
            kw[name.length() - 2][name.erase(name.length() - 2, 2)] = code;
        }
    }

}

map<string, int> TypeCodes::tc = map<string, int>();

int TypeCodes::getTypeCode(string word) 
{ 
    int code = 0;
    if (tc.count(word) > 0) code = tc[word];

    return code;
}

map<string, int> TypeCodes::getTypeCodes() { return tc; }


void TypeCodes::run(string fileName)
{
    tc = map<string, int>();
    ifstream tcFile = ifstream(fileName);
    if (!tcFile.is_open()) cout << "Возникли проблемы в файле с кодами всех символов" << endl;
    else
    {
        string name;
        int code;
        while (!tcFile.eof())
        {
            tcFile >> name >> code;
            tc[name.erase(name.length() - 2, 2)] = code;
        }
    }

}
