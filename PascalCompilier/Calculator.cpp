#include "Calculator.h"

Calculator::Calculator(map<string, VariableClass*> newTable)
{
    varTable = newTable;
}


int Calculator::calc(int x, int y, char sym)
{
    int res = 0;
    switch (sym)
    {
    case '+':
        res = x + y;
        break;
    case '-':
        res = x - y;
        break;
    case '/':
        res = x / y;
        break;
    case '*':
        res = x * y;
        break;
    }
    return res;
}

double Calculator::calc(double x, double y, char sym)
{
    double res = 0;
    switch (sym)
    {
    case '+':
        res = x + y;
        break;
    case '-':
        res = x - y;
        break;
    case '/':
        res = x / y;
        break;
    case '*':
        res = x * y;
        break;
    }
    return res;
}

char Calculator::calc(char x, char y, char sym)
{
    char res = 0;
    switch (sym)
    {
    case '+':
        res = x + y;
        break;
    case '-':
        res = x - y;
        break;
    case '/':
        res = x / y;
        break;
    case '*':
        res = x * y;
        break;
    }
    return res;
}

string Calculator::calc(string x, string y, char sym)
{
    string res = "";
    switch (sym)
    {
    case '+':
        res = x + y;
        break;
    }
    return res;
}

bool Calculator::isSign(string c)
{
    return c == "+" || c == "-" || c == "/" || c == "*" || c == "(";
}

int Calculator::calculateInteger(vector <string> postFixed)
{
    stack<int> st = stack<int>();
    string num = "";
    char sign;

    for (int i = 0; i < postFixed.size(); i++)
    {
        if (isSign(postFixed[i]))
        {
            sign = postFixed[i][0];
            int x = st.top(); st.pop();
            int y = st.top(); st.pop();

            st.push(calc(y, x, sign));
        }
        else
        {
            if (isdigit(postFixed[i][0]))
                st.push(stoi(postFixed[i]));
            else
                st.push(varTable[postFixed[i]]->getIntegerVal());
        }

    }
    return st.top();
}

double Calculator::calculateFloat(vector <string> postFixed)
{
    stack<double> st = stack<double>();
    string num = "";
    char sign;

    for (int i = 0; i < postFixed.size(); i++)
    {
        if (isSign(postFixed[i]))
        {
            sign = postFixed[i][0];
            double x = st.top(); st.pop();
            double y = st.top(); st.pop();

            st.push(calc(y, x, sign));
            sign = '1';
        }
        else
        {
            if (isdigit(postFixed[i][0]))
                st.push(stod(postFixed[i]));
            else
                st.push(varTable[postFixed[i]]->getFloatVal());
        }

    }
    return st.top();
}

string Calculator::calculateString(vector <string> postFixed)
{
    stack<string> st = stack<string>();
    string num = "";
    char sign;

    for (int i = 0; i < postFixed.size(); i++)
    {
        if (isSign(postFixed[i]))
        {
            sign = postFixed[i][0];
            string x = st.top(); st.pop();
            string y = st.top(); st.pop();

            st.push(calc(y, x, sign));
            sign = '1';
        }
        else
        {
            if (varTable.count(postFixed[i]) == 0)
                st.push(postFixed[i]);
            else
                st.push(varTable[postFixed[i]]->getStringVal());
        }

    }
    return st.top();
}

char Calculator::calculateChar(vector<string> postFixed)
{
    stack<char> st = stack<char>();
    string num = "";
    char sign;

    for (int i = 0; i < postFixed.size(); i++)
    {
        if (isSign(postFixed[i]))
        {
            sign = postFixed[i][0];
            char x = st.top(); st.pop();
            char y = st.top(); st.pop();

            st.push(calc(y, x, sign));
            sign = '1';
        }
        else
        {
            if (varTable.count(postFixed[i]) == 0)
                st.push(postFixed[i][0]);
            else
                st.push(varTable[postFixed[i]]->getCharVal());
        }

    }
    return st.top();
}

string Calculator::getResByString(vector <string> postFixed, int dataType)
{
    if (dataType == TypeCodes::getTypeCode("integer"))
        return to_string(calculateInteger(postFixed));
    else if (dataType == TypeCodes::getTypeCode("float"))
        return to_string(calculateFloat(postFixed));
    else if (dataType == TypeCodes::getTypeCode("char"))
        return to_string(calculateChar(postFixed));
    else if (dataType == TypeCodes::getTypeCode("string"))
        return calculateString(postFixed);
}