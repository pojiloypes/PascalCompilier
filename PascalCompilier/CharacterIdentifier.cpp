#include "CharacterIdentifier.h"

bool CharacterIdentifier::isDigit(char ch)
{
    bool ans = false;
    if (ch >= '0' && ch <= '9') ans = true;
    return ans;
}
bool CharacterIdentifier::isLetter(char ch)
{
    bool ans = false;
    if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z') ans = true;
    return ans;
}

int CharacterIdentifier::getCharCode(char ch)
{
    if (isDigit(ch)) return 1;
    else if (isLetter(ch)) return 2;
    else return 3;
}
