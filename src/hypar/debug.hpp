/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#pragma once
#include "hypar/localdefs.h"

BEGIN_NAMESPACE (hy);

/**
 * Convert to wchar_t * and return a pointer to the converted (static)
 * buffer location (hence: non thread-safe!!)
 */
const char *echowide (const char *s);
#ifdef USE_WIDECHAR
char *echowide (const _char *s);
#endif
long echowide (long l);
void debug_print (const char *format, ...);

#define hy_warn(a) debug_print ("WARN:%s(%d): ", __FILE__, __LINE__); debug_print a
END_NAMESPACE (hy);

#ifdef _HYPAR_DEBUG_H_
# undef debug
#endif

#ifndef DEBUG
#define DEBUG 1
#endif

#if DEBUG
#define debug(a) hy::debug_print ("DEBUG:%s(%d): ", __FILE__, __LINE__); hy::debug_print a
#else /* !DEBUG */
#define debug(a)
#endif /* DEBUG */
