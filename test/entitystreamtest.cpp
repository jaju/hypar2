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

typedef pair<EntityStream::EntityType, string> TypeDataPair;

TEST_F(EntityStreamTest, GetsAllEntitiesProperly) {
    EntityStream e(buffer->get(), buffer->length());
    EntityStream::EntityType type;
    vector<TypeDataPair> typesAndData;
    char *pEntityStr;
    while ((type = e.getNextEntity(&pEntityStr, true)) != EntityStream::END) {
        typesAndData.push_back(TypeDataPair(type, pEntityStr));
    }
    ASSERT_EQ(typesAndData.size(), 12);
    ASSERT_EQ(typesAndData[0].second, "html");
    ASSERT_EQ(typesAndData[0].first, EntityStream::ELEMENT);
    ASSERT_EQ(typesAndData[2].second, "!--This is my comment.--");
    ASSERT_EQ(typesAndData[2].first, EntityStream::COMMENT);
    ASSERT_EQ(typesAndData[7].second, "body bgcolor=\"white\" ");
    ASSERT_EQ(typesAndData[7].first, EntityStream::ELEMENT);
    ASSERT_EQ(typesAndData[8].second, "  Hello, World ! ");
    ASSERT_EQ(typesAndData[8].first, EntityStream::TEXT);
    ASSERT_EQ(typesAndData[11].second, "!- A broken comment! -");
    ASSERT_EQ(typesAndData[11].first, EntityStream::COMMENT);
}
