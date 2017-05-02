
#include "test_common.h"

#include "simcc/tokener.h"

namespace {

void test_token_0() {
    const char* text = "0";
    simcc::Tokener token(text);
    H_TEST_ASSERT(token.size() == strlen(text));
    std::string s = token.NextSlice().ToString();
    H_TEST_ASSERT(s == "0");

    text = "0\t";
    token.Reset(text);
    s = token.NextSlice().ToString();
    H_TEST_ASSERT(s == "0");
}

void test_token_1() {
    const char* text = "a\t\nb\t c\td";
    simcc::Tokener token(text);
    H_TEST_ASSERT(token.size() == strlen(text));
    std::string s = token.NextString();
    H_TEST_ASSERT(s == "a");
    token.SkipSpaces();
    s = token.NextString();
    H_TEST_ASSERT(s == "b");
    token.SkipSpaces();
    s = token.NextString();
    H_TEST_ASSERT(s == "c");
    token.SkipSpaces();
    s = token.NextString();
    H_TEST_ASSERT(s == "d");

    token.Reset(text);
    token.SkipSpaces();
    s = token.NextSlice().ToString();
    H_TEST_ASSERT(s == "a");
    token.SkipSpaces();
    s = token.NextSlice().ToString();
    H_TEST_ASSERT(s == "b");
    token.SkipSpaces();
    s = token.NextSlice().ToString();
    H_TEST_ASSERT(s == "c");
    token.SkipSpaces();
    s = token.NextSlice().ToString();
    H_TEST_ASSERT(s == "d");
}

void test_token_2() {
    const char* text = "a\t\nb\t c\td";
    simcc::Tokener token(text);
    H_TEST_ASSERT(token.size() == strlen(text));
    H_TEST_ASSERT(token.GetCurrent() == text);
    token.SkipSpaces();
    H_TEST_ASSERT(token.GetCurrent() == text);
    std::string s = token.NextString();
    H_TEST_ASSERT(token.GetCurrent() >= text + 1);
    H_TEST_ASSERT(s == "a");
    token.SkipSpaces();
    H_TEST_ASSERT(token.GetCurrent() == text + 3);
    s = token.NextSlice().ToString();
    H_TEST_ASSERT(s == "b");
    token.SkipSpaces();
    H_TEST_ASSERT(token.GetCurrent() == text + 6);
    s = token.NextSlice().ToString();
    H_TEST_ASSERT(s == "c");
    token.SkipSpaces();
    s = token.NextSlice().ToString();
    H_TEST_ASSERT(s == "d");
}

void test_token_3() {
    const char* text = "abcd";
    simcc::Tokener token(text);
    H_TEST_ASSERT(token.size() == strlen(text));
    H_TEST_ASSERT(token.Next() == 'a');
    H_TEST_ASSERT(token.Next() == 'b');
    H_TEST_ASSERT(token.Next() == 'c');
    H_TEST_ASSERT(token.Next() == 'd');
    H_TEST_ASSERT(token.Next() == 0);
    token.Reset(text);
    H_TEST_ASSERT(token.Next() == 'a');
    H_TEST_ASSERT(token.Next() == 'b');
    H_TEST_ASSERT(token.Next() == 'c');
    token.Back();
    token.Back();
    token.Back();
    H_TEST_ASSERT(token.GetCurrent() == text);
}
}

TEST_UNIT(TestTokener) {
    test_token_0();
    test_token_1();
    test_token_2();
    test_token_3();
}


