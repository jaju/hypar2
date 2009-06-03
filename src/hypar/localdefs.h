/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#ifndef _LOCALDEFS_H_
#define _LOCALDEFS_H_

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
using std::string;

#if USE_WIDECHAR
using std::wstring;
#endif

#if USE_WIDECHAR
# include <cwchar>
# include <cwctype>
#define _STDLIB_H
#include <cstdlib>
# define L(x) L##x
# define _strcmp ::wcscmp
# define _strncmp ::wcsncmp
# define _strcasecmp ::wcscasecmp
# define _strncasecmp ::wcsncasecmp
# define _strrchr ::wcsrchr
# define _strstr ::wcswcs
# define _strdup ::wcsdup
# define _strlen ::wcslen
# define _tolower ::towlower
# define _isspace ::iswspace
# define _char wchar_t
# define _cout wcout
# define _cerr wcerr
# define _strncpy ::wcsncpy
# define _strtol ::wcstol
typedef std::wstring _string;
#else /* !USE_WIDECHAR */
# define L(x) x
# define _strcmp strcmp
# define _strncmp strncmp
# define _strcasecmp strcasecmp
# define _strncasecmp strncasecmp
# define _strrchr strrchr
# define _strstr strstr
# define _strdup strdup
# define _strlen strlen
# define _tolower tolower
# define _isspace isspace
# define _char char
# define _cout cout
# define _cerr cerr
# define _strncpy strncpy
# define _strtol strtol
typedef std::string _string;
#endif /* USE_WIDECHAR */

#endif // _LOCALDEFS_H_
