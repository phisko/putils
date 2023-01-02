// gtest
#include <gtest/gtest.h>

// stl
#include <thread>

// putils
#include "putils/timer.hpp"

static const auto sleep_duration = std::chrono::milliseconds(100);

TEST(timer, is_done) {
	const putils::timer t(std::chrono::seconds(0));
	EXPECT_TRUE(t.is_done());

	const putils::timer t2(sleep_duration);
	EXPECT_FALSE(t2.is_done());
	std::this_thread::sleep_for(sleep_duration);
	EXPECT_TRUE(t2.is_done());
}

TEST(timer, get_times_done) {
	const putils::timer t(sleep_duration);
	std::this_thread::sleep_for(sleep_duration);
	EXPECT_GT(t.get_times_done(), 1);
	EXPECT_LT(t.get_times_done(), 1.5f);
	std::this_thread::sleep_for(sleep_duration);
	EXPECT_GT(t.get_times_done(), 2);
	EXPECT_LT(t.get_times_done(), 2.5f);
}

TEST(timer, get_time_since_start) {
	const putils::timer t(sleep_duration);
	std::this_thread::sleep_for(sleep_duration);
	EXPECT_GT(t.get_time_since_start(), sleep_duration);
	EXPECT_LT(t.get_time_since_start(), sleep_duration * 2);
}

TEST(timer, get_time_since_done) {
	const putils::timer t(sleep_duration);
	std::this_thread::sleep_for(sleep_duration * 2);
	EXPECT_TRUE(t.get_time_since_done() > sleep_duration);
	EXPECT_TRUE(t.get_time_since_done() < sleep_duration * 2);
}

TEST(timer, get_time_left) {
	const putils::timer t(sleep_duration);
	EXPECT_GT(t.get_time_left(), std::chrono::seconds(0));
	std::this_thread::sleep_for(sleep_duration / 2);
	EXPECT_LT(t.get_time_left(), sleep_duration);
}

TEST(timer, restart) {
	putils::timer t(sleep_duration);
	const auto before = std::chrono::steady_clock::now();
	std::this_thread::sleep_for(sleep_duration);
	t.restart();
	std::this_thread::sleep_for(sleep_duration);
	const auto after = std::chrono::steady_clock::now();
	EXPECT_GT(t.get_start(), before);
	EXPECT_LT(t.get_start(), after);
}

TEST(timer, set_start) {
	putils::timer t(sleep_duration);
	const auto now = std::chrono::steady_clock::now();
	t.set_start(now);
	EXPECT_EQ(t.get_start(), now);
}

TEST(timer, get_duration) {
	const putils::timer t(sleep_duration);
	EXPECT_EQ(t.get_duration(), sleep_duration);
}

TEST(timer, set_duration) {
	putils::timer t(std::chrono::seconds(0));
	t.set_duration(sleep_duration);
	EXPECT_EQ(t.get_duration(), sleep_duration);
}
