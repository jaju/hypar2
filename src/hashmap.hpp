/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#pragma once
#ifndef WIN32 // This is a GNU using system, with gcc version >= 3
# include <unordered_map>
# define HashMap std::unordered_map
#else
/* WIN32 guys, include your hash_map here! */
#error "Win32: hashmap not implemented!"
#endif
