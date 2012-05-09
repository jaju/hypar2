#include "xml.hpp"
#include "domnode.hpp"
#include <string>
#include <gtest/gtest.h>
#include <cstring>

using namespace std;
using namespace hy;

TEST(DOMParserTest, ParsesASimpleString) {
    string input = "<html><head><title>Hello, World!</title></head><body>This is the body. </body></html>";
    char *copy = strdup(input.c_str());
    DOMNode *node = DOMNode::create();

    XML h;
    DOMNode *pNode = h.parse(copy, node);
    ASSERT_TRUE(pNode != 0);

    DOMNode *htmlNode = pNode->child();
    ASSERT_STREQ(htmlNode->name(), "html");

    ASSERT_STREQ(htmlNode->child()->name(), "head");
    ASSERT_STREQ(htmlNode->last()->name(), "body");

    DOMNode *headNode = htmlNode->child();
    ASSERT_EQ(headNode->child(), headNode->last());
    ASSERT_EQ(headNode->child()->type(), DOMNode::ELEMENT);
    ASSERT_STREQ(headNode->child()->name(), "title");
    ASSERT_STREQ(headNode->child()->child()->content(), "Hello, World!");

    DOMNode *bodyNode = headNode->next();
    ASSERT_STREQ(bodyNode->name(), "body");
    ASSERT_EQ(bodyNode->child(), bodyNode->last());
    ASSERT_EQ(bodyNode->child()->type(), DOMNode::TEXT);
    ASSERT_STREQ(bodyNode->child()->content(), "This is the body. ");

    free(copy);
}
