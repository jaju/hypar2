/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "hypar/debug.hpp"

#include <iostream>
#include <cstdio>
#include <stdarg.h>

BEGIN_NAMESPACE (hy);

const char *echowide (const char *s)
{
    return s;
}

#if USE_WIDECHAR
char *echowide (const _char *s)
{
#define BUFSZ 16*1024
    static char s_strBuf[BUFSZ];
    int len = _strlen (s) < BUFSZ ? _strlen (s) : BUFSZ;
    bzero (s_strBuf, BUFSZ);
    wcstombs (s_strBuf, s, len);
    return s_strBuf;
}
#endif

void debug_print (const char *format, ...)
{
    va_list ap;
    va_start (ap, format);
    vfprintf (stderr, format, ap);
    va_end (ap);
    fflush (stderr);
}
END_NAMESPACE (hy);
