/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "debug.hpp"

#include <iostream>
#include <cstdio>
#include <stdarg.h>

BEGIN_NAMESPACE (hy);

void debug_print (const char *format, ...)
{
    va_list ap;
    va_start (ap, format);
    vfprintf (stderr, format, ap);
    va_end (ap);
    fflush (stderr);
}
END_NAMESPACE (hy);
