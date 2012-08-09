#include <tokenizer.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <iostream>

using std::string;

TEST(XMLTokenizer, IsVeryForgiving) {
    string token;
    string input = "<hello attrib=\"helloworld<<<>><<>'''\"> world</hello attrib='wdjhskjdfjkf<>'><body>How are you? <i> YOU! </i></body>";
    typedef boost::tokenizer<XMLTokenizerFunc> T;

    T tok(input);
    std::vector<string> tokens;
    for (T::iterator x = tok.begin(); x != tok.end(); tokens.push_back(*x), x++);

    EXPECT_EQ(tokens[0], "<hello attrib=\"helloworld<<<>><<>'''\">");
    EXPECT_EQ(tokens[1], " world");
    EXPECT_EQ(tokens[2], "</hello attrib='wdjhskjdfjkf<>'>");
    EXPECT_EQ(tokens[3], "<body>");
    EXPECT_EQ(tokens[4], "How are you? ");
    EXPECT_EQ(tokens[5], "<i>");
    EXPECT_EQ(tokens[6], " YOU! ");
    EXPECT_EQ(tokens[7], "</i>");
    EXPECT_EQ(tokens[8], "</body>");
    
}
