#include "TextPosition.h"
#include <string>

using namespace std;

struct Error
{
public:
    int errorCode;
    TextPosition position;

    Error(TextPosition errorPos, int errorCode);

    string decodeError();
}; 
