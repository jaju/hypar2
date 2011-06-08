#include <tokenizer.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <iostream>

using std::string;

TEST(XMLTokenizer, DoesNothing) {
    string token;
    string input = "<hello attrib=\"helloworld<<<>><<>'''\">world</hello attrib='wdjhskjdfjkf<>'><body>How are you?<i>YOU!</i></body>";

    boost::tokenizer<XMLTokenizerFunc> tok(input);

    boost::tokenizer<XMLTokenizerFunc>::iterator x;

    x = tok.begin();
    while (x != tok.end()) {
        std::cout << *x << std::endl;
        x++;
    }
}
