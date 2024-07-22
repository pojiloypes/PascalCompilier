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
        lexer->addNewError(993); /* ������ ���� ��� */
        skipto2(id_starters, followers);
    }

    if (belong(id_starters))
    {
        nextSym();
        if (!belong(followers))
        {
            lexer->addNewError(6); /* ����������� ������ */
            skipto(followers);
        }
    }
}


void SyntacticAnalyzer::varDeclaration(vector<int> followers) /* ������ ����������� <�������� ���������� ����������> */
{
    id_starters.clear();
    id_starters.push_back(Identifiers::ident);

    if (!belong(id_starters))
    {
        lexer->addNewError(2); /* ������ ���� ��� */
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
            lexer->addNewError(6); /* ����������� ������ */
            skipto(followers);
        }
    }
}


bool SyntacticAnalyzer::isItConst() // ���������� ��� ���� ������� ������ ��� ���� ������ ���������� ���������, ���� ��������� ���������, ���� ������������� ��������� � �.�. ,���� ��� �� ���������� ����
{
    return symbolCode == Identifiers::booleanc || symbolCode == Identifiers::charc ||
        symbolCode == Identifiers::floatc || symbolCode == Identifiers::intc ||
        symbolCode == Identifiers::stringc;
}


void SyntacticAnalyzer::expression(vector<int> followers) // ������������ ��������� ���� 23 + 'dawd' - 1 * d / 56 - True
/*�������� �� ������� ����� ��� ��� ��������� � ����� ��������� ��������� � ���������� ���������� � ������ � ������� � ��������, �� � ������� ��� ����� ������ �������� ����� ��� ��������� ������*/
{
    if (symbolCode != Identifiers::ident && !isItConst() && symbolCode != Signs::leftpar) // ��������� ����� ���������� � ���������, � ���������� ���� � "("
    {
        lexer->addNewError(18);
        vector<int> identifiers = vector<int>{ Identifiers::ident,  Identifiers::booleanc, Identifiers::charc,
                                                Identifiers::floatc, Identifiers::intc, Identifiers::stringc };
        skipto2(identifiers, followers); // ���������� ���� �� ������ ���� �� ��������� ���� �� ���������� ���� �� "(" ���� �� ���� ��� ������ ������ ����� ���������
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
            lexer->addNewError(6); /* ����������� ������ */
            skipto(followers);
        }
    }
}
void SyntacticAnalyzer::assignmentStatement(vector<int> followers)
{
    vector<int> ptra = vector<int>{ Identifiers::ident, Signs::semicolon, Signs::assign };

    accept2(Identifiers::ident, ptra); // �������� ������������� ���� ������� ��� ������� ���� �� ������ ���-�� ��� ������ ������ ����� ����
    if (symbolCode != Signs::assign) // assign = ":=" - �������� ������������
    {
        lexer->addNewError(Signs::assign);
        skipto2(vector<int> {Signs::assign}, followers);
    }

    if (symbolCode == Signs::assign)
    {

        accept(Signs::assign);
        expression(followers); // ������� ��������� ���������
        accept(Signs::semicolon);
        if (!belong(followers))
        {
            lexer->addNewError(6); /* ����������� ������ */
            skipto(followers);
        }
    }
}

void SyntacticAnalyzer::statement(vector<int> followers) // ������������ ��� ��������� ���� ��������� ���� ��������� � ��������� ���� ������
{
    vector<int> ptra;
    switch (symbolCode)
    {
    case Identifiers::ident: // �������� ������������ ���������� � ���������� �� ������ ���� ����� �� ���������� �� �������� ��������� �������� �������������
        ptra = set_disjunct(vector<int> {Identifiers::ident, Signs::semicolon}, followers); // ����� ������ ��������� ������ ������ ";", ���� �� ��� �� ����� ����������� ���� ����� ������������� ���� �� ��� ������ ������ ����� ������ � ��������� ������������ � �����
        assignmentStatement(ptra); // ����� ������� ��������� �������� �������������

        break;
    case 113: // ��� ������� begin, � ������ ������ ����� � ����� �� ���������, ������ ��� ��� ����������
        compoundStatement(followers);
        break;
    default:

        break;
    }
    if (!belong(set_disjunct(vector<int> {Identifiers::ident}, followers)))
    {
        lexer->addNewError(6); /* ����������� ������ */
        skipto(followers);
    }
}

void SyntacticAnalyzer::compoundStatement(vector<int> followers) /* ������ ����������� <��������� ��������> */
{
    vector<int> ptra;

    if (symbolCode != Keywords::getKeyWordCode("begin")) // ��������� �������� ������ ���������� ��� ���� �����
    {
        lexer->addNewError(Keywords::getKeyWordCode("begin"));
        skipto2(vector<int> {Keywords::getKeyWordCode("begin")}, followers);
    }
    if (symbolCode == Keywords::getKeyWordCode("begin"))
    {
        ptra = set_disjunct(vector<int> {Keywords::getKeyWordCode("end")}, followers);// ��� ��������� �������� ������ ����������� ������ ���, ���� 

        accept(Keywords::getKeyWordCode("begin"));
        statement(ptra); // ������������ ��� ��������� ����� begin-end

        while (!lexer->checkEof() && !belong(ptra)) // ���� ����� ��������� ����� ";" ������ ����� ���������
        {
            statement(ptra);
        }
        ptra = set_disjunct(vector<int> {Keywords::getKeyWordCode("end")}, followers); // ����� ���� ��������� ������ ���� end
        accept2(Keywords::getKeyWordCode("end"), ptra); // ���� ����� �� ����� end �� ��� ������, ���� �� ����� �� ������� �� ������� ���� �� ������ ������ end ��� ��� �� ��� ����� ������ ����� ����� ���������� ���������
        //accept(Keywords::getKeyWordCode("end"));

        if (!belong(followers))
        {
            lexer->addNewError(6); /* ����������� ������ */
            skipto(followers);
        }
    }
}

void SyntacticAnalyzer::varPart(vector<int> followers)  /* ������ ����������� <������ ����������> */
{
    vector<int> ptra;

    if (!belong(st_varpart))
    {
        lexer->addNewError(18); /* ������ � ������� �������� */
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
            lexer->addNewError(6); /* ����������� ������ */
            skipto(followers);
        }
    }
}

void SyntacticAnalyzer::procPart(vector<int> followers)
{
    vector<int> ptra;

    if (!belong(st_procfuncpart))
    {
        lexer->addNewError(18); /* ������ � ������� �������� */
        skipto2(st_varpart, followers);
    }

    if (symbolCode == Keywords::getKeyWordCode("procedure"))
    {
        accept(Keywords::getKeyWordCode("procedure"));

        ptra = set_disjunct(vector<int> {Identifiers::ident, Signs::leftpar, Keywords::getKeyWordCode("var")}, followers);
        accept2(Identifiers::ident, ptra);

        ptra = set_disjunct(vector<int> {Signs::leftpar, Keywords::getKeyWordCode("var")}, followers); // ����� ������������� ������ ������� "(", ���� ��� �� �������� �� ��� ����� �� ��� ������ � �� ����������� �� ����� var
        accept2(Signs::leftpar, ptra); // ������� ������ ��� ����� ���

        ptra = set_disjunct(vector<int> {Identifiers::ident, Signs::rightpar, Signs::semicolon, Keywords::getKeyWordCode("var")}, followers); // ����� "(" ��������� �������� ����������, ����� ")" ��� ����� ���
        do
        {
            varDeclaration(ptra); // ������ ���� ����� ��� � � varpart, ���� ��� �� �� ��� ������ ����� �� ������� ���� �� ������ ")" ���� ";" ���� ����� ��� 
            if (symbolCode != Signs::rightpar) accept(Signs::semicolon);
        } while (!lexer->checkEof() && symbolCode == Identifiers::ident);

        ptra = set_disjunct(vector<int> {Signs::rightpar, Signs::semicolon, Keywords::getKeyWordCode("var")}, followers);
        accept2(Signs::rightpar, ptra); // ���� �������� ")", ����, ���� ������ �� �� ��� �� ���������� ������ ���� �� ����� ")" ���� ";" ���� ����� ���

        ptra = set_disjunct(vector<int> {Signs::semicolon, Keywords::getKeyWordCode("var")}, followers);
        accept2(Signs::semicolon, ptra); // ���� �������� ";",  ����, ���� ������ �� �� ��� �� ���������� ������ ���� �� ����� ";" ���� ����� ���

        ptra = set_disjunct(vector<int>{Keywords::getKeyWordCode("var")}, followers);
        varPart(ptra); // ���� �������� ����� ���,  ����, ���� ������ �� �� ��� �� ���������� ������ ���� �� �����  ����� ��� ���� �� ������ ��� ������ ��� ������ ������ ����� ����� ���

        ptra = set_disjunct(vector<int>{Signs::semicolon}, followers);
        statementPart(ptra); // ��� �������� ��� ��� ����� begin � end ������ �������� ���������

        accept2(Signs::semicolon, followers); // ����� ��������� ��������� ������ ������ ";", ���� ������ �� �� ��� �� ���������� ������ ���� �� ����� ";" ���� ��� �� �� ���� ��� ����� ���� ����� �������� ��������� (���� ������� ���� ��������� ���� ��� ������� ������� ��� ��������)

        if (!belong(followers))
        {
            lexer->addNewError(6); /* ����������� ������ */
            skipto(followers);
        }
    }
}

void SyntacticAnalyzer::funcPart(vector<int> followers) // �������� ������� ���������� �� �������� ��������� ������ ������ function � �������� ����� ")" ������� ":" � �������� ���� ������
{
    vector<int> ptra;

    if (!belong(st_procfuncpart))
    {
        lexer->addNewError(18); /* ������ � ������� �������� */
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
        accept2(Signs::colon, ptra); // ��������� ���� ���� ��������� ����� ������ ������� ������

        ptra = set_disjunct(vector<int> {Signs::semicolon}, followers);
        type(ptra); // ��������� ���� ���� ��� ������

        ptra = set_disjunct(vector<int> {Signs::semicolon, Keywords::getKeyWordCode("var")}, followers);
        accept2(Signs::semicolon, ptra);

        ptra = set_disjunct(vector<int>{Keywords::getKeyWordCode("var")}, followers);
        varPart(ptra);

        ptra = set_disjunct(vector<int>{Signs::semicolon}, followers);
        statementPart(ptra);

        accept2(Signs::semicolon, followers);

        if (!belong(followers))
        {
            lexer->addNewError(6); /* ����������� ������ */
            skipto(followers);
        }
    }
}


void SyntacticAnalyzer::statementPart(vector<int> followers) // ������������ ���������, � ����� �� ����� �� �� �������� ������ ���������
{
    compoundStatement(followers); // ����� ������� ������� ������������ ��������� ��������� �������� 
}


void SyntacticAnalyzer::block(vector<int> followers) /* ������ ����������� <����> */
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
            lexer->addNewError(6); /* ����������� ������ */
            skipto(followers);
        }
    }
}


void SyntacticAnalyzer::programme()
/* ������ ����������� <���������> */
{
    accept(Keywords::getKeyWordCode("program"));
    accept(Identifiers::ident);
    accept(Signs::semicolon);
    block(vector<int>{Signs::point}); /* ������ ����������� <����> */
    accept(Signs::point);
}

SyntacticAnalyzer::~SyntacticAnalyzer()
{
    lexer->~LexicalAnalyzer();
}

