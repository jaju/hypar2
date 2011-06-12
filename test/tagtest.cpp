#include <tag.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <vector>
#include <iostream>
#include "textutils.hpp"

using namespace std;
using namespace hy;

TEST(Tag, ShouldParseASimpleTag) {
    const char *tag = "html";
    textutils::CBuffer c(tag, strlen(tag));
    Tag t(c.get(), true);

    ASSERT_STREQ(t.name(), "html");
}

TEST(Tag, ShouldParseAttributesOfATag) {
    const char *tag = "body bgcolor=\"white\" fgcolor='black and white' foo=\"bar > and > baaz\"";
    textutils::CBuffer c(tag, strlen(tag));
    Tag t(c.get(), true);

    ASSERT_STREQ(t.name(), "body");
    ASSERT_STREQ(t.findProperty("bgcolor"), "white");
    ASSERT_STREQ(t.findProperty("fgcolor"), "black and white");
    ASSERT_STREQ(t.findProperty("foo"), "bar > and > baaz");
}

TEST(Tag, ShouldIdentifyEndTags) {
    const char *tag = "/html bgcolor=\"white\"";
    textutils::CBuffer c(tag, strlen(tag));
    Tag t(c.get(), true);

    ASSERT_STREQ(t.name(), "html");
    ASSERT_TRUE(t.endTag());
    ASSERT_STREQ(t.findProperty("bgcolor"), "white");
}
