#include <url.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <iostream>

using namespace std;
using namespace hy;

TEST(URL, ParsesAllComponents) {
    string urlStr("http://www.google.com/search?q=hello+world");
    URL u(urlStr);

    ASSERT_EQ(u.getProtocol(), "http");
    ASSERT_EQ(u.getCanonical(), "http://www.google.com:80/search?q=hello+world");
    ASSERT_EQ(u.getHost(), "www.google.com");
    ASSERT_EQ(u.getPort(), 80);
    ASSERT_EQ(u.getPath(), "/search");
    ASSERT_EQ(u.getCgiUri(), "q=hello+world");
    ASSERT_EQ(u.getURI(), "/search?q=hello+world");
    u.parseCgiUri();
    string qkey = "q";
    string qvalue;
    u.getCgiValue(qkey, qvalue);
    ASSERT_EQ(qvalue, "hello+world");
}

TEST(URL, GeneratesCorrectUrlFromRelativeAndBase) {
    string urlStr("http://www.google.com/search/?q=hello+world");
    string relativeUrlStr("blog/");
    URL u(relativeUrlStr, urlStr);

    ASSERT_EQ(u.getCanonical(), "http://www.google.com:80/search/blog/");

    urlStr = "http://www.google.com/search?q=hello+world";
    URL u2(relativeUrlStr, urlStr);
    ASSERT_EQ(u2.getCanonical(), "http://www.google.com:80/blog/");
}
