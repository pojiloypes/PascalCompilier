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
        res =  "Запрещенный символ\n";
        break;
    case 95:
        res =  "Незакрытый многострочный комментарий\n";
        break;
    case 80:
        res =  "Отсутствует символ \"'\"\n";
        break;
    case 301:
        res =  "Переполнение вещественной константы\n";
        break;
    case 302:
        res =  "Переполнение целочисленной константы\n";
        break;
    case 156:
        res =  "Переполнение сиволной константы\n";
        break;
    case 2:
        res = "Отсутствует идентификатор\n";
        break;
    case 21: 
        res = "Отсутствует \"*\"\n"; 
        break;
    case 60: res = "Отсутствует \"/\"\n"; break;
    case 16: res = "Отсутствует \"=\"\n"; break;
    case 20: res = "Отсутствует \",\"\n"; break;
    case 14: res = "Отсутствует \";\"\n"; break;
    case 5: res = "Отсутствует \":\"\n"; break;
    case 61: res = "Отсутствует \".\"\n"; break;
    case 62: res = "Отсутствует \"^\"\n"; break;
    case 9: res = "Отсутствует \"(\"\n"; break;
    case 4: res = "Отсутствует \")\"\n"; break;
    case 11: res = "Отсутствует \"[\"\n"; break;
    case 13: res = "Отсутствует \"]\"\n"; break;
    case 63: res = "Отсутствует \"{\"\n"; break;
    case 64: res = "Отсутствует \"}\"\n"; break;
    case 65: res = "Отсутствует \"<\"\n"; break;
    case 66: res = "Отсутствует \">\"\n"; break;
    case 67: res = "Отсутствует \"<=\"\n"; break;
    case 68: res = "Отсутствует \">=\"\n"; break;
    case 69: res = "Отсутствует \"<>\"\n"; break;
    case 70: res = "Отсутствует \"+\"\n"; break;
    case 71: res = "Отсутствует \"–\"\n"; break;
    case 72: res = "Отсутствует \"(*\"\n"; break;
    case 73: res = "Отсутствует \"*)\"\n"; break;
    case 51: res = "Отсутствует \":=\"\n"; break;
    case 74: res = "Отсутствует \"..\"\n"; break;
    case 100: res = "Отсутствует \"in\"\n"; break;
    case 101: res = "Отсутствует \"of\"\n"; break;
    case 102: res = "Отсутствует \"or\"\n"; break;
    case 103: res = "Отсутствует \"to\"\n"; break;
    case 104: res = "Отсутствует \"end\"\n"; break;
    case 105: res = "Отсутствует \"var\"\n"; break;
    case 106: res = "Отсутствует \"div\"\n"; break;
    case 107: res = "Отсутствует \"and\"\n"; break;
    case 108: res = "Отсутствует \"not\"\n"; break;
    case 109: res = "Отсутствует \"for\"\n"; break;
    case 110: res = "Отсутствует \"mod\"\n"; break;
    case 111: res = "Отсутствует \"nil\"\n"; break;
    case 112: res = "Отсутствует \"set\"\n"; break;
    case 113: res = "Отсутствует \"begin\"\n"; break;
    case 114: res = "Отсутствует \"while\"\n"; break;
    case 115: res = "Отсутствует \"array\"\n"; break;
    case 116: res = "Отсутствует \"const\"\n"; break;
    case 117: res = "Отсутствует \"label\"\n"; break;
    case 118: res = "Отсутствует \"downto\"\n"; break;
    case 119: res = "Отсутствует \"packed\"\n"; break;
    case 120: res = "Отсутствует \"record\"\n"; break;
    case 121: res = "Отсутствует \"repeat\"\n"; break;
    case 122: res = "Отсутствует \"program\"\n"; break;
    case 123: res = "Отсутствует \"function\"\n"; break;
    case 124: res = "Отсутствует \"procedure\"\n"; break;
    case 125: res = "Отсутствует \"case\"\n"; break;
    case 126: res = "Отсутствует \"else\"\n"; break;
    case 127: res = "Отсутствует \"file\"\n"; break;
    case 128: res = "Отсутствует \"goto\"\n"; break;
    case 129: res = "Отсутствует \"then\"\n"; break;
    case 130: res = "Отсутствует \"type\"\n"; break;
    case 131: res = "Отсутствует \"until\"\n"; break;
    case 132: res = "Отсутствует \"do\"\n"; break;
    case 133: res = "Отсутствует \"with\"\n"; break;
    case 134: res = "Отсутствует \"if\"\n"; break;
    case 200: res = "Отсутствует \"integersy\"\n"; break;
    case 201: res = "Отсутствует \"floatsy\"\n"; break;
    case 202: res = "Отсутствует \"stringsy\"\n"; break;
    case 203: res = "Отсутствует \"charsy\"\n"; break;
    case 204: res = "Отсутствует \"realsy\"\n"; break;
    case 205: res = "Отсутствует \"booleansy\"\n"; break;
    case 206: res = "Ожидалось значение\"\n"; break;
    case 993:
        res =  "Ожидался другой символ\n";
        break;
    case 1000:
        res = "Данное имя уже используется\n";
        break;
    case 1001:
        res = "Несуществует идентификатора с таким названием\n";
        break;
    case 1002:
        res = "Несовпадение типов\n";
        break;
    case 1003:
        res = "Недопустимый параметр для данного типа выражения\n";
        break;
    case 1004:
        res = "Неверное число параметров\n";
        break;
        //...
    default:
        res =  "В данный момент ошибка находится в разработке\n";
        break;
    }
    return res;
}
