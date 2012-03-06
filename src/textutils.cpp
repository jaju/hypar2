/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "textutils.hpp"

#include <cstdlib>
#include <algorithm>

using namespace std;

BEGIN_NAMESPACE (hy);
BEGIN_NAMESPACE (textutils);

void tolower (string &s)
{
    transform (s.begin (), s.end (), s.begin (), (int (*)(int)) std::tolower);
}

void tolower (char *s)
{
    transform (s, s + strlen (s), s, (int (*)(int)) std::tolower);
}

void towlower (_string &s)
{
    transform (s.begin (), s.end (), s.begin (), (int (*)(int)) ::towlower);
}

void towlower (_char *s)
{
    transform (s, s + _strlen (s), s, (int (*)(int)) ::towlower);
}

_char *echoSpaces (int iNumSpaces)
{
#define kMaxSpaces 1024
    static _char sSpaces[kMaxSpaces + 1] = {L(' ')};
    /* XXX - Can be optimized, to initialize the last char to NULL only once */
    sSpaces[kMaxSpaces] = L('\0');
    _char *returnString = sSpaces + kMaxSpaces;
    if (iNumSpaces <= 0)
        return returnString;
    else if (iNumSpaces > kMaxSpaces)
    {
        iNumSpaces = kMaxSpaces;
    }
    returnString -= iNumSpaces;
    return returnString;
}

_string formatContent (const _char *pContent, int iNumIndents)
{
    _string formattedString = L("");
    int currentCursor = 0;
    _string spacePrefix = echoSpaces (iNumIndents);
    if (iNumIndents <= 0)
    {
        formattedString = pContent;
        return formattedString;
    }
    while (1)
    {
        if (!*pContent)
            break;
        for (currentCursor = 0; *pContent &&
                *(pContent + currentCursor) != L('\n'); currentCursor++);
        if (*(pContent + currentCursor) == L('\n'))
            currentCursor++;
        formattedString += spacePrefix;
        formattedString.append (pContent, currentCursor + 1);
        pContent += currentCursor + 1;
    }
    return formattedString;
}

_string _itos (int ii)
{
    /* sprintf is ok, but this is more fun! */

    _char wbuf[17] = {0};
    char buf[17] = {0};
    int i = ii;

    _char *retstr = wbuf + 15;
    char *p = buf + 15;

    *retstr = L('0');
    *p = '0';

    int residue = 0;
    bool cut = false;
    while (1)
    {
        if (cut)
            break;
        if (i < 10)
        {
            cut = true;
        }
        residue = i % 10;
        *retstr = L('0') + residue;
        *p = '0' + residue;
        i = i/10;
        retstr--; p--;
    }
    return _string (retstr + 1);
}

string wideToNarrow (const wstring &ws)
{
    return wideToNarrow (static_cast<const wchar_t *> (ws.c_str ()));
}

string wideToNarrow (const wchar_t *ws)
{
    string return_string;
    int bufsz = (wcslen (ws) + 1) * sizeof (wchar_t);
    char *pLocalLocaleBuffer = new char[bufsz];
    wcstombs (pLocalLocaleBuffer, ws, bufsz);
    return_string.assign (pLocalLocaleBuffer);
    delete pLocalLocaleBuffer;
    return return_string;
}

string toLocalLocale (const wstring &s)
{
    return toLocalLocale (s.c_str ());
}

string toLocalLocale(const wchar_t *s)
{
    return wideToNarrow (s);
}

END_NAMESPACE (textutils);
END_NAMESPACE (hy);
