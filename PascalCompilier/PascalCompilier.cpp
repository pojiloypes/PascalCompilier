#include "Generator.h"

int main()
{
    //setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    
    Generator* g = new Generator("pascalProgram.txt");
    g->run();
}

