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

    ASSERT_STREQ(t.m_pName, "html");
}

TEST(Tag, ShouldParseAttributesOfATag) {
    const char *tag = "body bgcolor=\"white\" fgcolor='black and white' foo=\"bar > and > baaz\"";
    textutils::CBuffer c(tag, strlen(tag));
    Tag t(c.get(), true);

    ASSERT_STREQ(t.m_pName, "body");
    ASSERT_STREQ(t.findProperty("bgcolor"), "white");
    ASSERT_STREQ(t.findProperty("fgcolor"), "black and white");
    ASSERT_STREQ(t.findProperty("foo"), "bar > and > baaz");
}

TEST(Tag, ShouldParseAttributesWithoutQuotes) {
    const char *tag = "body bgcolor=white fgcolor='black and white' foo=bar";
    textutils::CBuffer c(tag, strlen(tag));
    Tag t(c.get(), true);

    ASSERT_STREQ(t.m_pName, "body");
    ASSERT_STREQ(t.findProperty("bgcolor"), "white");
    ASSERT_STREQ(t.findProperty("fgcolor"), "black and white");
    ASSERT_STREQ(t.findProperty("foo"), "bar");
}

TEST(Tag, ShouldIdentifyEndTags) {
    const char *tag = "/html bgcolor=\"white\"";
    textutils::CBuffer c(tag, strlen(tag));
    Tag t(c.get(), true);

    ASSERT_STREQ(t.m_pName, "html");
    ASSERT_TRUE(t.m_bEndTag);
    ASSERT_STREQ(t.findProperty("bgcolor"), "white");
}
