/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#pragma once
#include "localdefs.h"
#include "url.hpp"

#include <iostream>
using namespace std;

BEGIN_NAMESPACE (hy);

struct eqcase
{
    public:
        eqcase () {}
        ~eqcase () {}
        bool operator () (const string &str1, const string &str2) const;
        bool operator () (const char *str1, const char *str2) const;
#ifdef USE_WIDECHAR
        bool operator () (const wstring &str1, const wstring &str2);
        bool operator () (const wchar_t *str1, const wchar_t *str2) const;
#endif
};

struct strhash
{
    public:
        strhash () {}
        ~strhash () {}

        size_t operator () (const char *) const;
        size_t operator () (const string &) const;
#ifdef USE_WIDECHAR
        size_t operator () (const wchar_t *) const;
        size_t operator () (const wstring &) const;
#endif
};

struct strcasehash
{
    public:
        strcasehash () {}
        ~strcasehash () {}
        size_t operator () (const char *) const;
        size_t operator () (const string &) const;
        size_t operator () (const URL &u) const;
#ifdef USE_WIDECHAR
        size_t operator () (const wchar_t *) const;
        size_t operator () (const wstring &) const;
#endif
};

END_NAMESPACE (hy);
