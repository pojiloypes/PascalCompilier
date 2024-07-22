#include "SyntacticAnalyzer.h"

SyntacticAnalyzer::SyntacticAnalyzer(string fileName)
{
    lexer = new LexicalAnalyzer(fileName, true);
    lexer->nextSym();
    symbolCode = lexer->getSymbolCode();

    begpart = vector<int>{ Keywords::getKeyWordCode("var"), Keywords::getKeyWordCode("function"), Keywords::getKeyWordCode("procedure"), Keywords::getKeyWordCode("begin") };
    st_varpart = vector<int>{ Keywords::getKeyWordCode("var"), Keywords::getKeyWordCode("function"), Keywords::getKeyWordCode("procedure"), Keywords::getKeyWordCode("begin") };
    st_procfuncpart = vector<int>{ Keywords::getKeyWordCode("function"), Keywords::getKeyWordCode("procedure"), Keywords::getKeyWordCode("begin") };
    after_var = vector<int>{ Signs::semicolon };
    after_proc = vector<int>{ Signs::semicolon };
}

void SyntacticAnalyzer::SyntacticAnalyzer::nextSym()
{
    do
    {
        lexer->nextSym();
        symbolCode = lexer->getSymbolCode();
    } while (!lexer->checkEof() && symbolCode == 0);

}

void SyntacticAnalyzer::accept(int symbolExpected)
{
    if (symbolCode == symbolExpected) nextSym();
    else lexer->addNewError(993);
}

void SyntacticAnalyzer::accept2(int symbolExpected, vector<int> followers)
{
    if (symbolCode != symbolExpected)
    {
        lexer->addNewError(symbolExpected);
        skipto(followers);
    }
    if (symbolCode == symbolExpected) nextSym();
}

void SyntacticAnalyzer::skipto(vector<int> vec)
{
    while (!lexer->checkEof() && (find(vec.begin(), vec.end(), symbolCode) == vec.end()))
    {
        nextSym();
        symbolCode = lexer->getSymbolCode();
    }
}

void SyntacticAnalyzer::skipto2(vector<int> vec1, vector<int> vec2)
{
    while (!lexer->checkEof() && (find(vec1.begin(), vec1.end(), symbolCode) == vec1.end() &&
        find(vec2.begin(), vec2.end(), symbolCode) == vec2.end()))
    {
        nextSym();
        symbolCode = lexer->getSymbolCode();
    }
}

bool SyntacticAnalyzer::belong(vector<int> vec)
{
    bool ans = false;
    for (int i = 0; i < vec.size(); i++) if (symbolCode == vec[i]) ans = true;
    return ans;
}

vector<int> SyntacticAnalyzer::set_disjunct(vector<int> vec1, vector<int> vec2)
{
    vector<int> res;
    res.reserve(vec1.size() + vec2.size());
    res.insert(res.end(), vec1.begin(), vec1.end());
    res.insert(res.end(), vec2.begin(), vec2.end());
    return res;
}

void SyntacticAnalyzer::type(vector<int> followers)
{
    map<string, int> tc = TypeCodes::getTypeCodes();
    id_starters.clear();
    for (map<string, int>::iterator it = tc.begin(); it != tc.end(); it++) id_starters.push_back(it->second);

    if (!belong(id_starters))
    {
        lexer->addNewError(993); /* должно идти имя */
        skipto2(id_starters, followers);
    }

    if (belong(id_starters))
    {
        nextSym();
        if (!belong(followers))
        {
            lexer->addNewError(6); /* запрещенный символ */
            skipto(followers);
        }
    }
}


void SyntacticAnalyzer::varDeclaration(vector<int> followers) /* анализ конструкции <описание однотипных переменных> */
{
    id_starters.clear();
    id_starters.push_back(Identifiers::ident);

    if (!belong(id_starters))
    {
        lexer->addNewError(2); /* должно идти имя */
        skipto2(id_starters, followers);
    }
    if (symbolCode == Identifiers::ident)
    {
        nextSym();
        while (!lexer->checkEof() && symbolCode == Signs::comma)
        {
            nextSym();
            accept(Identifiers::ident);
        }
        accept(Signs::colon);
        type(followers);
        if (!belong(followers))
        {
            lexer->addNewError(6); /* запрещенный символ */
            skipto(followers);
        }
    }
}


bool SyntacticAnalyzer::isItConst() // возвращает тру если текущий символ это либо символ символьной константы, либо строковой константы, либо целочисленной константы и т.д. ,если нет то возвращает фолс
{
    return symbolCode == Identifiers::booleanc || symbolCode == Identifiers::charc ||
        symbolCode == Identifiers::floatc || symbolCode == Identifiers::intc ||
        symbolCode == Identifiers::stringc;
}


void SyntacticAnalyzer::expression(vector<int> followers) // обрабатывает выражение вида 23 + 'dawd' - 1 * d / 56 - True
/*работает не смовсем верно так как позволяет в одном вырадении находится и логическим константам и числам и строкам и символам, но я подумал что такие ошибки выявлять будет уже следующий модуль*/
{
    if (symbolCode != Identifiers::ident && !isItConst() && symbolCode != Signs::leftpar) // выражение может начинаться с константы, с переменной либо с "("
    {
        lexer->addNewError(18);
        vector<int> identifiers = vector<int>{ Identifiers::ident,  Identifiers::booleanc, Identifiers::charc,
                                                Identifiers::floatc, Identifiers::intc, Identifiers::stringc };
        skipto2(identifiers, followers); // пропускаем пока не дойдем либо до константы либо до переменной либо до "(" либо до того что должно стоять после выражения
    }
    else
    {
        bool operatorNotFoundFlag = false;
        while (!lexer->checkEof() && !operatorNotFoundFlag && (symbolCode == Identifiers::ident || isItConst() || symbolCode == Signs::leftpar))
        {

            if (symbolCode == Signs::leftpar)
            {
                accept(Signs::leftpar);
                expression(set_disjunct(followers, vector<int> {Signs::rightpar}));
                accept(Signs::rightpar);
            }
            else nextSym();

            switch (symbolCode)
            {
            case Signs::star:
                accept(Signs::star);
                break;
            case Signs::slash:
                accept(Signs::slash);
                break;
            case Signs::plus:
                accept(Signs::plus);
                break;
            case Signs::minus:
                accept(Signs::minus);
                break;
            case Signs::laterequal:
                accept(Signs::laterequal);
                break;
            case Signs::greaterequal:
                accept(Signs::greaterequal);
                break;
            case Signs::latergreater:
                accept(Signs::latergreater);
                break;
            default:
                operatorNotFoundFlag = true;
                break;
            }
        }
        if (!belong(followers))
        {
            lexer->addNewError(6); /* запрещенный символ */
            skipto(followers);
        }
    }
}
void SyntacticAnalyzer::assignmentStatement(vector<int> followers)
{
    vector<int> ptra = vector<int>{ Identifiers::ident, Signs::semicolon, Signs::assign };

    accept2(Identifiers::ident, ptra); // аксептим идентификатор либо скипаем все символы пока не найдем что-то что должно стоять после него
    if (symbolCode != Signs::assign) // assign = ":=" - оператор присваивания
    {
        lexer->addNewError(Signs::assign);
        skipto2(vector<int> {Signs::assign}, followers);
    }

    if (symbolCode == Signs::assign)
    {

        accept(Signs::assign);
        expression(followers); // функция обработки выражения
        accept(Signs::semicolon);
        if (!belong(followers))
        {
            lexer->addNewError(6); /* запрещенный символ */
            skipto(followers);
        }
    }
}

void SyntacticAnalyzer::statement(vector<int> followers) // обрабатывает все возможные толи выраженяи толи опреаторы я запутался если честно
{
    vector<int> ptra;
    switch (symbolCode)
    {
    case Identifiers::ident: // операция присваивания начинается с переменной по ээтому если стоим на переменной то вызываем обарботку операции присванивания
        ptra = set_disjunct(vector<int> {Identifiers::ident, Signs::semicolon}, followers); // после одного выражения должна стоять ";", если ее нет то может встретиться либо новый идентификатор либо то что должно стоять после строки с операцией присваивания в целом
        assignmentStatement(ptra); // вызов функции обарботки операции присванивания

        break;
    case 113: // код символа begin, в плюсах нельзя юзать в кейсе не константы, потому вот так выкрутился
        compoundStatement(followers);
        break;
    default:

        break;
    }
    if (!belong(set_disjunct(vector<int> {Identifiers::ident}, followers)))
    {
        lexer->addNewError(6); /* запрещенный символ */
        skipto(followers);
    }
}

void SyntacticAnalyzer::compoundStatement(vector<int> followers) /* анализ конструкции <составной оператор> */
{
    vector<int> ptra;

    if (symbolCode != Keywords::getKeyWordCode("begin")) // составной оператор должен начинаться сос лова бегин
    {
        lexer->addNewError(Keywords::getKeyWordCode("begin"));
        skipto2(vector<int> {Keywords::getKeyWordCode("begin")}, followers);
    }
    if (symbolCode == Keywords::getKeyWordCode("begin"))
    {
        ptra = set_disjunct(vector<int> {Keywords::getKeyWordCode("end")}, followers);// сам составной оператор должен закончиться словом енд, либо 

        accept(Keywords::getKeyWordCode("begin"));
        statement(ptra); // обрабатывает все выражения между begin-end

        while (!lexer->checkEof() && !belong(ptra)) // пока после выражения стоит ";" читаем новое выражение
        {
            statement(ptra);
        }
        ptra = set_disjunct(vector<int> {Keywords::getKeyWordCode("end")}, followers); // после всеж выражений должен быть end
        accept2(Keywords::getKeyWordCode("end"), ptra); // если стоим на слове end то все хорошо, если не стоим то скипаем ве символы пока не найдем словов end или что то что должо стоять после всего составного оператора
        //accept(Keywords::getKeyWordCode("end"));

        if (!belong(followers))
        {
            lexer->addNewError(6); /* запрещенный символ */
            skipto(followers);
        }
    }
}

void SyntacticAnalyzer::varPart(vector<int> followers)  /* анализ конструкции <раздел переменных> */
{
    vector<int> ptra;

    if (!belong(st_varpart))
    {
        lexer->addNewError(18); /* ошибка в разделе описаний */
        skipto2(st_varpart, followers);
    }

    if (symbolCode == Keywords::getKeyWordCode("var"))
    {
        nextSym();
        ptra = set_disjunct(vector<int> {Identifiers::ident, Signs::semicolon}, followers);
        do
        {
            varDeclaration(ptra);
            accept(Signs::semicolon);
        } while (!lexer->checkEof() && symbolCode == Identifiers::ident);
        if (!belong(followers))
        {
            lexer->addNewError(6); /* запрещенный символ */
            skipto(followers);
        }
    }
}

void SyntacticAnalyzer::procPart(vector<int> followers)
{
    vector<int> ptra;

    if (!belong(st_procfuncpart))
    {
        lexer->addNewError(18); /* ошибка в разделе описаний */
        skipto2(st_varpart, followers);
    }

    if (symbolCode == Keywords::getKeyWordCode("procedure"))
    {
        accept(Keywords::getKeyWordCode("procedure"));

        ptra = set_disjunct(vector<int> {Identifiers::ident, Signs::leftpar, Keywords::getKeyWordCode("var")}, followers);
        accept2(Identifiers::ident, ptra);

        ptra = set_disjunct(vector<int> {Signs::leftpar, Keywords::getKeyWordCode("var")}, followers); // после идентификатор должна найтись "(", если она не найдется то нам похуй на эту строку и мы остановимся на слове var
        accept2(Signs::leftpar, ptra); // находим скобку или слово вар

        ptra = set_disjunct(vector<int> {Identifiers::ident, Signs::rightpar, Signs::semicolon, Keywords::getKeyWordCode("var")}, followers); // после "(" ожидается описание переменных, затем ")" или слово вар
        do
        {
            varDeclaration(ptra); // делает отже самое что и в varpart, если что то не так внутри этого то скипаем пока не найдем ")" либо ";" либо слово вар 
            if (symbolCode != Signs::rightpar) accept(Signs::semicolon);
        } while (!lexer->checkEof() && symbolCode == Identifiers::ident);

        ptra = set_disjunct(vector<int> {Signs::rightpar, Signs::semicolon, Keywords::getKeyWordCode("var")}, followers);
        accept2(Signs::rightpar, ptra); // либо акспетим ")", либо, если сейчас не на ней то пропускаем сиволы пока не нйдем ")" либо ";" либо слово вар

        ptra = set_disjunct(vector<int> {Signs::semicolon, Keywords::getKeyWordCode("var")}, followers);
        accept2(Signs::semicolon, ptra); // либо акспетим ";",  либо, если сейчас не на ней то пропускаем сиволы пока не нйдем ";" либо слово вар

        ptra = set_disjunct(vector<int>{Keywords::getKeyWordCode("var")}, followers);
        varPart(ptra); // либо акспетим слово вар,  либо, если сейчас не на нем то пропускаем сиволы пока не нйдем  слово вар либо не найдем что угодно что должно стоять после слова вар

        ptra = set_disjunct(vector<int>{Signs::semicolon}, followers);
        statementPart(ptra); // тут читается все что между begin и end внутри описания процедуры

        accept2(Signs::semicolon, followers); // после оописания процедуры должна стоять ";", если сейчас не на ней то пропускаем сиволы пока не нйдем ";" либо что то из того что может быть после описания процедуры (либо оснвной блок программы либо еще опсания функций или процедур)

        if (!belong(followers))
        {
            lexer->addNewError(6); /* запрещенный символ */
            skipto(followers);
        }
    }
}

void SyntacticAnalyzer::funcPart(vector<int> followers) // опсиание функции отлчиается от описания процедуры только словом function и наличием после ")" символа ":" и названия типа данных
{
    vector<int> ptra;

    if (!belong(st_procfuncpart))
    {
        lexer->addNewError(18); /* ошибка в разделе описаний */
        skipto2(st_varpart, followers);
    }

    if (symbolCode == Keywords::getKeyWordCode("function"))
    {
        accept(Keywords::getKeyWordCode("function"));

        ptra = set_disjunct(vector<int> {Identifiers::ident, Signs::leftpar, Signs::colon, Keywords::getKeyWordCode("var")}, followers);
        accept2(Identifiers::ident, ptra);

        ptra = set_disjunct(vector<int> {Signs::leftpar, Signs::colon, Keywords::getKeyWordCode("var")}, followers);
        accept2(Signs::leftpar, ptra);

        ptra = set_disjunct(vector<int> {Identifiers::ident, Signs::rightpar, Signs::semicolon, Keywords::getKeyWordCode("var")}, followers);
        do
        {
            varDeclaration(ptra);
            if (symbolCode != Signs::rightpar) accept(Signs::semicolon);
        } while (!lexer->checkEof() && symbolCode == Identifiers::ident);

        ptra = set_disjunct(vector<int> {Signs::rightpar, Signs::semicolon, Keywords::getKeyWordCode("var")}, followers);
        accept2(Signs::rightpar, ptra);

        ptra = set_disjunct(vector<int> {Signs::colon, Signs::semicolon, Keywords::getKeyWordCode("var")}, followers);
        accept2(Signs::colon, ptra); // принимаем если есть двоеточие после правой круглой скобки

        ptra = set_disjunct(vector<int> {Signs::semicolon}, followers);
        type(ptra); // принимаем если есть тип данных

        ptra = set_disjunct(vector<int> {Signs::semicolon, Keywords::getKeyWordCode("var")}, followers);
        accept2(Signs::semicolon, ptra);

        ptra = set_disjunct(vector<int>{Keywords::getKeyWordCode("var")}, followers);
        varPart(ptra);

        ptra = set_disjunct(vector<int>{Signs::semicolon}, followers);
        statementPart(ptra);

        accept2(Signs::semicolon, followers);

        if (!belong(followers))
        {
            lexer->addNewError(6); /* запрещенный символ */
            skipto(followers);
        }
    }
}


void SyntacticAnalyzer::statementPart(vector<int> followers) // обрабатывает операторы, в целом их много но по варианту только составной
{
    compoundStatement(followers); // вызов фунцкии которая обрабатывает конкретно составной оператор 
}


void SyntacticAnalyzer::block(vector<int> followers) /* анализ конструкции <блок> */
{
    vector<int> ptra;

    if (!belong(begpart))
    {
        lexer->addNewError(18);
        skipto2(begpart, followers);
    }

    if (belong(begpart))
    {
        ptra = set_disjunct(st_procfuncpart, followers);

        varPart(ptra);
        procPart(ptra);
        funcPart(ptra);

        statementPart(followers);
        if (!belong(followers))
        {
            lexer->addNewError(6); /* запрещенный символ */
            skipto(followers);
        }
    }
}


void SyntacticAnalyzer::programme()
/* анализ конструкции <программа> */
{
    accept(Keywords::getKeyWordCode("program"));
    accept(Identifiers::ident);
    accept(Signs::semicolon);
    block(vector<int>{Signs::point}); /* анализ конструкции <блок> */
    accept(Signs::point);
}

SyntacticAnalyzer::~SyntacticAnalyzer()
{
    lexer->~LexicalAnalyzer();
}

