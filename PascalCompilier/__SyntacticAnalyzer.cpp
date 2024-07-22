//class SyntacticAnalyzer
//{
//public:
//    int symbolCode;
//    LexicalAnalyzer* lexer;
//
//
//    SyntacticAnalyzer(string fileName)
//    {
//        lexer = new LexicalAnalyzer(fileName);
//        lexer->nextSym();
//        symbolCode = lexer->getSymbolCode();
//    }
//
//    void nextSym()
//    {
//        lexer->nextSym();
//        symbolCode = lexer->getSymbolCode();
//
//    }
//
//    void accept(int symbolExpected)
//    {
//        if (symbolCode == symbolExpected) nextSym();
//        else lexer->addNewError(993);
//        //if (symbolCode != symbolExpected) lexer->addNewError(993);
//        //nextSym();
//    }
//
//
//    void type()
//    {
//        map<string, int> tc = TypeCodes::getTypeCodes();
//
//        bool flag = false;
//        for (map<string, int>::iterator it = tc.begin(); it != tc.end(); it++) if (symbolCode == it->second) flag = true;
//        if (flag) nextSym();
//        else lexer->addNewError(993);
//
//    }
//
//
//    void varDeclaration() /* анализ конструкции <описание однотипных переменных> */
//    {
//        accept(Identifiers::ident);
//        while (symbolCode == Signs::comma)
//        {
//            nextSym();
//            accept(Identifiers::ident);
//        }
//        accept(Signs::colon);
//        type();
//    }
//
//    void variable() /* анализ конструкции <переменная> */
//    {
//        accept(Identifiers::ident);
//        while (symbolCode == Signs::lbracket || symbolCode == Signs::point
//            || symbolCode == Signs::arrow)
//            switch (symbolCode)
//            {
//            case Signs::lbracket:
//                nextSym(); expression();
//                while (symbolCode == Signs::comma)
//                {
//                    nextSym(); expression();
//                }
//                accept(Signs::rbracket);
//                break;
//            case Signs::point:
//                nextSym(); accept(Identifiers::ident);
//                break;
//            case Signs::arrow:
//                nextSym();
//                break;
//            }
//    }
//
//    void varPart()  /* анализ конструкции <раздел переменных> */
//    {
//        if (symbolCode == Keywords::getKeyWordCode("var"))
//        {
//            accept(Keywords::getKeyWordCode("var"));
//            do
//            {
//                varDeclaration();
//                accept(Signs::semicolon);
//            } while (symbolCode == Identifiers::ident);
//        }
//    }
//
//    bool isItConst()
//    {
//        return symbolCode == Identifiers::booleanc || symbolCode == Identifiers::charc ||
//            symbolCode == Identifiers::floatc || symbolCode == Identifiers::intc ||
//            symbolCode == Identifiers::stringc;
//    }
//
//    void expression()
//    {
//        while (symbolCode == Identifiers::ident || isItConst())
//        {
//            nextSym();
//            switch (symbolCode)
//            {
//            case Signs::star:
//                accept(Signs::star);
//                break;
//            case Signs::slash:
//                accept(Signs::slash);
//                break;
//            case Signs::plus:
//                accept(Signs::plus);
//                break;
//            case Signs::minus:
//                accept(Signs::minus);
//                break;
//            case Signs::laterequal:
//                accept(Signs::laterequal);
//                break;
//            case Signs::greaterequal:
//                accept(Signs::greaterequal);
//                break;
//            case Signs::latergreater:
//                accept(Signs::latergreater);
//                break;
//            }
//        }
//        accept(Signs::semicolon);
//    }
//
//    void statement()
//    {
//        accept(Identifiers::ident);
//        accept(Signs::assign);
//        expression();
//    }
//
//    void body()
//    {
//        accept(Keywords::getKeyWordCode("begin"));
//        while (symbolCode == Keywords::getKeyWordCode("var"))
//        {
//            accept(Keywords::getKeyWordCode("var"));
//            varDeclaration();
//            accept(Signs::semicolon);
//        }
//        if (symbolCode == Identifiers::ident || isItConst()) statement();
//
//        accept(Keywords::getKeyWordCode("end"));
//    }
//
//    void procPart()
//    {
//        if (symbolCode == Keywords::getKeyWordCode("procedure"))
//        {
//            accept(Keywords::getKeyWordCode("procedure"));
//            accept(Identifiers::ident);
//            accept(Signs::leftpar);
//
//            do
//            {
//                varDeclaration();
//                if (symbolCode != Signs::rightpar) accept(Signs::semicolon);
//            } while (symbolCode == Identifiers::ident);
//
//            accept(Signs::rightpar);
//            accept(Signs::semicolon);
//
//            varPart();
//            body();
//            accept(Signs::semicolon);
//        }
//    }
//
//    void funcPart()
//    {
//        if (symbolCode == Keywords::getKeyWordCode("function"))
//        {
//            accept(Keywords::getKeyWordCode("function"));
//            accept(Identifiers::ident);
//            accept(Signs::leftpar);
//
//            do
//            {
//                varDeclaration();
//                if (symbolCode != Signs::rightpar) accept(Signs::semicolon);
//            } while (symbolCode == Identifiers::ident);
//
//            accept(Signs::rightpar);
//            accept(Signs::colon);
//            type();
//            accept(Signs::semicolon);
//
//            varPart();
//            body();
//            accept(Signs::semicolon);
//        }
//    }
//
//    void block()
//    {
//        /* анализ конструкции <блок> */
//        while (symbolCode != Keywords::getKeyWordCode("begin"))
//        {
//            varPart();
//            procPart();
//            funcPart();
//        }
//
//        body();
//        accept(Signs::point);
//        //procfuncpart();
//    }
//
//    void programme()
//        /* анализ конструкции <программа> */
//    {
//        accept(Keywords::getKeyWordCode("program"));
//        accept(Identifiers::ident);
//        accept(Signs::semicolon);
//        block(); /* анализ конструкции <блок> */
//
//
//    }
//
//    ~SyntacticAnalyzer()
//    {
//        lexer->~LexicalAnalyzer();
//    }
//};