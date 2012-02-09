/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#pragma once
#include "localdefs.h"
#include <string>
#include <list>

BEGIN_NAMESPACE (hy);

typedef std::pair<std::string, std::string> NVPair;
typedef std::list<NVPair> CGIAttrList;

/**
 * The URL parser class
 *
 * Use this to parse URLs and extract various components, including
 * protocol, port, host, path, cgi name-value pairs, views
 */

/**
 * Class for parsing and working with URLs
 */
class URL
{
    private:
        typedef std::string string;
    public:
        /**
         * Indicates successes and failures */
        typedef enum
        {
            URL_BAD_SYNTAX = -100,
            URL_OK = 0
        }
        ReturnCode;

    protected:
        static const char hex[];
        static const unsigned char acceptMask[];
        static const unsigned int XALPHAS = 0x1;

        string m_protocol, m_host, m_path, m_fragment, m_cgiUri;
        short m_sPort;
        ReturnCode rc;
        bool m_bIsCgi, m_bIsCgiUriParsed, m_bNoFollow;
        CGIAttrList m_CGIAttrList;

    protected:
        void simplifyPath ();

    public:
        /**
         * static call for parsing - fills up all values */
        static void parse (
                const string &url,
                string &proto,
                string &host,
                short  &port,
                string &path,
                string &fragment,
                string &cgiUri,
                bool   &iscgi
                );
        /**
         * Tells whether the character is an acceptable character in URLs */
        static bool isAcceptable (unsigned char cx);
        /**
         * Clear up all attributes */
        void clear ();

    public:
        /**
         * Constructor from absolute URLs */
        URL (const string &absolute);
        /**
         * Constructor from relative URLs, given the base URL */
        URL (const string &relative, const string & base);
        /**
         * Get the status after parsing */
        ReturnCode getParsestatus_t () const;
        /**
         * Get the canonical form of the URL */
        string getCanonical (bool dropFragment = false) const;
        /**
         * Get the protocol in string format */
        string getProtocol () const;
        /**
         * Get the target host in string format */
        string getHost () const;
        /**
         * Get the port of the target host as a short int */
        short getPort () const;
        /**
         * Get the URI path on the destination host */
        string getPath () const;
        /**
         * Get the fragment indicator, like the part after #
         * in http://..../a/b.html#part2 */
        string getFragment () const;
        /**
         * Get the CGI component, after the '?' character */
        string getCgiUri () const;
        /**
         * Get the resource indicator on the target host */
        string getURI ();
        /**
         * Parse the CGI component, and store the name-value pairs */
        void parseCgiUri ();
        /**
         * Given a CGI LHS variable, extract the value on the RHS */
        bool getCgiValue (const string &name, string &value);
        /**
         * Is the URL a dynamic, CGI URL? */
        bool isCGI () const;

        /* Is the no-follow attribute set? */
        bool isNoFollowSet () { return m_bNoFollow; }
        void setNoFollow (bool b = true) { m_bNoFollow = b; }

        /**
         * Is the supplied URL absolute? */
        static bool isAbsolute (const string & aurl);
        /**
         * Escape invalid characters, and convert them */
        static string escape (const string & unes);
        /**
         * Unescape chars in the URL */
        static string unEscape (const string & esc);

    public:
        bool operator == (const URL &u) const;
        bool equals (const URL &u) const;

};

END_NAMESPACE (hy);
