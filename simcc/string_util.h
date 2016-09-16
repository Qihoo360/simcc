#pragma once

#include <vector>
#include <algorithm>
#include <ctype.h>
#include <wctype.h>

#include "simcc/inner_pre.h"
#include "simcc/slice.h"
#include "simcc/memmem.h"


namespace simcc {

// 字符串相关的一些常用操作
class SIMCC_EXPORT StringUtil {
public:
    //  @brief Removes any whitespace characters, which are one of them
    //       " " (ASCII 32 (0x20)) SPACE
    //       "\t" (ASCII 9 (0x09)) tab
    //       "\n" (ASCII 10 (0x0A)) new line
    //       "\r" (ASCII 13 (0x0D)) carriage ret
    //       "\0" (ASCII 0 (0x00)) NUL
    //       "\v" (ASCII 11 (0x0B)) vertical tab
    //     The user may specify whether they want to trim only the
    //    beginning or the end of the String ( the default action is
    //    to trim both).
    //  @param str[in,out], the source string, and the result will stored in it.
    //  @param cutset[in], all leading and trailing characters contained in cutset will be removed.
    //  @param left, trim the beginning of the string
    //  @param right, trim the end of the string
    static void Trim(string& str, bool left = true, bool right = true);
    static void Trim(wstring& str, bool left = true, bool right = true);
    static void Trim(string& str, const string& cutset, bool left = true, bool right = true);
    static void Trim(wstring& str, const wstring& cutset, bool left = true, bool right = true);
    static void Trim(string& str, const char* cutset, bool left = true, bool right = true);
    static void Trim(wstring& str, const wchar_t* cutset, bool left = true, bool right = true);

    static void TrimLeft(string& str) {
        Trim(str, true, false);
    }
    static void TrimLeft(wstring& str) {
        Trim(str, true, false);
    }
    static void TrimLeft(string& str, const string& delims) {
        Trim(str, delims, true, false);
    }
    static void TrimLeft(wstring& str, const wstring& delims) {
        Trim(str, delims, true, false);
    }
    static void TrimLeft(string& str, const char* delims) {
        Trim(str, delims, true, false);
    }
    static void TrimLeft(wstring& str, const wchar_t* delims) {
        Trim(str, delims, true, false);
    }

    static void TrimRight(string& str) {
        Trim(str, false, true);
    }
    static void TrimRight(wstring& str) {
        Trim(str, false, true);
    }
    static void TrimRight(string& str, const string& delims) {
        Trim(str, delims, false, true);
    }
    static void TrimRight(wstring& str, const wstring& delims) {
        Trim(str, delims, false, true);
    }
    static void TrimRight(string& str, const char* delims) {
        Trim(str, delims, false, true);
    }
    static void TrimRight(wstring& str, const wchar_t* delims) {
        Trim(str, delims, false, true);
    }


    // @brief trim a char
    template<class _StringType> static void Trim(_StringType& str, char c, bool left = true, bool right = true);
    template<class _StringType> static void TrimLeft(_StringType& str, char c) {
        Trim(str, c, true, false);
    }
    template<class _StringType> static void TrimRight(_StringType& str, char c) {
        Trim(str, c, false, true);
    }


    // @brief Replaces a section equaling to <code>needle</code> of the current string with the new substring <code>new_value</code>
    // @param string & str [in,out], -
    // @param const string & needle -
    // @param const string & new_value -
    // @param string::size_type start_pos -
    // @param int replace_count - If it is -1, replaces all the <code>needle</code> by <code>new_value</code>
    // @return void -
    static void Replace(string& str, const string& needle, const string& new_value, size_t start_pos = 0, int replace_count = -1);
    static void Replace(wstring& str, const wstring& needle, const wstring& new_value, size_t start_pos = 0, int replace_count = -1);


    // Returns a StringVector that contains all the substrings delimited
    // by any of the characters in the passed <code>delims</code> argument.
    // @param vec[out], the result substrings are stored here.
    // @param delims A list of delimiter characters to split by
    // @param max_splits The maximum number of splits to perform (0 for unlimited splits). If this
    //     parameters is > 0, the splitting process will stop after this many splits, left to right.
    static void Split(std::vector<string>& vec, const string& str, const string& cutset = "\t\n ", unsigned int max_splits = 0);
    static void Split(std::vector<wstring>& vec, const wstring& str, const wstring& cutset = L"\t\n ", unsigned int max_splits = 0);

    static void Split(std::vector<string>& vec, const string& str, const string::value_type& cutset, unsigned int max_splits = 0);
    static void Split(std::vector<wstring>& vec, const wstring& str, const wstring::value_type& cutset, unsigned int max_splits = 0);

    static void Split(std::vector<Slice>& vec, const string& str, int cutset, unsigned int max_splits = 0);
    static void Split(std::vector<Slice>& vec, const string& str, const string& cutset = "\t\n ", unsigned int max_splits = 0);

    // @brief
    //   <code>
    //     string s = "a|b|c|d|e";
    //     Slice v[2];
    //     Split(s, '|', v, 2);  //after Split, v[0]=="a", v[1] == "b|c|d|e", vc == 2
    //   </code>
    //
    //   <code>
    //     string s = "a|b|c";
    //     Slice v[8];
    //     Split(s, '|', v, 8);  //after Split, v[0]=="a", v[1] == "b", v[2] == "c", vc == 3
    //   </code>
    // @param const string & str -
    // @param int delims -
    // @param[out] Slice slices[] -
    // @param[in,out] size_t & slice_count -
    // @return void -
    static void Split(const string& str, int cutset, Slice slices[], size_t& slice_count);
    static void Split(const string& str, const string& cutset, Slice slices[], size_t& slice_count);
    static void Split(const Slice& str, int cutset, Slice slices[], size_t& slice_count);
    static void Split(const Slice& str, int cutset, std::vector<Slice>& slices, unsigned int max_splits = 0);



    /**
    * Split a string into tow strings using the special characters .
    * e.g. src="abc, hello world "  if cutset="," then left="abc", right=" hello world "
    * @warning If any of delimiters was found, we just return, left string and right string will not be changed.
    * @param src The source string
    * @param left The left string separated by cutset
    * @param left The right string separated by cutset
    * @param cutset A list of delimiter characters to split by. We only use the first one when come up against a delimiter
    */
    static void Split(const string& src, string& left, string& right, const string& cutset = "\t\n ");
    static void Split(const wstring& src, wstring& left, wstring& right, const wstring& cutset = L"\t\n ");
    static void Split(const string& src, string& left, string& right, const char* cutset = "\t\n ");
    static void Split(const wstring& src, wstring& left, wstring& right, const wchar_t* cutset = L"\t\n ");

    template<class _SourceStringType, class _SubStringType>
    static void Explode(const _SourceStringType& source, const _SourceStringType& cutset, std::vector<_SubStringType>& return_value, int limit = -1);

    static void ToLower(string& str);
    static void ToLower(wstring& str);
    static void ToUpper(string& str);
    static void ToUpper(wstring& str);

    // string compare
    // @param case_sensitive true If we compare the string with case sensitively
    static bool Equals(const string& str1, const string& str2, bool case_sensitive = true);

    // @brief string compare ignoring case sensitively
    static bool EqualsIgnoreCase(const string& str1, const string& str2);

    static string Rot13(const string& val);
    static string Rot13(const char* str, size_t len);

    // @brief     Returns whether the string begins with the pattern passed in.
    // @param[in] pattern The pattern to compare with.
    // @param[in] case_sensitive true case sensitive, false ignore the case
    static bool StartsWith(const string& str, const string& pattern, bool case_sensitive = true);
    static bool StartsWith(const wstring& str, const wstring& pattern, bool case_sensitive = true);

    // @brief     Returns whether the string ends with the pattern passed in.
    // @param[in] pattern The pattern to compare with.
    // @param[in] case_sensitive true case sensitive, false ignore the case
    static bool EndsWith(const string& str, const string& pattern, bool case_sensitive = true);
    static bool EndsWith(const wstring& str, const wstring& pattern, bool case_sensitive = true);


    // Simple pattern-matching routine allowing a wild card pattern.
    // @param str String to test
    // @param pattern Pattern to match against; which can include simple '*' wildcards
    // @param case_sensitive Whether the match is case sensitive or not
    static bool Match(const string& str, const string& pattern, bool case_sensitive = true);
    static bool Match(const wstring& str, const wstring& pattern, bool case_sensitive = true);


    // @brief Reports whether a char or a substr is within mother
    // @param mother, the mother string to test
    // @param pattern, the pattern string or char to find
    // @param[in] case_sensitive true case sensitive, false ignore the case
    // @return bool, return true if the occurrence of pattern within the motherStr or false
    static bool Contains(const string& mother, char pattern, bool case_sensitive = true);
    static bool Contains(const wstring& mother, wchar_t pattern, bool case_sensitive = true);
    static bool Contains(const string& mother, const string& pattern, bool case_sensitive = true);
    static bool Contains(const wstring& mother, const wstring& pattern, bool case_sensitive = true);


    // @brief This function is convenient when encoding a string
    //     to be used in a query part of a URL, as a convenient way
    //     to pass variables to the next page
    // @param const char * url -
    // @param size_t url_len -
    // @param[in] char * edcoded_url -
    // @param[in,out] size_t & edcoded_url_len -
    // @return bool -
    static bool URLEncode(const char* url, size_t url_len, char* edcoded_url, size_t& edcoded_url_len);
    static string URLEncode(const string& str) {
        string out;
        URLEncode(str, out);
        return out;
    }
    static void URLEncode(const string& str, string& out) {
        URLEncode(str.data(), str.size(), out);
    }
    static void URLEncode(const char* url, size_t url_len, string& out);


    static string URLDecode(const string& str) {
        string out;
        URLDecode(str, out);
        return out;
    }
    static void URLDecode(const string& str, string& out);
    static void URLDecode(const char* encoded_url, size_t encoded_url_len, string& out);
    static void URLDecode(const char* encoded_url, size_t encoded_url_len, char* decoded_url, size_t& decoded_url_len);
    static void URLDecode(string& str) {
        URLDecode(str, str);
    }

    // query whether parameter string is a float number string or not.
    static bool IsFloatNumber(string& s);

    static const string kEmpty;
};

// 字符串的一些常用操作(宽字符版本)
class SIMCC_EXPORT StringUtilW {
public:
    static void ToLower(wstring& str) {
        std::transform(str.begin(), str.end(), str.begin(), towlower);
    }
    static void ToUpper(wstring& str) {
        std::transform(str.begin(), str.end(), str.begin(), towupper);
    }

    static void Split(std::vector<wstring>& resultSubstrsVec, const wstring& str, const wstring& delims = L"\t\n ", unsigned int maxSplits = 0);
};

template<class _Tx, class _Ty>
inline void StringUtil::Explode(const _Tx& source, const _Tx& delim, std::vector<_Ty>& return_value, int limit) {
    const char* p1, *p2, *endp;
    const char* str = source.data();
    size_t len = source.size();
    endp = str + source.size();

    if (limit == 0 || limit == 1) {
        return_value.push_back(_Ty(str, endp - str));
        return;
    }

    p1 = str;
    size_t delim_len = delim.size();
    p2 = (const char*)memmem(str, len, delim.data(), delim_len);
    if (p2 == NULL) {
        return_value.push_back(_Ty(str, len));
    } else {
        do {
            return_value.push_back(_Ty(p1, p2 - p1));
            p1 = p2 + delim_len;
        } while ((p2 = (char*)memmem(p1, len - (p1 - str), delim.data(), delim_len)) != NULL && (limit == -1 || --limit > 1));

        if (p1 < endp) {
            if ((p2 = (char*)memmem(p1, len - (p1 - str), delim.data(), delim_len)) != NULL) {
                endp = p2;
            }
            return_value.push_back(_Ty(p1, endp - p1));
        }
    }
}

template<class _StringType>
inline void StringUtil::Trim(_StringType& str, char c, bool left /*= true*/, bool right /*= true*/) {
    if (str.empty()) {
        return;
    }

    int stop_pos = (int)str.size() - 1;//included
    if (right) {
        for (; stop_pos >= 0; --stop_pos) {
            if (str[stop_pos] != c) {
                break;
            }
        }
    }

    if (stop_pos < 0) {
        str = _StringType();
        return;
    }

    int start_pos = 0;//included
    if (left) {
        for (; start_pos <= stop_pos; ++start_pos) {
            if (str[start_pos] != c) {
                break;
            }
        }
    }

    if (start_pos == 0 && stop_pos == (int)str.size() - 1) {
        return;
    }

    str = _StringType(str.data() + start_pos, stop_pos + 1 - start_pos);
}


} // namespace

