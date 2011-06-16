#include "tree.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <iostream>

using namespace std;

class Dummy {
    public:
    void reset() {}
};

typedef TreeNode<Dummy> T;

/*
 *               root
 *               /
 *               a-b-c-last1
 *               /
 *               d-e-last2
 */
class TreeTest : public ::testing::Test {
    protected:
        T *root, *a, *b, *c, *d, *e, *last1, *last2;

        TreeTest() {
            root = T::create();
            a = T::create();
            b = T::create();
            c = T::create();
            d = T::create();
            e = T::create();
            last1 = T::create();
            last2 = T::create();

            root->attachAsChild(d);
            root->insertAsChild(a);

            root->attachAsChild(b);
            root->attachAsChild(c);
            root->attachAsChild(last1);
            a->attachAsChild(e);
            a->attachAsChild(last2);

        }

        ~TreeTest() {
            delete root;
        }
};

TEST_F(TreeTest, ChildrenInsertsWork) {
    ASSERT_EQ(root->child(), a);
    ASSERT_EQ(a->child(), d);
    ASSERT_EQ(d->parent(), a);
    ASSERT_EQ(a->parent(), root);

    ASSERT_EQ(root->last(), last1);
    ASSERT_EQ(a->last(), last2);
    ASSERT_EQ(a->next(), b);
    ASSERT_EQ(d->next(), e);
}

TEST_F(TreeTest, LevelsAreInitializedCorrectly) {
    ASSERT_EQ(root->level(), 0);
    ASSERT_EQ(a->level(), 1);
    ASSERT_EQ(b->level(), 1);
    ASSERT_EQ(c->level(), 1);
    ASSERT_EQ(last1->level(), 1);
    ASSERT_EQ(d->level(), 2);
    ASSERT_EQ(e->level(), 2);
    ASSERT_EQ(last2->level(), 2);

    ASSERT_EQ(root->horizontalLevel(), 0);
    ASSERT_EQ(a->horizontalLevel(), 0);
    ASSERT_EQ(b->horizontalLevel(), 1);
    ASSERT_EQ(c->horizontalLevel(), 2);
    ASSERT_EQ(last1->horizontalLevel(), 3);
    ASSERT_EQ(d->horizontalLevel(), 0);
    ASSERT_EQ(e->horizontalLevel(), 1);
    ASSERT_EQ(last2->horizontalLevel(), 2);
}
