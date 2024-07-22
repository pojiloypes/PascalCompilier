#include "Error.h"


Error::Error(TextPosition errorPos, int errorCode)
{
    this->errorCode = errorCode;
    this->position = errorPos;
}

string Error::decodeError()
{
    string res;

    switch (this->errorCode)
    {
    case 12:
        res =  "����������� ������\n";
        break;
    case 95:
        res =  "���������� ������������� �����������\n";
        break;
    case 80:
        res =  "����������� ������ \"'\"\n";
        break;
    case 301:
        res =  "������������ ������������ ���������\n";
        break;
    case 302:
        res =  "������������ ������������� ���������\n";
        break;
    case 156:
        res =  "������������ �������� ���������\n";
        break;
    case 2:
        res = "����������� �������������\n";
        break;
    case 21: 
        res = "����������� \"*\"\n"; 
        break;
    case 60: res = "����������� \"/\"\n"; break;
    case 16: res = "����������� \"=\"\n"; break;
    case 20: res = "����������� \",\"\n"; break;
    case 14: res = "����������� \";\"\n"; break;
    case 5: res = "����������� \":\"\n"; break;
    case 61: res = "����������� \".\"\n"; break;
    case 62: res = "����������� \"^\"\n"; break;
    case 9: res = "����������� \"(\"\n"; break;
    case 4: res = "����������� \")\"\n"; break;
    case 11: res = "����������� \"[\"\n"; break;
    case 13: res = "����������� \"]\"\n"; break;
    case 63: res = "����������� \"{\"\n"; break;
    case 64: res = "����������� \"}\"\n"; break;
    case 65: res = "����������� \"<\"\n"; break;
    case 66: res = "����������� \">\"\n"; break;
    case 67: res = "����������� \"<=\"\n"; break;
    case 68: res = "����������� \">=\"\n"; break;
    case 69: res = "����������� \"<>\"\n"; break;
    case 70: res = "����������� \"+\"\n"; break;
    case 71: res = "����������� \"�\"\n"; break;
    case 72: res = "����������� \"(*\"\n"; break;
    case 73: res = "����������� \"*)\"\n"; break;
    case 51: res = "����������� \":=\"\n"; break;
    case 74: res = "����������� \"..\"\n"; break;
    case 100: res = "����������� \"in\"\n"; break;
    case 101: res = "����������� \"of\"\n"; break;
    case 102: res = "����������� \"or\"\n"; break;
    case 103: res = "����������� \"to\"\n"; break;
    case 104: res = "����������� \"end\"\n"; break;
    case 105: res = "����������� \"var\"\n"; break;
    case 106: res = "����������� \"div\"\n"; break;
    case 107: res = "����������� \"and\"\n"; break;
    case 108: res = "����������� \"not\"\n"; break;
    case 109: res = "����������� \"for\"\n"; break;
    case 110: res = "����������� \"mod\"\n"; break;
    case 111: res = "����������� \"nil\"\n"; break;
    case 112: res = "����������� \"set\"\n"; break;
    case 113: res = "����������� \"begin\"\n"; break;
    case 114: res = "����������� \"while\"\n"; break;
    case 115: res = "����������� \"array\"\n"; break;
    case 116: res = "����������� \"const\"\n"; break;
    case 117: res = "����������� \"label\"\n"; break;
    case 118: res = "����������� \"downto\"\n"; break;
    case 119: res = "����������� \"packed\"\n"; break;
    case 120: res = "����������� \"record\"\n"; break;
    case 121: res = "����������� \"repeat\"\n"; break;
    case 122: res = "����������� \"program\"\n"; break;
    case 123: res = "����������� \"function\"\n"; break;
    case 124: res = "����������� \"procedure\"\n"; break;
    case 125: res = "����������� \"case\"\n"; break;
    case 126: res = "����������� \"else\"\n"; break;
    case 127: res = "����������� \"file\"\n"; break;
    case 128: res = "����������� \"goto\"\n"; break;
    case 129: res = "����������� \"then\"\n"; break;
    case 130: res = "����������� \"type\"\n"; break;
    case 131: res = "����������� \"until\"\n"; break;
    case 132: res = "����������� \"do\"\n"; break;
    case 133: res = "����������� \"with\"\n"; break;
    case 134: res = "����������� \"if\"\n"; break;
    case 200: res = "����������� \"integersy\"\n"; break;
    case 201: res = "����������� \"floatsy\"\n"; break;
    case 202: res = "����������� \"stringsy\"\n"; break;
    case 203: res = "����������� \"charsy\"\n"; break;
    case 204: res = "����������� \"realsy\"\n"; break;
    case 205: res = "����������� \"booleansy\"\n"; break;
    case 206: res = "��������� ��������\"\n"; break;
    case 993:
        res =  "�������� ������ ������\n";
        break;
    case 1000:
        res = "������ ��� ��� ������������\n";
        break;
    case 1001:
        res = "������������ �������������� � ����� ���������\n";
        break;
    case 1002:
        res = "������������ �����\n";
        break;
    case 1003:
        res = "������������ �������� ��� ������� ���� ���������\n";
        break;
    case 1004:
        res = "�������� ����� ����������\n";
        break;
        //...
    default:
        res =  "� ������ ������ ������ ��������� � ����������\n";
        break;
    }
    return res;
}
