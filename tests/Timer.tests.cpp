#include <gtest/gtest.h>
#include <thread>
#include "Timer.hpp"

static const auto sleepDuration = std::chrono::milliseconds(100);

TEST(Timer, isDone) {
    const putils::Timer t(std::chrono::seconds(0));
    EXPECT_TRUE(t.isDone());

    const putils::Timer t2(sleepDuration);
    EXPECT_FALSE(t2.isDone());
    std::this_thread::sleep_for(sleepDuration);
    EXPECT_TRUE(t2.isDone());
}

TEST(Timer, getTimesDone) {
    const putils::Timer t(sleepDuration);
    std::this_thread::sleep_for(sleepDuration);
    EXPECT_GT(t.getTimesDone(), 1);
    EXPECT_LT(t.getTimesDone(), 1.5f);
    std::this_thread::sleep_for(sleepDuration);
    EXPECT_GT(t.getTimesDone(), 2);
    EXPECT_LT(t.getTimesDone(), 2.5f);
}

TEST(Timer, getTimeSinceStart) {
    const putils::Timer t(sleepDuration);
    std::this_thread::sleep_for(sleepDuration);
    EXPECT_GT(t.getTimeSinceStart(), sleepDuration);
    EXPECT_LT(t.getTimeSinceStart(), sleepDuration * 2);
}

TEST(Timer, getTimeSinceDone) {
    const putils::Timer t(sleepDuration);
    std::this_thread::sleep_for(sleepDuration * 2);
    EXPECT_TRUE(t.getTimeSinceDone() > sleepDuration);
    EXPECT_TRUE(t.getTimeSinceDone() < sleepDuration * 2);
}

TEST(Timer, getTimeLeft) {
    const putils::Timer t(sleepDuration);
    EXPECT_GT(t.getTimeLeft(), std::chrono::seconds(0));
    std::this_thread::sleep_for(sleepDuration / 2);
    EXPECT_LT(t.getTimeLeft(), sleepDuration);
}

TEST(Timer, restart) {
    putils::Timer t(sleepDuration);
    const auto before = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(sleepDuration);
    t.restart();
    std::this_thread::sleep_for(sleepDuration);
    const auto after = std::chrono::steady_clock::now();
    EXPECT_GT(t.getStart(), before);
    EXPECT_LT(t.getStart(), after);
}

TEST(Timer, setStart) {
    putils::Timer t(sleepDuration);
    const auto now = std::chrono::steady_clock::now();
    t.setStart(now);
    EXPECT_EQ(t.getStart(), now);
}

TEST(Timer, getDuration) {
    const putils::Timer t(sleepDuration);
    EXPECT_EQ(t.getDuration(), sleepDuration);
}

TEST(Timer, setDuration) {
    putils::Timer t(std::chrono::seconds(0));
    t.setDuration(sleepDuration);
    EXPECT_EQ(t.getDuration(), sleepDuration);
}
