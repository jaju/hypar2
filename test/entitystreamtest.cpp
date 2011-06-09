#include <entitystream.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <vector>
#include <iostream>
#include "textutils.hpp"

using namespace std;
using namespace hy;

class EntityStreamTest : public ::testing::Test {
    public:
        EntityStreamTest() {
            const char *pText = "<html><head><!--This is my comment.--><title>My Title</title></head><body bgcolor=\"white\" >  Hello, World ! </body></html><!- A broken comment! ->";
            buffer = new textutils::CBuffer(pText, strlen(pText));
        }
        ~EntityStreamTest() {
            delete buffer;
        }

    protected:
       textutils::CBuffer *buffer;
};

TEST_F(EntityStreamTest, GetsAllEntitiesProperly) {
    EntityStream e(buffer->get(), buffer->length());
    EntityStream::EntityType type;
    vector<string> entityStrings;
    vector<EntityStream::EntityType> types;
    char *pEntityStr;
    while ((type = e.getNextEntity(&pEntityStr, true)) != EntityStream::END) {
        entityStrings.push_back(pEntityStr);
        types.push_back(type);
    }
    ASSERT_EQ(entityStrings.size(), 12);
    ASSERT_EQ(entityStrings[0], "html");
    ASSERT_EQ(types[0], EntityStream::ELEMENT);
    ASSERT_EQ(entityStrings[2], "!--This is my comment.--");
    ASSERT_EQ(types[2], EntityStream::COMMENT);
    ASSERT_EQ(entityStrings[7], "body bgcolor=\"white\" ");
    ASSERT_EQ(types[7], EntityStream::ELEMENT);
    ASSERT_EQ(entityStrings[8], "  Hello, World ! ");
    ASSERT_EQ(types[8], EntityStream::TEXT);
    ASSERT_EQ(entityStrings[11], "!- A broken comment! -");
    ASSERT_EQ(types[11], EntityStream::COMMENT);
}
