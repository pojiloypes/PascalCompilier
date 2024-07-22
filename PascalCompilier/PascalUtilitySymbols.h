#include <map>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

//--- лючевые слова-------------
class Keywords
{
private:
    static map<int, map<string, int>> kw;


public:

    static int getKeyWordCode(string word);

    static void run(string fileName);
};

class TypeCodes
{
private:
    static map<string, int> tc;


public:

    static  map<string, int> getTypeCodes();

    static int getTypeCode(string word);

    static void run(string fileName);
};

//-----«наки-------------
struct Signs
{
public:
    static const int
       star = 21, // *
       slash = 60, // /
       equal = 16, // =
       comma = 20, // ,
       semicolon = 14, // ;
       colon = 5, // :
       point = 61,	// .
       arrow = 62,	// ^
       leftpar = 9,	// (
       rightpar = 4,	// )
       lbracket = 11,	// [
       rbracket = 13,	// ]
       flpar = 63,	// {
       frpar = 64,	// }
       later = 65,	// <
       greater = 66,	// >
       laterequal = 67,	//  <=
       greaterequal = 68,	//  >=
       latergreater = 69,	//  <>
       plus = 70,	// +
       minus = 71,	// Ц
       lcomment = 72,	//  (*
       rcomment = 73,	//  *)
       assign = 51,	//  :=
       twopoints = 74;
};

//---»дентификаторы и константы-------------
struct Identifiers
{
public:
    static const int
        ident = 2,// идентификатор
        floatc = 82,	// вещественна€ константа
        intc = 15,	// цела€ константа
        charc = 1001,
        stringc = 1002,
        booleanc = 1003,
        realc = 1004;
};
