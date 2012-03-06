/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "hash.hpp"
#include <cctype> // for tolower etc.

BEGIN_NAMESPACE (hy);
using std::wstring;

size_t str_hash_func (const wchar_t *s)
{
	unsigned long __h = 0;
	for ( ; *s; ++s)
		__h = 5*__h + *s;
	return size_t (__h);
}

size_t strcase_hash_func (const wchar_t *s)
{
	unsigned long __h = 0;
	for ( ; *s; ++s)
		__h = 5*__h + towlower (*s);
	return size_t (__h);
}

size_t strcase_hash_func (const char *s)
{
	unsigned long __h = 0;
	for ( ; *s; ++s)
		__h = 5*__h + tolower (*s);
	return size_t (__h);
}

size_t strcase_hash_func (const URL &u)
{
    unsigned long __h1 = 0, __h2 = 0;
    __h1 += strcase_hash_func (u.getHost ().c_str ());
    __h1 += u.getPort ();
    std::string s = u.getPath ();
    __h2 = strcase_hash_func (s.c_str ());
    if (s.at (s.length () - 1) != '/')
    {
        __h2 = 5*__h2 + '/';
    }
#if DEBUG
    if (u.isCGI ())
    {
        cerr << "Warning: CGI URI in strcase_hash_func - not handled!" << endl;
    }
#endif
    return size_t (__h1 + __h2);
}

size_t str_hash_func (const char *s)
{
    unsigned long __h = 0;
    for ( ; *s; ++s)
        __h = 5*__h + *s;
    return size_t (__h);
}


size_t strhash::operator () (const char *s) const
{
    return str_hash_func (s);
}

size_t strhash::operator() (const string &s) const
{
    return str_hash_func (s.c_str ());
}

size_t strcasehash::operator() (const char *s) const
{
    return strcase_hash_func (s);
}

size_t strcasehash::operator() (const string &s) const
{
    return strcase_hash_func (s.c_str ());
}

size_t strcasehash::operator() (const wchar_t *s) const
{
    return strcase_hash_func (s);
}
size_t strcasehash::operator() (const wstring &s) const
{
    return strcase_hash_func (s.c_str ());
}

bool eqcase::operator () (const string &str1, const string &str2) const
{
    return (strcasecmp (str1.c_str (), str2.c_str ()) == 0);
}

bool eqcase::operator() (const char *str1, const char *str2) const
{
    return (strcasecmp (str1, str2) == 0);
}

bool eqcase::operator() (const wstring &str1, const wstring &str2)
{
    return (wcscasecmp(str1.c_str (), str2.c_str ()) == 0);
}

bool eqcase::operator () (const wchar_t *str1, const wchar_t *str2) const
{
    return (wcscasecmp (str1, str2) == 0);
}

size_t strcasehash::operator () (const URL &u) const
{
    return strcase_hash_func (u);
}

END_NAMESPACE (hy);
