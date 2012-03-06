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

BEGIN_NAMESPACE (hy);

struct eqcase
{
    private:
        typedef std::string string;
    public:
        eqcase () {}
        ~eqcase () {}
        bool operator () (const string &str1, const string &str2) const;
        bool operator () (const char *str1, const char *str2) const;
        bool operator () (const std::wstring &str1, const std::wstring &str2);
        bool operator () (const wchar_t *str1, const wchar_t *str2) const;
};

struct strhash
{
    private:
        typedef std::string string;
    public:
        strhash () {}
        ~strhash () {}

        size_t operator () (const char *) const;
        size_t operator () (const string &) const;
        size_t operator () (const wchar_t *) const;
        size_t operator () (const std::wstring &) const;
};

struct strcasehash
{
    private:
        typedef std::string string;
    public:
        strcasehash () {}
        ~strcasehash () {}
        size_t operator () (const char *) const;
        size_t operator () (const string &) const;
        size_t operator () (const URL &u) const;
        size_t operator () (const wchar_t *) const;
        size_t operator () (const std::wstring &) const;
};

END_NAMESPACE (hy);
