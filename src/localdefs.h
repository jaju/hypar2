/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#pragma once
#ifdef HAVE_CONFIG_H
# include "hypar/config.h"
#endif

#ifdef __cplusplus
#  define BEGIN_C_DECLS extern "C" {
#  define END_C_DECLS   }
#  define BEGIN_NAMESPACE(x) namespace x {
#  define END_NAMESPACE(x) }
#  define USING_NAMESPACE(x) using namespace x
#  define safe_delete(x) if (x) { delete x; x = 0; }
#else /* !__cplusplus */
#  define BEGIN_C_DECLS
#  define END_C_DECLS
#  define BEGIN_NAMESPACE(x)
#  define END_NAMESPACE(x)
#  define USING_NAMESPACE(x)
#endif /* __cplusplus */

#ifdef __GNUC__
# define likely(x) __builtin_expect(!!(x), 1)
# define unlikely(x)  __builtin_expect(!!(x), 0)
#else
# define likely(x) (x)
# define unlikely(x) (x)
#endif

#include <cstring>
#include <string>

#include <cwchar>
#include <cwctype>
#include <cstdlib>

# define _strcmp strcmp
# define _strncmp strncmp
# define _strcasecmp strcasecmp
# define _strncasecmp strncasecmp
# define _strrchr strrchr
# define _strstr strstr
# define _strdup strdup
# define _strlen strlen
//# define _tolower tolower
# define _isspace isspace
# define _char char
# define _cout cout
# define _cerr cerr
# define _strncpy strncpy
# define _strtol strtol
typedef std::string _string;
