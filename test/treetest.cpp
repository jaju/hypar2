#include "tree.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

class Dummy {
    public:
    void reset() {}
};

typedef TreeNode<Dummy> T;


//TEST(TreeNode, SimpleInsert)

/*
 *               root
 *               /
 *               a-b-c-last1
 *               /
 *               d-e-last2
 */
TEST(TreeNode, ChildrenInsertsWork) {
    T *root = T::create();
    T *a = T::create();
    T *b = T::create();
    T *c = T::create();
    T *d = T::create();
    T *e = T::create();
    T *last1 = T::create();
    T *last2 = T::create();

    root->attachAsChild(d);
    root->insertAsChild(a);

    ASSERT_EQ(root->last(), a);
    ASSERT_EQ(root->child(), a);
    ASSERT_EQ(root->last(), a);
    ASSERT_EQ(a->child(), d);
    ASSERT_EQ(a->last(), d);
    ASSERT_EQ(d->parent(), a);
    ASSERT_EQ(a->parent(), root);

    root->attachAsChild(b);
    root->attachAsChild(c);
    root->attachAsChild(last1);
    a->attachAsChild(e);
    a->attachAsChild(last2);


    ASSERT_EQ(root->last(), last1);
    ASSERT_EQ(a->last(), last2);
    ASSERT_EQ(a->next(), b);
    ASSERT_EQ(a->child(), d);
    ASSERT_EQ(d->next(), e);
}
