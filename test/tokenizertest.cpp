#include <tokenizer.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <iostream>

using std::string;

TEST(XMLTokenizer, IsVeryForgiving) {
    string token;
    string input = "<hello attrib=\"helloworld<<<>><<>'''\">world</hello attrib='wdjhskjdfjkf<>'><body>How are you?<i>YOU!</i></body>";
    typedef boost::tokenizer<XMLTokenizerFunc> T;

    T tok(input);
    std::vector<string> tokens;
    for (T::iterator x = tok.begin(); x != tok.end(); tokens.push_back(*x), x++);

    ASSERT_EQ(tokens[0], "<hello attrib=\"helloworld<<<>><<>'''\">");
    ASSERT_EQ(tokens[1], "world");
    ASSERT_EQ(tokens[2], "</hello attrib='wdjhskjdfjkf<>'>");
    ASSERT_EQ(tokens[3], "<body>");
    ASSERT_EQ(tokens[4], "How are you?");
    ASSERT_EQ(tokens[5], "<i>");
    ASSERT_EQ(tokens[6], "YOU!");
    ASSERT_EQ(tokens[7], "</i>");
    ASSERT_EQ(tokens[8], "</body>");
    
}
