#include <gtest/gtest.h>
#include <iostream>
#include <cctype>
#include <functional>
std::string trim(const std::string & str, int (*condition)(int));
std::string trim(const std::string & str, const std::function<bool (const unsigned char)> & condition);


TEST(Task3_testing, constchar_isspace)
{
    EXPECT_EQ(trim("           test               ", isspace), "test");
    EXPECT_EQ(trim("                           test", isspace), "test");
    EXPECT_EQ(trim("test                         ", isspace), "test");
    EXPECT_EQ(trim("test", isspace), "test");

    EXPECT_EQ(trim("", isspace), "");
    EXPECT_EQ(trim(" ", isspace), "");
    EXPECT_EQ(trim("a", isspace), "a");
    EXPECT_EQ(trim(" a", isspace), "a");
    EXPECT_EQ(trim("a ", isspace), "a");
    EXPECT_EQ(trim(" a ", isspace), "a");
}

TEST(Task3_testing, lambda_overload)
{
    auto lambda = [](const unsigned char ch){return ch == 'a';};

    EXPECT_EQ(trim("aaatestaaa", lambda), "test");

    EXPECT_EQ(trim("", lambda), "");
    EXPECT_EQ(trim(" ", lambda), " ");
    EXPECT_EQ(trim("a", lambda), "");
    EXPECT_EQ(trim(" a", lambda), " ");
    EXPECT_EQ(trim("a ", lambda), " ");
    EXPECT_EQ(trim(" a ", lambda), " a ");
    EXPECT_EQ(trim("aaaaaaaaaaaaaaaaaaaax", lambda), "x");
    EXPECT_EQ(trim("xaaaaaaaaaaaaaaaaaaaa", lambda), "x");
    EXPECT_EQ(trim("aaaaaaaaaaaaaaaaaxaaaaaaaaaaaaaaaaaaaa", lambda), "x");
    EXPECT_EQ(trim("aaaxaaa", lambda), "x");

    EXPECT_EQ(trim("bbbbbbbbbb", [](const unsigned char ch){return true;}),"");

    EXPECT_EQ(trim("", [](const unsigned char ch){return true;}),"");
}

TEST(Task3_testing, string_ispace)
{
    std::string s = " ";
    EXPECT_EQ(trim(s, isspace), "");

    s = "";
    EXPECT_EQ(trim(s, isspace), "");

    s = "a";
    EXPECT_EQ(trim(s, isspace), "a");

    s = " a";
    EXPECT_EQ(trim(s, isspace), "a");

    s = "a ";
    EXPECT_EQ(trim(s, isspace), "a");

    s = " a ";
    EXPECT_EQ(trim(s, isspace), "a");


    s = "           test               ";
    EXPECT_EQ(trim(s, isspace), "test");

    s = "                           test";
    EXPECT_EQ(trim(s, isspace), "test");

    s = "test                         ";
    EXPECT_EQ(trim(s, isspace), "test");

    s = "test";
    EXPECT_EQ(trim(s, isspace), "test");

}
