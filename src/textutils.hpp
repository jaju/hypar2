/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

/*
 * General helper functions for working with strings (C and C++), of 'both'
 * kinds - 'char' and 'wchar_t'
 */

#pragma once
#include "localdefs.h"
#include <string>

using std::string;
#if USE_WIDECHAR
using std::wstring;
#endif

BEGIN_NAMESPACE (hy);
BEGIN_NAMESPACE (textutils);

void tolower (string &s);
void tolower (char *s);
#if USE_WIDECHAR
void towlower (wstring &s);
void towlower (wchar_t *s);
string wideToNarrow (const wstring &ws);
string wideToNarrow (const wchar_t *ws);
string toLocalLocale (const wstring &s);
string toLocalLocale (const wchar_t *s);
#endif

_char *echoSpaces (int iNumSpaces);
_string formatContent (const _char *pContent, int iNumIndents);
_string _itos (int ii);

END_NAMESPACE (textutils);
END_NAMESPACE (hy);
