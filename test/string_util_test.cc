#include "test_common.h"
#include "wcpp/exp.h"
#include "wcpp/string_util.h"

#include <map>
#include <iostream>

namespace {
void test_StringUtil_split_null_1() {
    std::string s = "|a||c|d";
    std::vector<std::string> v;
    wcpp::StringUtil::Split(v, s, "|");
    H_TEST_ASSERT(v.size() == 5);
    H_TEST_ASSERT(v[0] == "");
    H_TEST_ASSERT(v[1] == "a");
    H_TEST_ASSERT(v[2] == "");
    H_TEST_ASSERT(v[3] == "c");
    H_TEST_ASSERT(v[4] == "d");
}

void test_StringUtil_split_null_2() {
    std::string s = "|a||c|d|";
    std::vector<std::string> v;
    wcpp::StringUtil::Split(v, s, "|");
    H_TEST_ASSERT(v.size() == 6);
    H_TEST_ASSERT(v[0] == "");
    H_TEST_ASSERT(v[1] == "a");
    H_TEST_ASSERT(v[2] == "");
    H_TEST_ASSERT(v[3] == "c");
    H_TEST_ASSERT(v[4] == "d");
    H_TEST_ASSERT(v[5] == "");
}

void test_StringUtil_split_null_3() {
    std::string s = "|a||c;d|;";
    std::vector<std::string> v;
    wcpp::StringUtil::Split(v, s, "|;");
    H_TEST_ASSERT(v.size() == 7);
    H_TEST_ASSERT(v[0] == "");
    H_TEST_ASSERT(v[1] == "a");
    H_TEST_ASSERT(v[2] == "");
    H_TEST_ASSERT(v[3] == "c");
    H_TEST_ASSERT(v[4] == "d");
    H_TEST_ASSERT(v[5] == "");
    H_TEST_ASSERT(v[6] == "");
}


void test_StringUtil_split_null_4() {
    std::string s = "|a||c;d|;";
    std::vector<std::string> v;
    wcpp::StringUtil::Split(v, s, "|; ");
    H_TEST_ASSERT(v.size() == 7);
    H_TEST_ASSERT(v[0] == "");
    H_TEST_ASSERT(v[1] == "a");
    H_TEST_ASSERT(v[2] == "");
    H_TEST_ASSERT(v[3] == "c");
    H_TEST_ASSERT(v[4] == "d");
    H_TEST_ASSERT(v[5] == "");
    H_TEST_ASSERT(v[6] == "");
}

void test_StringUtil_split_5() {
    std::string s = "a|b|c|d|e";
    std::vector<std::string> v;
    wcpp::StringUtil::Split(v, s, "|; ", 2);
    H_TEST_ASSERT(v.size() == 2);
    H_TEST_ASSERT(v[0] == "a");
    H_TEST_ASSERT(v[1] == "b|c|d|e");
}

void test_StringUtil_split_6() {
    std::string s = "a|b|c|d|e";
    std::vector<std::string> v;
    wcpp::StringUtil::Split(v, s, '|', 2);
    H_TEST_ASSERT(v.size() == 2);
    H_TEST_ASSERT(v[0] == "a");
    H_TEST_ASSERT(v[1] == "b|c|d|e");
}

void test_StringUtil_split_7() {
    std::string s = "a|b|c|d|e";
    std::vector<std::string> v;
    wcpp::StringUtil::Split(v, s, '|');
    H_TEST_ASSERT(v.size() == 5);
    H_TEST_ASSERT(v[0] == "a");
    H_TEST_ASSERT(v[1] == "b");
    H_TEST_ASSERT(v[2] == "c");
    H_TEST_ASSERT(v[3] == "d");
    H_TEST_ASSERT(v[4] == "e");
}


void test_StringUtil_split_slice_6() {
    std::string s = "a|b|c|d|e";
    std::vector<wcpp::Slice> v;
    wcpp::StringUtil::Split(v, s, '|', 2);
    H_TEST_ASSERT(v.size() == 2);
    H_TEST_ASSERT(v[0] == "a");
    H_TEST_ASSERT(v[1] == "b|c|d|e");
}

void test_StringUtil_split_slice_7() {
    std::string s = "a|b|c|d|e";
    std::vector<wcpp::Slice> v;
    wcpp::StringUtil::Split(v, s, '|');
    H_TEST_ASSERT(v.size() == 5);
    H_TEST_ASSERT(v[0] == "a");
    H_TEST_ASSERT(v[1] == "b");
    H_TEST_ASSERT(v[2] == "c");
    H_TEST_ASSERT(v[3] == "d");
    H_TEST_ASSERT(v[4] == "e");
}


void test_StringUtil_split_slice_8() {
    std::string s = "a|b|c|d|e";
    wcpp::Slice v[3];
    size_t vcount = 3;
    wcpp::StringUtil::Split(s, '|', v, vcount);
    H_TEST_ASSERT(vcount == 3);
    H_TEST_ASSERT(v[0] == "a");
    H_TEST_ASSERT(v[1] == "b");
    H_TEST_ASSERT(v[2] == "c|d|e");
}


void test_StringUtil_split_slice_9() {
    std::string s = "a|b|c|d|e";
    wcpp::Slice v[8];
    size_t vcount = 8;
    wcpp::StringUtil::Split(s, '|', v, vcount);
    H_TEST_ASSERT(vcount == 5);
    H_TEST_ASSERT(v[0] == "a");
    H_TEST_ASSERT(v[1] == "b");
    H_TEST_ASSERT(v[2] == "c");
    H_TEST_ASSERT(v[3] == "d");
    H_TEST_ASSERT(v[4] == "e");
}


void test_StringUtil_split_slice_10() {
    std::string s = "a|b|c;d|e";
    wcpp::Slice v[8];
    size_t vcount = 8;
    wcpp::StringUtil::Split(s, ";|", v, vcount);
    H_TEST_ASSERT(vcount == 5);
    H_TEST_ASSERT(v[0] == "a");
    H_TEST_ASSERT(v[1] == "b");
    H_TEST_ASSERT(v[2] == "c");
    H_TEST_ASSERT(v[3] == "d");
    H_TEST_ASSERT(v[4] == "e");
}



void test_StringUtil_split_slice_11() {
    wcpp::Slice s = "a|b|c|d|e";
    wcpp::Slice v[8];
    size_t vcount = 8;
    wcpp::StringUtil::Split(s, '|', v, vcount);
    H_TEST_ASSERT(vcount == 5);
    H_TEST_ASSERT(v[0] == "a");
    H_TEST_ASSERT(v[1] == "b");
    H_TEST_ASSERT(v[2] == "c");
    H_TEST_ASSERT(v[3] == "d");
    H_TEST_ASSERT(v[4] == "e");
}

void test_StringUtil_split_slice_12() {
    wcpp::Slice s = "a|b|c|d|e";
    wcpp::Slice v[3];
    size_t vcount = 3;
    wcpp::StringUtil::Split(s, '|', v, vcount);
    H_TEST_ASSERT(vcount == 3);
    H_TEST_ASSERT(v[0] == "a");
    H_TEST_ASSERT(v[1] == "b");
    H_TEST_ASSERT(v[2] == "c|d|e");
}
void test_StringUtil_split_slice_13() {
    wcpp::Slice s("afb|c|d|e", 2);
    wcpp::Slice v[2];
    size_t vcount = 2;
    wcpp::StringUtil::Split(s, '|', v, vcount);
    H_TEST_ASSERT(vcount == 1);
    H_TEST_ASSERT(v[0] == "af");
}

}



TEST_UNIT(stringutil_split_null) {
    test_StringUtil_split_null_1();
    test_StringUtil_split_null_2();
    test_StringUtil_split_null_3();
    test_StringUtil_split_null_4();
    test_StringUtil_split_5();
    test_StringUtil_split_6();
    test_StringUtil_split_7();
    test_StringUtil_split_slice_6();
    test_StringUtil_split_slice_7();
    test_StringUtil_split_slice_8();
    test_StringUtil_split_slice_9();
    test_StringUtil_split_slice_10();
    test_StringUtil_split_slice_11();
    test_StringUtil_split_slice_12();
    test_StringUtil_split_slice_13();
}



TEST_UNIT(string_util_split_limit_test) {
    std::string s = "a|b|c";
    std::vector<std::string> v;
    wcpp::StringUtil::Split(v, s, "|", 2);
    H_TEST_ASSERT(v.size() == 2);
    H_TEST_ASSERT(v[0] == "a");
    H_TEST_ASSERT(v[1] == "b|c");
}


TEST_UNIT(string_util_split_limit_test2) {
    std::string s = "a|b";
    std::vector<std::string> v;
    wcpp::StringUtil::Split(v, s, "|", 2);
    H_TEST_ASSERT(v.size() == 2);
    H_TEST_ASSERT(v[0] == "a");
    H_TEST_ASSERT(v[1] == "b");
}


TEST_UNIT(string_util_split_limit_test3) {
    std::string s = "a|";
    std::vector<std::string> v;
    wcpp::StringUtil::Split(v, s, "|", 2);
    H_TEST_ASSERT(v.size() == 2);
    H_TEST_ASSERT(v[0] == "a");
    H_TEST_ASSERT(v[1] == "");
}

TEST_UNIT(string_util_split_slicetoslice_vec_limit) {
    std::string s = "a|bcde|def|sss";
    wcpp::Slice slice(s.data(), s.size());
    std::vector<wcpp::Slice> vv;
    wcpp::StringUtil::Split(slice, '|', vv, 2);
    H_TEST_ASSERT(vv.size() == 2);
    H_TEST_ASSERT(vv[0] == "a");
    H_TEST_ASSERT(vv[1] == "bcde|def|sss");

    vv.clear();
    wcpp::StringUtil::Split(slice, '|', vv);
    H_TEST_ASSERT(vv.size() == 4);
    H_TEST_ASSERT(vv[0] == "a");
    H_TEST_ASSERT(vv[1] == "bcde");
    H_TEST_ASSERT(vv[2] == "def");
    H_TEST_ASSERT(vv[3] == "sss");
}


namespace {

void test_string_util() {
    using namespace wcpp;

    //cout << "bool StringUtil::StartsWith( const string& str, const string& pattern, bool bCaseSensitive )"<<endl;
    H_TEST_ASSERT(!StringUtil::StartsWith("abcdef", "aBc"));
    H_TEST_ASSERT(StringUtil::StartsWith("abcdef", "aBc", false));

    H_TEST_ASSERT(!StringUtil::StartsWith("abcdef", "12"));
    H_TEST_ASSERT(!StringUtil::StartsWith("abcdef", "123", false));

    H_TEST_ASSERT(!StringUtil::StartsWith("abcdef", "abcdefasda"));
    H_TEST_ASSERT(!StringUtil::StartsWith("abcdef", "abcdefass", false));

    // cout << "bool StringUtil::EndsWith( const string& str, const string& pattern, bool bCaseSensitive )" << endl;

    H_TEST_ASSERT(!StringUtil::EndsWith("abcdef", "dEf"));
    H_TEST_ASSERT(StringUtil::EndsWith("abcdef", "dEf", false));

    H_TEST_ASSERT(!StringUtil::EndsWith("abcdef", "a32"));
    H_TEST_ASSERT(!StringUtil::EndsWith("abcdef", "a32", false));

    H_TEST_ASSERT(!StringUtil::EndsWith("abcdef", "asds2abcdef"));
    H_TEST_ASSERT(!StringUtil::EndsWith("abcdef", "xczxxabcdef", false));

    //cout<<"EqualsIgnoreCase"<<endl;

    H_TEST_ASSERT(StringUtil::EqualsIgnoreCase("abc", "AbC"));
    H_TEST_ASSERT(!StringUtil::EqualsIgnoreCase("abc", "AbCea"));

    //cout<<"bool StringUtil::Contains( const string& motherStr, const char pattern, bool caseSensitive )"<<endl;
    H_TEST_ASSERT(!StringUtil::Contains("abcdef", 'C', true));
    H_TEST_ASSERT(!StringUtil::Contains("abcdef", 'x', true));
    H_TEST_ASSERT(StringUtil::Contains("abcdef", 'C', false));
    H_TEST_ASSERT(!StringUtil::Contains("abcdef", 'x', false));

    //cout<<"bool StringUtil::Contains( const std::wstring& motherStr, const wchar_t pattern, bool caseSensitive )"<<endl;
    H_TEST_ASSERT(!StringUtil::Contains(L"abcdef", L'C', true));
    H_TEST_ASSERT(!StringUtil::Contains(L"abcdef", L'x', true));
    H_TEST_ASSERT(StringUtil::Contains(L"abcdef", L'C', false));
    H_TEST_ASSERT(!StringUtil::Contains(L"abcdef", L'x', false));

    //cout<<"bool StringUtil::Contains( const string& motherStr, const string& pattern, bool caseSensitive )"<<endl;
    H_TEST_ASSERT(!StringUtil::Contains("abcdef", "BCD", true));
    H_TEST_ASSERT(!StringUtil::Contains("abcdef", "efg", true));
    H_TEST_ASSERT(StringUtil::Contains("abcdef", "BcD", false));
    H_TEST_ASSERT(!StringUtil::Contains("abcdef", "efg", false));

    //cout<<"bool StringUtil::Contains( const std::wstring& motherStr, const std::wstring& pattern, bool caseSensitive )"<<endl;
    H_TEST_ASSERT(!StringUtil::Contains(L"abcdef", L"BCD", true));
    H_TEST_ASSERT(!StringUtil::Contains(L"abcdef", L"efg", true));
    H_TEST_ASSERT(StringUtil::Contains(L"abcdef", L"BcD", false));
    H_TEST_ASSERT(!StringUtil::Contains(L"abcdef", L"efg", false));
}

void test_split() {
    using namespace wcpp;

    std::vector<int64_t> vec;
    vec.push_back(12);
    vec.push_back(45);
    vec.push_back(78);
    vec.push_back(0);

    string strSplit = "";
    strSplit += std::to_string(vec[0]);
    strSplit += ",";
    strSplit += std::to_string(vec[1]);
    strSplit += " ";
    strSplit += std::to_string(vec[2]);
    strSplit += ";";
    strSplit += std::to_string(vec[3]);

    {
        std::vector<std::string> retval;
        StringUtil::Split(retval, strSplit.c_str(), "; ,");
        std::vector<std::string> TempVal;
        TempVal.push_back("12");
        TempVal.push_back("45");
        TempVal.push_back("78");
        TempVal.push_back("0");
        H_TEST_ASSERT(retval == TempVal);
    }



    {
        std::vector<std::string> retval;
        StringUtil::Split(retval, strSplit.c_str(), ",; ");
        std::vector<std::string> TempVal;
        TempVal.push_back("12");
        TempVal.push_back("45");
        TempVal.push_back("78");
        TempVal.push_back("0");
        H_TEST_ASSERT(retval == TempVal);
    }


    {
        string left, right;
        StringUtil::Split("1234", left, right, ", ;");
        H_TEST_ASSERT(left.empty() && right.empty());
        //std::cout << left << "\t" << right << std::endl;
        StringUtil::Split("12,34", left, right, ", ");
        H_TEST_ASSERT(("12" == left) && ("34" == right));
        StringUtil::Split(",1234", left, right, ", ");
        H_TEST_ASSERT(("" == left) && ("1234" == right));
        StringUtil::Split("1234,", left, right, ", ");
        H_TEST_ASSERT(("1234" == left) && ("" == right));
    }

}

void test_std_string_append() {
    std::string s;
    const char* s1 = "errno=0\r\n";
    const char* s2 = "mid=731d8949087ebf6cb8b85a49779cab51\r\n";
    s.append(s1);
    s.append(s2);
    char buf[1024] = {};
    snprintf(buf, sizeof(buf), "%s%s", s1, s2);
    H_TEST_ASSERT(s.size() == strlen(s1) + strlen(s2));
    H_TEST_ASSERT(s == buf);
}

void test_std_string_resize() {
    std::string s = "1234567890";
    s.resize(10);
    char* p10 = &s[0];
    H_TEST_ASSERT(s == "1234567890");
    s.resize(5);
    char* p5 = &s[0];
    H_TEST_ASSERT(s == "12345");
    s.resize(8);
    char* p8 = &s[0];
    H_TEST_ASSERT(p8 == p5 && p5 == p10);
    H_TEST_ASSERT(strncmp(s.data(), "12345", 5) == 0);
    s.resize(100);
    char* p100 = &s[0];
    H_TEST_ASSERT(strncmp(s.data(), "12345", 5) == 0);
    H_TEST_ASSERT(p100 != p8);

    s.resize(0);
    s.reserve(0);
}


void test_string_empty_null() {
    std::string s0 = "";
    std::string s;
    std::string s2 = "fasdfasdf";
    s2.clear();
    H_TEST_ASSERT(s0 == s && s == s2);
}

void test_std_string_assign_append() {
    unsigned char key1[] = { 0x30, 0x81, 0x00, 0x02, 0x81, 0x81, 0x00, 0xe2, 0xc7, 0x68, 0x22, 0x2d, 0x8d, 0xbd, 0xb5, 0x0e };
    unsigned char key2[] = { 0x30, 0x01, 0x00, 0x02, 0x00, 0x81, 0x00, 0xe2, 0xc7, 0x68, 0x22, 0x2d, 0x8d, 0xbd, 0xb5, 0x0e, 0xb5, 0x0e };
    std::string s1;
    std::string s2;
    s1 = std::string((char*)key1, sizeof(key1));
    s2.assign((char*)key1, sizeof(key1));
    H_TEST_ASSERT(s1.size() == sizeof(key1));
    H_TEST_ASSERT(s2.size() == sizeof(key1));
    H_TEST_ASSERT(s1 == s2);
    H_TEST_ASSERT(memcmp(s1.data(), s2.data(), sizeof(key1)) == 0);

    const char* sz = "11111";
    s2.assign(sz);
    H_TEST_ASSERT(s2 == sz);

    s1.append((char*)key2, sizeof(key2));
    H_TEST_ASSERT(s1.length() == sizeof(key1) + sizeof(key2));
    unsigned char buf[1024] = {};
    memcpy(buf, (char*)key1, sizeof(key1));
    memcpy(buf + sizeof(key1), (char*)key2, sizeof(key2));
    H_TEST_ASSERT(memcmp(s1.data(), buf, s1.length()) == 0);
}

static int FLAGS_md5_index = 4;

wcpp::Slice SpliteCommand(const char* command, size_t len) {
    int count = 0;
    const char* begin = command;
    for (size_t i = 0; i < len; ++i) {
        if (begin[i] == '\t') {
            ++count;
            if (count == FLAGS_md5_index) {
                return wcpp::Slice(begin + i + 1, 32);
            }
        }
    }

    return wcpp::Slice();
}

void test_slice() {
    std::map<wcpp::Slice, wcpp::Slice> ssmap;
    ssmap["a"] = "aaa";
    ssmap["b"] = "bbb";
    H_TEST_ASSERT(ssmap["a"] == "aaa");
    H_TEST_ASSERT(ssmap["b"] == "bbb");
}


void test_string_replace_1() {
    std::string base = "this is a test string. It is very fun.";
    std::string base_replaced1 = "thAA is a test string. It is very fun.";
    std::string base_replaced2 = "thAA AA a test string. It is very fun.";
    std::string base_replaced3 = "thAA AA a test string. It AA very fun.";
    std::string needle = "is";
    std::string new_value = "AA";

    std::string base1 = base;
    wcpp::StringUtil::Replace(base1, needle, new_value, 0, 1);
    H_TEST_ASSERT(base1 == base_replaced1);

    std::string base2 = base;
    wcpp::StringUtil::Replace(base2, needle, new_value, 0, 2);
    H_TEST_ASSERT(base2 == base_replaced2);

    std::string base3 = base;
    wcpp::StringUtil::Replace(base3, needle, new_value, 0, 3);
    H_TEST_ASSERT(base3 == base_replaced3);

    std::string base_all = base;
    wcpp::StringUtil::Replace(base_all, needle, new_value);
    H_TEST_ASSERT(base_all == base_replaced3);

    std::string base_replaced1_1 = "this AA a test string. It is very fun.";
    std::string base_replaced1_2 = "this AA a test string. It AA very fun.";

    std::string base_1_1 = base;
    wcpp::StringUtil::Replace(base_1_1, needle, new_value, 3, 1);
    H_TEST_ASSERT(base_1_1 == base_replaced1_1);

    std::string base_1_2 = base;
    wcpp::StringUtil::Replace(base_1_2, needle, new_value, 3, 2);
    H_TEST_ASSERT(base_1_2 == base_replaced1_2);
}
}

TEST_UNIT(trivial_std_string_test) {
    test_std_string_append();
    test_std_string_resize();
    test_std_string_assign_append();
    test_string_empty_null();
}


TEST_UNIT(string_util_test_1) {
    test_string_util();
    test_split();
    test_string_replace_1();
}


//TODO change 360 string
TEST_UNIT(test_SpliteCommand) {
    std::string data1 = "dfbc22a2ba2fbff94d4557a707fb9c15\twd\turlproc\t2.1.0.1001\t2d7d233ef132096f2f5ba843972e4fad\t255\taHR0cDovL3d3dy5iYWlkdS5jb20vcz93b3JkPSVDNiVCRCVCQSVGRSVDNiVGQiVCMyVCNSVENSVCRSVCNSVFNyVCQiVCMCZ0bj1zaXRlaGFvMTIzJmY9Mw==\t1331165275$";
    wcpp::Slice slice = SpliteCommand(data1.data(), data1.size());
    H_TEST_ASSERT(slice.ToString() == "2d7d233ef132096f2f5ba843972e4fad");

    std::string data2 = "dfbc22a2ba2fbff94d4557a707fb9c15\twd\turlproc";
    slice = SpliteCommand(data2.data(), data2.size());
    H_TEST_ASSERT(slice.ToString() == "");
}


TEST_UNIT(test_slice) {
    test_slice();
}


TEST_UNIT(test_StringUtil_spilt_1) {
    using namespace wcpp;
    std::string abc = "abc";
    std::vector<std::string> v;
    wcpp::StringUtil::Split(v, abc, ",");
    H_TEST_ASSERT(v.size() == 1 && v[0] == abc);
}

TEST_UNIT(StringUtil_split_no_needle_test) {
    std::string needle = "|";
    std::string s = "abc";
    std::vector<std::string> vv;
    wcpp::StringUtil::Split(vv, s, needle);
    H_TEST_ASSERT(vv.size() == 1);
    H_TEST_ASSERT(vv[0] == s);
}



TEST_UNIT(String_find_null) {
    std::string needle = "";
    std::string s = "abc";
    bool b = s.find(needle) != std::string::npos;
    H_TEST_ASSERT(b);
}


TEST_UNIT(StringUtil_contains_null) {
    std::string needle = "";
    std::string s = "abc";
    bool b = wcpp::StringUtil::Contains(s, needle);
    H_TEST_ASSERT(b);
    needle = "a";
    b = wcpp::StringUtil::Contains(s, needle);
    H_TEST_ASSERT(b);
    needle = "Ab";
    b = wcpp::StringUtil::Contains(s, needle);
    H_TEST_ASSERT(!b);
}


TEST_UNIT(StringUtil_split_slice) {
    std::string s = "a|b|c";
    std::vector<wcpp::Slice> vv;
    wcpp::StringUtil::Split(vv, s, "|");
    H_TEST_ASSERT(vv.size() == 3);
    H_TEST_ASSERT(vv[0] == "a");
}


TEST_UNIT(StringUtil_trim_string_char1) {
    std::string s = "ac";
    wcpp::StringUtil::Trim(s, 'c');
    H_TEST_ASSERT(s == "a");
}

TEST_UNIT(StringUtil_trim_string_char2) {
    std::string s = "c";
    wcpp::StringUtil::Trim(s, 'c');
    H_TEST_ASSERT(s == "");
}

TEST_UNIT(StringUtil_trim_string_char3) {
    std::string s = "cac";
    wcpp::StringUtil::Trim(s, 'c');
    H_TEST_ASSERT(s == "a");
}

TEST_UNIT(StringUtil_trim_string_char4) {
    std::string s = "acccc";
    wcpp::StringUtil::TrimLeft(s, 'c');
    H_TEST_ASSERT(s == "acccc");
}

TEST_UNIT(StringUtil_trim_string_char5) {
    std::string s = "acccc";
    wcpp::StringUtil::TrimRight(s, 'c');
    H_TEST_ASSERT(s == "a");
}

TEST_UNIT(StringUtil_trim_string_char6) {
    std::string s = "ac";
    wcpp::StringUtil::TrimRight(s, 'd');
    H_TEST_ASSERT(s == "ac");
}


TEST_UNIT(StringUtil_trim_slice_char1) {
    wcpp::Slice s = "ac";
    wcpp::StringUtil::Trim(s, 'c');
    H_TEST_ASSERT(s == "a");
}

TEST_UNIT(StringUtil_trim_slice_char2) {
    wcpp::Slice s = "c";
    wcpp::StringUtil::Trim(s, 'c');
    H_TEST_ASSERT(s == "");
}

TEST_UNIT(StringUtil_trim_slice_char3) {
    wcpp::Slice s = "cac";
    wcpp::StringUtil::Trim(s, 'c');
    H_TEST_ASSERT(s == "a");
}

TEST_UNIT(StringUtil_trim_slice_char4) {
    wcpp::Slice s = "acccc";
    wcpp::StringUtil::TrimLeft(s, 'c');
    H_TEST_ASSERT(s == "acccc");
}

TEST_UNIT(StringUtil_trim_slice_char5) {
    wcpp::Slice s = "acccc";
    wcpp::StringUtil::TrimRight(s, 'c');
    H_TEST_ASSERT(s == "a");
}

TEST_UNIT(StringUtil_trim_slice_char6) {
    wcpp::Slice s = "ac";
    wcpp::StringUtil::TrimRight(s, 'd');
    H_TEST_ASSERT(s == "ac");
}


TEST_UNIT(StringUtil_trim_string_string1) {
    std::string s = " \t\r\nac\t\r\n  ";
    wcpp::StringUtil::Trim(s);
    H_TEST_ASSERT(s == "ac");
}

TEST_UNIT(StringUtil_trim_string_string2) {
    std::string s = " \t\r\nac";
    wcpp::StringUtil::Trim(s);
    H_TEST_ASSERT(s == "ac");
}

TEST_UNIT(StringUtil_trim_string_string3) {
    std::string s = "ac\t\r\n  ";
    wcpp::StringUtil::Trim(s);
    H_TEST_ASSERT(s == "ac");
}

TEST_UNIT(StringUtil_trim_string_string4) {
    std::string s = "ac";
    wcpp::StringUtil::Trim(s);
    H_TEST_ASSERT(s == "ac");
}


TEST_UNIT(StringUtil_trim_string_string5) {
    std::string s = "cbabcc";
    wcpp::StringUtil::Trim(s, "bc");
    H_TEST_ASSERT(s == "a");
}


TEST_UNIT(test_string_trim_ending0) {
    char buf[8] = {};
    memset(buf, 'c', 2);
    std::string s(buf, sizeof(buf));
    wcpp::StringUtil::Trim(s);
    H_TEST_ASSERT(s.size() == 2);
}


TEST_UNIT(test_string_trim_ending0_2) {
    char buf[8] = { 0 };
    std::string s(buf, sizeof(buf));
    wcpp::StringUtil::Trim(s);
    H_TEST_ASSERT(s.size() == 0);
}


TEST_UNIT(test_string_trim_ending0_3) {
    std::string s;
    wcpp::StringUtil::Trim(s);
    H_TEST_ASSERT(s.size() == 0);
}

namespace {

static std::string StringRot13(const std::string& val) {

    std::string ret;

    if (val.size()) {

        ret.resize(val.size());

        size_t len = val.size();

        char* res = (char*)&ret[0];

        unsigned char c, e;

        int rot = 13;

        for (size_t i = 0; i < len; ++i) {

            c = val[i];

            if (c >= 'A' && c <= 'Z') {

                if ((e = c + rot) <= 'Z') {

                    res[i] = e;

                } else {

                    res[i] = c - rot;

                }

            } else if (c >= 'a' && c <= 'z') {

                if ((e = c + rot) <= 'z') {

                    res[i] = e;

                } else {

                    res[i] = c - rot;

                }

            } else {

                res[i] = val[i];

            }

        }

    }

    return ret;
}
}

TEST_UNIT(string_rot13_test) {
    std::string data[] = {
        "",
        "a",
        "alajsdflkj923470123974  ;ljflakjsd 019827340` 1u",
    };

    for (size_t i = 0; i < H_ARRAYSIZE(data); ++i) {
        std::string a = wcpp::StringUtil::Rot13(data[i]);
        std::string b = StringRot13(data[i]);
        H_TEST_ASSERT(a == b);
    }
}

TEST_UNIT(test_string_rot13) {
    std::string a = "hello world";
    std::string b = wcpp::StringUtil::Rot13(a);
    std::string c = wcpp::StringUtil::Rot13(b);
    H_TEST_ASSERT(a == c);

    a = "";
    b = wcpp::StringUtil::Rot13(a);
    c = wcpp::StringUtil::Rot13(b);
    H_TEST_ASSERT(a == c);

    a = "xx";
    b = wcpp::StringUtil::Rot13(a);
    c = wcpp::StringUtil::Rot13(b);
    H_TEST_ASSERT(a == c);
}

TEST_UNIT(test_split_trim) {
    std::string f = " false, true ";
    std::vector<std::string> v;
    wcpp::StringUtil::Split(v, f, ",");
    H_TEST_ASSERT(v[0] == " false");
    H_TEST_ASSERT(v[1] == " true ");
}


TEST_UNIT(test_split_and_trimSlice) {
    std::string f = "abcd\r\nbcdef\nadfsdf\r\ndefa";
    std::vector<wcpp::Slice> v;
    wcpp::StringUtil::Split(v, f, "\r");
    H_TEST_ASSERT(v.size() == 3);

    wcpp::StringUtil::Trim(v[0], '\n');
    wcpp::StringUtil::Trim(v[1], '\n');
    wcpp::StringUtil::Trim(v[2], '\n');
    H_TEST_ASSERT(v[0] == "abcd");
    H_TEST_ASSERT(v[1] == "bcdef\nadfsdf");
    H_TEST_ASSERT(v[2] == "defa");
}





TEST_UNIT(test_explode1) {
    std::string s = "abc||xx||yy";
    std::string delim = "||";

    std::vector<std::string> sv;
    wcpp::StringUtil::Explode(s, delim, sv, -1);
    H_TEST_ASSERT(sv.size() == 3);
    H_TEST_ASSERT(sv[0] == "abc");
    H_TEST_ASSERT(sv[1] == "xx");
    H_TEST_ASSERT(sv[2] == "yy");

    sv.clear();
    wcpp::StringUtil::Explode(s, delim, sv, 3);
    H_TEST_ASSERT(sv.size() == 3);
    H_TEST_ASSERT(sv[0] == "abc");
    H_TEST_ASSERT(sv[1] == "xx");
    H_TEST_ASSERT(sv[2] == "yy");


    sv.clear();
    wcpp::StringUtil::Explode(s, delim, sv, 2);
    H_TEST_ASSERT(sv.size() == 2);
    H_TEST_ASSERT(sv[0] == "abc");
    H_TEST_ASSERT(sv[1] == "xx");
}


TEST_UNIT(test_explode2) {
    std::string s = "abc||||yy";
    std::string delim = "||";

    std::vector<std::string> sv;
    wcpp::StringUtil::Explode(s, delim, sv, -1);
    H_TEST_ASSERT(sv.size() == 3);
    H_TEST_ASSERT(sv[0] == "abc");
    H_TEST_ASSERT(sv[1] == "");
    H_TEST_ASSERT(sv[2] == "yy");
}



TEST_UNIT(test_explode3) {
    std::string s = "abc";
    std::string delim = "||";

    std::vector<std::string> sv;
    wcpp::StringUtil::Explode(s, delim, sv, -1);
    H_TEST_ASSERT(sv.size() == 1);
    H_TEST_ASSERT(sv[0] == "abc");
}

TEST_UNIT(test_StringUtil_trim) {
    std::string l = "\r\n\t \t\t   ";
    l.append(5, '\0');
    std::string r = l;
    r.insert(0, 5, '\0');

    std::string m = "abc";

    std::string s = l + m + r;

    wcpp::StringUtil::TrimLeft(s);
    H_TEST_ASSERT(s == m + r);

    wcpp::StringUtil::TrimRight(s);
    H_TEST_ASSERT(s == m);
}




namespace {
void toLowerCase_cpp(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), tolower);
}


static void toLowerCase_c(std::string& str) {
    char* p = &str[0];
    const char* end = &str[0] + str.size();
    for (; p < end; ++p) {
        *p = tolower(*p);
    }
}
}

TEST_UNIT(toLowerCase_test) {
    std::string s = "AadjowuerqxAcasdjfklj29384r10293;zxcknv aasodfu-bWQ1cz0zODgwNWQ1MGVmY2ZkNThhYjhkMDMyZmEzZGQ5Mjk5ZQk1MzkwNDg4CSV1c2VycHJvZmlsZSVcbG9jYWwgc2V0dGluZ3NcdGVtcFxrdXBpbmdfc18yMDE3MS5leGUJMQkKDQpwcm9kdWN0PWRlZXBzY2FuDQpjb21ibz1wcmltYXJ5DQp2az00YTE4ZWE0Ng0KbWlkPTEyZmVhYWE2Zjc3MzU4Y2MwODUxYjFkNzNlOTIzMmRmDQpsYW5naWQ9MjA1Mg0KZXh0PWV4dDphZGlkOjEwMDAxfHxtb2RlOjB8fGhpLklCUzo5Nnx8aGkuSURBOmgxS0U1Szh0dEhxcHZRNTQvWWljUWFHeUVlK3V2b29uRjFLRTVLOHR0SHFwdlE1NC9ZaWNRYUd5RWUrdXZvb2JAaGwwSVFRNVRpMkl1SGwwSVFRNVRpMkl1Q3x8aGkuRkJTOjQ5MTUyfHxoaS5GREE6L21LVU1QRHJnMXZWTzQyNG5ueHFMS2pRNkFrRnFhMllsRFVsdlJVVVNoM211aUBMcFhhY3RrRmMyWWxENXdVVVQzV3x8aGkuR0VOOuWuieijheeoi2930123khflasdhf;  7528723412 21s1afsdfasdf54q6238471*234132/4dasf        sdf asdf asdjf acB";
    std::string o = s;
    wcpp::StringUtil::ToLower(s);
    H_TEST_ASSERT(s != o && stricmp(s.data(), o.data()) == 0);
    const int loop = 1;
    unsigned long begin = wcpp::utcmicrosecond();
    for (int i = 0; i < loop; ++i) {
        toLowerCase_cpp(s);
    }
    unsigned long end = wcpp::utcmicrosecond();

    unsigned long cppcost = end - begin;

    begin = wcpp::utcmicrosecond();
    for (int i = 0; i < loop; ++i) {
        toLowerCase_c(s);
    }
    end = wcpp::utcmicrosecond();
    unsigned long ccost = end - begin;

    (void)cppcost;
    (void)ccost;
}



TEST_UNIT(test_split_slice_errorbug) {
    std::string d = "18995e0972c953c232a886c494bce61c\t389464\t\\PROGRAM FILES\\360\\360safe\\softmgr\\somquickinst.dll\t1\t\nxxx";
    wcpp::Slice m(d.data(), d.size() - 4);//"18995e0972c953c232a886c494bce61c\t389464\t\\PROGRAM FILES\\360\\360safe\\softmgr\\somquickinst.dll\t1\t" \nxxx
    wcpp::Slice vv[6];
    size_t slice_count = 6;
    static const std::string tab = "\t";
    wcpp::StringUtil::Split(m, '\t', vv, slice_count);
    H_TEST_ASSERT(slice_count == 5);
    H_TEST_ASSERT(strncmp(vv[0].data(), "18995e0972c953c232a886c494bce61c", 32) == 0);
    H_TEST_ASSERT(strncmp(vv[1].data(), "389464", 6) == 0);
    H_TEST_ASSERT(strncmp(vv[2].data(), "\\PROGRAM FILES\\360\\360safe\\softmgr\\somquickinst.dll", vv[2].size()) == 0);
    H_TEST_ASSERT(strncmp(vv[3].data(), "1", 1) == 0);
    H_TEST_ASSERT(strncmp(vv[4].data(), "", 0) == 0);
}

TEST_UNIT(std_stringstream_test) {
    std::stringstream oss;
    oss << "hello world";

    std::string a = oss.str();
    oss.str("");
    oss << "world";
    std::string b = oss.str();

    H_TEST_ASSERT(a == "hello world");
    H_TEST_ASSERT(b == "world");
}




