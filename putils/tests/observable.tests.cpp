// gtest
#include <gtest/gtest.h>

// putils
#include "putils/observable.hpp"

TEST(observable, add_observer) {
    putils::observable<> observable;

    bool done = false;
    observable.add_observer([&] { done = true; });
    observable.changed();

    EXPECT_TRUE(done);
}

TEST(observable, add_observer_unique_id) {
    putils::observable<> observable;

    const auto id1 = observable.add_observer([] {});
    const auto id2 = observable.add_observer([] {});

    EXPECT_NE(id1, id2);
}

TEST(observable, remove_observer) {
    putils::observable<> observable;

    bool done = false;
    const auto id = observable.add_observer([&] { done = true; });
    observable.remove_observer(id);
    observable.changed();

    EXPECT_FALSE(done);
}

TEST(observable, changed_arguments) {
    putils::observable<int> observable;

    int ret = 0;
    observable.add_observer([&](int i) { ret = i; });
    observable(42);

    EXPECT_EQ(ret, 42);
}