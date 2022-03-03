#include <gtest/gtest.h>
#include "file_extension.hpp"

TEST(file_extension, remove_directory) {
    EXPECT_EQ(putils::remove_directory("file.txt"), "file.txt");

    EXPECT_EQ(putils::remove_directory("dir/file.txt"), "file.txt");
    EXPECT_EQ(putils::remove_directory("./dir/file.txt"), "file.txt");
    EXPECT_EQ(putils::remove_directory("/dir/file.txt"), "file.txt");

    EXPECT_EQ(putils::remove_directory("dir/subdir/file.txt"), "file.txt");
    EXPECT_EQ(putils::remove_directory("./dir/subdir/file.txt"), "file.txt");
    EXPECT_EQ(putils::remove_directory("/dir/subdir/file.txt"), "file.txt");
}

TEST(file_extension, file_extension) {
    EXPECT_EQ(putils::file_extension("file.txt"), "txt");

    EXPECT_EQ(putils::file_extension("dir/file.txt"), "txt");
    EXPECT_EQ(putils::file_extension("./dir/file.txt"), "txt");
    EXPECT_EQ(putils::file_extension("/dir/file.txt"), "txt");

    EXPECT_EQ(putils::file_extension("dir/subdir/file.txt"), "txt");
    EXPECT_EQ(putils::file_extension("./dir/subdir/file.txt"), "txt");
    EXPECT_EQ(putils::file_extension("/dir/subdir/file.txt"), "txt");

    EXPECT_EQ(putils::file_extension("file.tmp.txt"), "txt");
}

TEST(file_extension, get_directory) {
    EXPECT_EQ(putils::get_directory("file.txt"), "");

    EXPECT_EQ(putils::get_directory("dir/file.txt"), "dir");
    EXPECT_EQ(putils::get_directory("./dir/file.txt"), "./dir");
    EXPECT_EQ(putils::get_directory("/dir/file.txt"), "/dir");

    EXPECT_EQ(putils::get_directory("dir/subdir/file.txt"), "dir/subdir");
    EXPECT_EQ(putils::get_directory("./dir/subdir/file.txt"), "./dir/subdir");
    EXPECT_EQ(putils::get_directory("/dir/subdir/file.txt"), "/dir/subdir");
}

TEST(file_extension, remove_extension) {
    EXPECT_EQ(putils::remove_extension("file.txt"), "file");

    EXPECT_EQ(putils::remove_extension("dir/file.txt"), "dir/file");
    EXPECT_EQ(putils::remove_extension("./dir/file.txt"), "./dir/file");
    EXPECT_EQ(putils::remove_extension("/dir/file.txt"), "/dir/file");

    EXPECT_EQ(putils::remove_extension("dir/subdir/file.txt"), "dir/subdir/file");
    EXPECT_EQ(putils::remove_extension("./dir/subdir/file.txt"), "./dir/subdir/file");
    EXPECT_EQ(putils::remove_extension("/dir/subdir/file.txt"), "/dir/subdir/file");

    EXPECT_EQ(putils::remove_extension("file.tmp.txt"), "file.tmp");
}
