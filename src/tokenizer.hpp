#pragma once

#include "localdefs.hpp"
#include <boost/tokenizer.hpp>
#include <string>
#include <unordered_set>

#include <iostream>
using namespace std;

class XMLTokenizerFunc {
    private:
        typedef std::unordered_set<char> charset;
        typedef std::string::const_iterator iter;
        charset quotes;
    public:
        XMLTokenizerFunc() {initQuotesSet();}
        ~XMLTokenizerFunc() {}

    private:
        void initQuotesSet() {
            quotes.insert('\'');
            quotes.insert('"');
        }

    public:
        bool operator()(iter &next,
                const iter end,
                std::string &token) {
            iter start = next;
            bool retval = true;
            bool bInXML = (*next == '<') ? true : false;
            if (next == end)
                return false;
            next++;
            if (!bInXML) {
                while (next != end && *next != '<')
                    next++;
            }
            else {
                char quoteChar = '\0';
                while (next != end && *next != '>') {
                    if (quotes.find(*next) != quotes.end()) {
                        quoteChar = *next;
                        next++;
                        escapeEscape(next, end, quoteChar);
                    }
                    next++;
                }
                if (next == end)
                    retval = false;
                else
                    next++;
            }
            token.assign(start, next);
            return retval;
        }

        void reset() {}
    private:
        bool escapeEscape(iter &cur, const iter end, char quoteChar) {
            while (cur != end && *cur != quoteChar)
                cur++;
            if (cur == end)
                return false;
            return true;
        }
};
