/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#ifndef _UTILS_HASHSET_H_
#define _UTILS_HASHSET_H_

#ifndef WIN32
# include <tr1/unordered_set>
# define HashSet std::tr1::unordered_set
#else
/* WIN32 guys, include your hash_map here! */
#include <set>
#define Set set
#endif

#endif // _UTILS_HASHSET_H_
