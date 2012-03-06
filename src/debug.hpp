/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#pragma once
#include "localdefs.h"

BEGIN_NAMESPACE (hy);

void debug_print (const char *format, ...);

#define hy_warn(a) debug_print ("WARN:%s(%d): ", __FILE__, __LINE__); debug_print a
END_NAMESPACE (hy);

#ifdef _HYPAR_DEBUG_H_
# undef debug
#endif

#ifndef DEBUG
#define DEBUG 0
#endif

#if DEBUG
#define debug(a) hy::debug_print ("DEBUG:%s(%d): ", __FILE__, __LINE__); hy::debug_print a
#else /* !DEBUG */
#define debug(a)
#endif /* DEBUG */
