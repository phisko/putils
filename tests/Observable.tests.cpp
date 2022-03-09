#include <gtest/gtest.h>
#include "Observable.hpp"

TEST(Observable, addObserver) {
    putils::Observable<> observable;

    bool done = false;
    observable.addObserver([&] { done = true; });
    observable.changed();

    EXPECT_TRUE(done);
}

TEST(Observable, addObserver_UniqueID) {
    putils::Observable<> observable;

    const auto id1 = observable.addObserver([] {});
    const auto id2 = observable.addObserver([] {});

    EXPECT_NE(id1, id2);
}

TEST(Observable, removeObserver) {
    putils::Observable<> observable;

    bool done = false;
    const auto id = observable.addObserver([&] { done = true; });
    observable.removeObserver(id);
    observable.changed();

    EXPECT_FALSE(done);
}

TEST(Observable, changed_arguments) {
    putils::Observable<int> observable;

    int ret = 0;
    observable.addObserver([&](int i) { ret = i; });
    observable(42);

    EXPECT_EQ(ret, 42);
}