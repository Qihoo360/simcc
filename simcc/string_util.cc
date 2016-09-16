#include "simcc/inner_pre.h"
#include "simcc/string_util.h"

namespace {

using namespace simcc;

struct NoCaseCompareChar {
    bool operator()(char l, char r)const {
        bool bEqual = (l == r);
        if (bEqual) {
            return true;
        }
        if (isalpha(static_cast<unsigned char>(l))) {
            if (isupper(static_cast<unsigned char>(l))) {
                return l == toupper(r);
            } else {
                return l == tolower(r);
            }
        }

        return bEqual;
    }
    bool operator()(wchar_t l , wchar_t r)const {
        bool bEqual = (l == r);

        if (bEqual) {
            return true;
        }

        if (iswalpha(l)) {
            if (iswupper(l)) {
                return l == static_cast<wchar_t>(towupper(r));
            } else {
                return l == static_cast<wchar_t>(towlower(r));
            }
        }

        return bEqual;
    }
} no_case_compare_char;


template<class T>
bool stringUtil_StartsWith(const T& str, const T& pattern, bool case_sensitive) {
    //H_ASSERT( str.length() >= pattern.length() );
    if (str.length() < pattern.length()) {
        return false;
    }

    if (case_sensitive) {
        return std::equal(pattern.begin() , pattern.end() , str.begin());
    } else {
        return std::equal(pattern.begin() , pattern.end() , str.begin() , no_case_compare_char);
    }
}

template<class T>
bool stringUtil_EndsWith(const T& str, const T& pattern, bool case_sensitive) {
    //H_ASSERT( str.length() >= pattern.length() );
    if (str.length() < pattern.length()) {
        return false;
    }

    if (case_sensitive) {
        return equal(pattern.rbegin() , pattern.rend() , str.rbegin());
    } else {
        return equal(pattern.rbegin() , pattern.rend() , str.rbegin() , no_case_compare_char);
    }
}




template<class T>
struct nocase_equal_char {
    T m_c;
    nocase_equal_char(T c): m_c(c) {
    }
    bool operator()(T c) {
        return no_case_compare_char(m_c , c);
    }
};


template<class T>
bool stringUtil_contains(const T& str , typename T::value_type c , bool case_sensitive) {
    if (case_sensitive) {
        return str.find(c , 0) != T::npos ;
    } else {
        return str.end() != std::find_if(str.begin() , str.end() , nocase_equal_char<typename T::value_type>(c));
    }
}
template<class T>
bool stringUtil_contains(const T& strL , const T& strR , bool case_sensitive) {
    if (case_sensitive) {
        return strL.end() != std::search(strL.begin() , strL.end() , strR.begin() , strR.end());
    } else {
        return strL.end() != std::search(strL.begin() , strL.end() , strR.begin() , strR.end() , no_case_compare_char);
    }
}

template<class _StringType>
void stringUtil_trim_charptr(_StringType& str, typename _StringType::const_pointer delims, bool left, bool right) {
    if (str.empty()) {
        return;
    }

    size_t stop_pos = (size_t)str.size() - 1;//included
    if (right) {
        stop_pos = str.find_last_not_of(delims);
    }

    if (stop_pos == _StringType::npos) {
        str = _StringType();
        return;
    }

    size_t start_pos = 0;//included
    if (left) {
        start_pos = str.find_first_not_of(delims);
    }

    if (start_pos == 0 && stop_pos == str.size() - 1) {
        return;
    }

    str = _StringType(str.data() + start_pos, stop_pos + 1 - start_pos);
}

template<class _StringType>
void stringUtil_trim_string(_StringType& str, const _StringType& delims, bool left, bool right) {
#if 1
    if (str.empty()) {
        return;
    }

    if (str.empty()) {
        return;
    }

    size_t stop_pos = (size_t)str.size() - 1;//included
    if (right) {
        stop_pos = str.find_last_not_of(delims);
    }

    if (stop_pos == _StringType::npos) {
        str = _StringType();
        return;
    }

    size_t start_pos = 0;//included
    if (left) {
        start_pos = str.find_first_not_of(delims);
    }

    if (start_pos == 0 && stop_pos == str.size() - 1) {
        return;
    }

    str = _StringType(str.data() + start_pos, stop_pos + 1 - start_pos);
#else
    if (str.empty()) {
        return;
    }

    if (right) {
        str.erase(str.find_last_not_of(delims) + 1);        // trim right
    }

    if (left) {
        str.erase(0, str.find_first_not_of(delims));        // trim left
    }
#endif
}

template< class _StringVector,
          class StringType,
          class _DelimType>
inline void _stringUtilSplit(
    _StringVector& ret,
    const StringType& str,
    const _DelimType& delims,
    unsigned int maxSplits) {
    unsigned int numSplits = 0;

    // Use STL methods
    size_t start, pos;
    start = 0;

    do {
        pos = str.find_first_of(delims, start);

        if (pos == start) {
            ret.push_back(StringType());
            start = pos + 1;
        } else if (pos == StringType::npos || (maxSplits && numSplits + 1 == maxSplits)) {
            // Copy the rest of the string
            ret.push_back(StringType());
            *(ret.rbegin()) = StringType(str.data() + start, str.size() - start);
            break;
        } else {
            // Copy up to delimiter
            //ret.push_back( str.substr( start, pos - start ) );
            ret.push_back(StringType());
            *(ret.rbegin()) = StringType(str.data() + start, pos - start);
            start = pos + 1;
        }

        ++numSplits;

    } while (pos != StringType::npos);
}


template< class _SliceVector,
          class StringType,
          class _DelimType>
void _stringUtilSplitStringToSlice(
    _SliceVector& ret,
    const StringType& str,
    const _DelimType& delims,
    unsigned int maxSplits) {
    unsigned int numSplits = 0;

    // Use STL methods
    size_t start, pos;
    start = 0;

    do {
        pos = str.find_first_of(delims, start);

        if (pos == start) {
            ret.push_back(Slice());
            start = pos + 1;
        } else if (pos == StringType::npos || (maxSplits && numSplits + 1 == maxSplits)) {
            // Copy the rest of the string
            ret.push_back(Slice(str.data() + start, str.size() - start));
            break;
        } else {
            // Copy up to delimiter
            //ret.push_back( str.substr( start, pos - start ) );
            ret.push_back(Slice(str.data() + start, pos - start));
            start = pos + 1;
        }

        ++numSplits;

    } while (pos != StringType::npos);
}



template< class StringType,
          class _DelimType>
inline void _stringUtilSplitStringToSlice(
    const StringType& str,
    const _DelimType& delims,
    Slice* ret, size_t& slices_count) {
    unsigned int numSplits = 0;

    // Use STL methods
    size_t start, pos;
    start = 0;

    do {
        pos = str.find_first_of(delims, start);

        if (pos == start) {
            ret[numSplits++] = Slice();
            start = pos + 1;
        } else if (pos == StringType::npos || (numSplits + 1 == slices_count)) {
            // Copy the rest of the string
            ret[numSplits++] = (Slice(str.data() + start, str.size() - start));
            break;
        } else {
            // Copy up to delimiter
            //ret.push_back( str.substr( start, pos - start ) );
            ret[numSplits++] = (Slice(str.data() + start, pos - start));
            start = pos + 1;
        }
    } while (pos != StringType::npos);

    slices_count = numSplits;
}

inline void _stringUtilSplitSliceToSlice(
    const Slice& str,
    const char& delim,
    std::vector<Slice>& ret, unsigned int maxSplits) {
    // Use STL methods
    size_t start, pos;
    start = 0;

    const char* p = NULL;
    do {
        //fix strchr compile warning
#ifdef H_OS_WINDOWS
        p = (const char*) memchr(start + const_cast<char*>(str.data()), delim , str.size() - start);
#else
        p = (const char*) memchr(start + str.data(), delim, str.size() - start);
#endif

        if (!p || p >= str.data() + str.size() || ((maxSplits) && (ret.size() + 1 == maxSplits))) {
            ret.push_back(Slice(str.data() + start, str.size() - start));
            break;
        }

        pos = p - str.data();

        if (pos == start) {
            ret.push_back(Slice());
            start = pos + 1;
        } else {
            ret.push_back(Slice(str.data() + start, pos - start));
            start = pos + 1;
        }
    } while (p);
}


inline void _stringUtilSplitSliceToSlice(
    const Slice& str,
    const char& delim,
    Slice* ret, size_t& slices_count) {
    unsigned int numSplits = 0;

    // Use STL methods
    size_t start, pos;
    start = 0;

    const char* p = NULL;
    do {
        //fix strchr compile warning
#ifdef H_OS_WINDOWS
        p = (const char*) memchr(start + const_cast<char*>(str.data()), delim , str.size() - start);
#else
        p = (const char*) memchr(start + str.data(), delim, str.size() - start);
#endif
        if (!p || p >= str.data() + str.size() || (numSplits == slices_count - 1)) {
            ret[numSplits++] = (Slice(str.data() + start, str.size() - start));
            break;
        }

        pos = p - str.data();

        if (pos == start) {
            ret[numSplits++] = Slice();
            start = pos + 1;
        } else {
            ret[numSplits++] = (Slice(str.data() + start, pos - start));
            start = pos + 1;
        }
    } while (p);

    slices_count = numSplits;
}

template<typename StringType>
void stringUtil_Split(const StringType& src, StringType& left, StringType& right, typename StringType::const_pointer pDelims , size_t nDelimsLength) {
    typename StringType::const_iterator iter = find_first_of(src.begin() , src.end() , pDelims , pDelims + nDelimsLength);
    if (src.end() == iter) {
        return ;
    }

    left.assign(src.begin() , iter);
    iter++;
    right.assign(iter , src.end());

}
}

namespace simcc {

const string StringUtil::kEmpty;

void StringUtil::Trim(string& str, const char* delims, bool left, bool right) {
    stringUtil_trim_charptr(str , delims , left , right);
}
void StringUtil::Trim(wstring& str, const wchar_t* delims, bool left, bool right) {
    stringUtil_trim_charptr(str , delims , left , right);
}

void StringUtil::Trim(string& str, bool left, bool right) {
    //const char* delims = " \t\r\n";
    static const string delims("\0 \t\r\n\v", 6);
    StringUtil::Trim(str, delims, left, right);
}

void StringUtil::Trim(string& str, const string& delims, bool left, bool right) {
    stringUtil_trim_string(str, delims, left, right);
}

void StringUtil::Trim(wstring& str, bool left, bool right) {
    //const wchar_t* delims = L" \t\r\n";
    static const wstring delims(L"\0 \t\r\n\v", 6);
    Trim(str, delims, left, right);
}

void StringUtil::Trim(wstring& str, const wstring& delims, bool left, bool right) {
    stringUtil_trim_string(str, delims, left, right);
}

void StringUtil::Split(std::vector<string>& ret, const string& str, const string& delims, unsigned int maxSplits) {
    _stringUtilSplit(ret , str , delims , maxSplits);
}

void StringUtil::Split(std::vector<string>& ret, const string& str, const string::value_type& delims, unsigned int maxSplits) {
    _stringUtilSplit(ret , str , delims , maxSplits);
}

void StringUtil::Split(std::vector<wstring>& ret, const wstring& str, const wstring& delims, unsigned int maxSplits) {
    _stringUtilSplit(ret , str , delims , maxSplits);
}

void StringUtil::Split(std::vector<wstring>& ret, const wstring& str, const wstring::value_type& delims, unsigned int maxSplits) {
    _stringUtilSplit(ret , str , delims , maxSplits);
}

void StringUtil::Split(std::vector<Slice>& ret, const string& str, int delims, unsigned int maxSplits) {
    _stringUtilSplitStringToSlice(ret , str , delims , maxSplits);
}
void StringUtil::Split(std::vector<Slice>& ret, const string& str, const string& delims, unsigned int maxSplits) {
    _stringUtilSplitStringToSlice(ret , str , delims , maxSplits);
}

void StringUtil::Split(const string& str, int delims, Slice slices[], size_t& slice_count) {
    _stringUtilSplitStringToSlice(str , delims , slices, slice_count);
}
void StringUtil::Split(const string& str, const string& delims, Slice slices[], size_t& slice_count) {
    _stringUtilSplitStringToSlice(str , delims , slices, slice_count);
}

void StringUtil::Split(const Slice& str, int delims, Slice slices[], size_t& slice_count) {
    _stringUtilSplitSliceToSlice(str , delims , slices, slice_count);
}

void StringUtil::Split(const Slice& str, int delims, std::vector<Slice>& slices, unsigned int maxSplits) {
    _stringUtilSplitSliceToSlice(str , delims , slices, maxSplits);
}

void StringUtil::Split(const string& src, string& left, string& right, const char* delims/* = "\t\n "*/) {
    stringUtil_Split(src , left , right , delims , strlen(delims));
}

void StringUtil::Split(const string& src, string& left, string& right, const string& delims/* = "\t\n "*/) {
    Split(src, left, right, delims.c_str());
}

void StringUtil::Split(const wstring& src, wstring& left, wstring& right, const wchar_t* delims/* = L"\t\n "*/) {
    stringUtil_Split(src , left , right , delims , wcslen(delims));
}

void StringUtil::Split(const wstring& src, wstring& left, wstring& right, const wstring& delims/* = L"\t\n "*/) {
    Split(src, left, right, delims.c_str());
}

namespace {
template< typename _String >
void _replace(_String& str, const _String& needle, const _String& new_value, size_t start_pos/* = 0*/, int replace_count /*= -1 */) {
    if (0 == replace_count) {
        return;
    }

    size_t i = 0;
    size_t pos = str.find(needle, start_pos);
    while (pos != _String::npos) {
        str.replace(pos, needle.size(), new_value);
        pos = str.find(needle, pos);
        if (++i >= (size_t)(replace_count)) {
            break;
        }
    }
}
}

void StringUtil::Replace(string& str, const string& needle, const string& new_value, size_t start_pos/* = 0*/, int replace_count /*= -1 */) {
    return _replace(str, needle, new_value, start_pos, replace_count);
}

void StringUtil::Replace(wstring& str, const wstring& needle, const wstring& new_value, size_t start_pos/* = 0*/, int replace_count /*= -1 */) {
    return _replace(str, needle, new_value, start_pos, replace_count);
}

bool StringUtil::StartsWith(const string& str, const string& pattern, bool case_sensitive) {
    return stringUtil_StartsWith(str , pattern , case_sensitive);
}

bool StringUtil::StartsWith(const wstring& str, const wstring& pattern, bool case_sensitive) {
    return stringUtil_StartsWith(str , pattern , case_sensitive);
}

bool StringUtil::EndsWith(const string& str, const string& pattern, bool case_sensitive) {
    return stringUtil_EndsWith(str , pattern , case_sensitive);
}

bool StringUtil::EndsWith(const wstring& str, const wstring& pattern, bool case_sensitive) {
    return stringUtil_EndsWith(str , pattern , case_sensitive);
}

bool StringUtil::Match(const string& str, const string& pattern, bool caseSensitive) {
    string tmpStr = str;
    string tmpPattern = pattern;

    if (!caseSensitive) {
        StringUtil::ToLower(tmpStr);
        StringUtil::ToLower(tmpPattern);
    }

    string::const_iterator strIt = tmpStr.begin();
    string::const_iterator patIt = tmpPattern.begin();
    string::const_iterator lastWildCardIt = tmpPattern.end();

    while (strIt != tmpStr.end() && patIt != tmpPattern.end()) {
        if (*patIt == '*') {
            lastWildCardIt = patIt;
            // Skip over looking for next character
            ++patIt;

            if (patIt == tmpPattern.end()) {
                // Skip right to the end since * matches the entire rest of the string
                strIt = tmpStr.end();
            } else {
                // scan until we find next pattern character
                while (strIt != tmpStr.end() && *strIt != *patIt) {
                    ++strIt;
                }
            }
        } else {
            if (*patIt != *strIt) {
                if (lastWildCardIt != tmpPattern.end()) {
                    // The last wildcard can match this incorrect sequence
                    // rewind pattern to wildcard and keep searching
                    patIt = lastWildCardIt;
                    lastWildCardIt = tmpPattern.end();
                } else {
                    // no wildwards left
                    return false;
                }
            } else {
                ++patIt;
                ++strIt;
            }
        }

    }

    // If we reached the end of both the pattern and the string, we succeeded
    if (patIt == tmpPattern.end() && strIt == tmpStr.end()) {
        return true;
    } else {
        return false;
    }

}

bool StringUtil::Match(const wstring& str, const wstring& pattern, bool caseSensitive) {
    wstring tmpStr = str;
    wstring tmpPattern = pattern;

    if (!caseSensitive) {
        StringUtil::ToLower(tmpStr);
        StringUtil::ToLower(tmpPattern);
    }

    wstring::const_iterator strIt = tmpStr.begin();
    wstring::const_iterator patIt = tmpPattern.begin();
    wstring::const_iterator lastWildCardIt = tmpPattern.end();

    while (strIt != tmpStr.end() && patIt != tmpPattern.end()) {
        if (*patIt == L'*') {
            lastWildCardIt = patIt;
            // Skip over looking for next character
            ++patIt;

            if (patIt == tmpPattern.end()) {
                // Skip right to the end since * matches the entire rest of the string
                strIt = tmpStr.end();
            } else {
                // scan until we find next pattern character
                while (strIt != tmpStr.end() && *strIt != *patIt) {
                    ++strIt;
                }
            }
        } else {
            if (*patIt != *strIt) {
                if (lastWildCardIt != tmpPattern.end()) {
                    // The last wildcard can match this incorrect sequence
                    // rewind pattern to wildcard and keep searching
                    patIt = lastWildCardIt;
                    lastWildCardIt = tmpPattern.end();
                } else {
                    // no wildwards left
                    return false;
                }
            } else {
                ++patIt;
                ++strIt;
            }
        }

    }

    // If we reached the end of both the pattern and the string, we succeeded
    if (patIt == tmpPattern.end() && strIt == tmpStr.end()) {
        return true;
    } else {
        return false;
    }

}

bool StringUtil::Contains(const string& motherStr, char pattern, bool caseSensitive) {
    return stringUtil_contains(motherStr , pattern , caseSensitive);
}

bool StringUtil::Contains(const wstring& motherStr, wchar_t pattern, bool caseSensitive) {
    return stringUtil_contains(motherStr , pattern , caseSensitive);
}

bool StringUtil::Contains(const string& motherStr, const string& pattern, bool caseSensitive) {
    return stringUtil_contains(motherStr , pattern , caseSensitive);
}

bool StringUtil::Contains(const wstring& motherStr, const wstring& pattern, bool caseSensitive) {
    return stringUtil_contains(motherStr , pattern , caseSensitive);
}

static unsigned char hexchars[] = "0123456789ABCDEF";

void StringUtil::URLEncode(const char* url, size_t url_len, string& strDest) {
#if 1
    size_t encoded_url_len = url_len * 3;
    strDest.resize(encoded_url_len);
    URLEncode(url, url_len, &strDest[0], encoded_url_len);
    strDest.resize(encoded_url_len);
#else
    //copy from php source code : ext/standard/url.c: PHPAPI char *php_url_encode(char const *s, int len, int *new_length)
    strDest.resize(url_len * 3);

    register unsigned char c;
    unsigned char* to, *start;
    unsigned char const* from, *end;

    start = to = (unsigned char*)(&strDest[0]);

    from = (unsigned char const*)url;
    end = from + url_len;

    while (from < end) {
        c = *from++;

        if (c == ' ') {
            *to++ = '+';
        } else if ((c < '0' && c != '-' && c != '.') ||
                   (c < 'A' && c > '9') ||
                   (c > 'Z' && c < 'a' && c != '_') ||
                   (c > 'z')) {
            to[0] = '%';
            to[1] = hexchars[c >> 4];
            to[2] = hexchars[c & 15];
            to += 3;
        } else {
            *to++ = c;
        }
    }
    *to = 0;

    strDest.resize(to - start);
#endif
}

bool StringUtil::URLEncode(const char* url, size_t url_len, char* edcoded_url, size_t& edcoded_url_len) {
    //copy from php source code : ext/standard/url.c: PHPAPI char *php_url_encode(char const *s, int len, int *new_length)

    register unsigned char c;
    unsigned char* to, *start, *to_end;
    unsigned char const* from, *end;

    start = to = (unsigned char*)(edcoded_url);
    to_end = to + edcoded_url_len;

    from = (unsigned char const*)url;
    end = from + url_len;

    while (from < end) {
        c = *from++;

        if (c == ' ') {
            if (to < to_end) {
                *to++ = '+';
            } else {
                return false;
            }
        } else if ((c < '0' && c != '-' && c != '.') ||
                   (c < 'A' && c > '9') ||
                   (c > 'Z' && c < 'a' && c != '_') ||
                   (c > 'z')) {
            if (to + 2 < to_end) {
                to[0] = '%';
                to[1] = hexchars[c >> 4];
                to[2] = hexchars[c & 15];
                to += 3;
            } else {
                return false;
            }
        } else {
            if (to < to_end) {
                *to++ = c;
            } else {
                return false;
            }
        }
    }
    *to = 0;
    edcoded_url_len = (to - start);
    return true;
}

static int php_htoi(const char* s) {
    int value;
    int c;

    c = ((const unsigned char*)s)[0];
    if (isupper(c)) {
        c = tolower(c);
    }
    value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

    c = ((const unsigned char*)s)[1];
    if (isupper(c)) {
        c = tolower(c);
    }
    value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

    return (value);
}

void StringUtil::URLDecode(const string& str, string& out) {
    //copy from php source code : ext/standard/url.c: PHPAPI int php_url_decode(char *str, int len)
#if 1
    URLDecode(str.data(), str.size(), out);
//         out.resize(str.size());
// #if 1
//         size_t decoded_url_len = str.size();
//         urldecode(str.data(), str.size(), &out[0], decoded_url_len);
//         out.resize(decoded_url_len);
#else
    char* dest = &out[0];
    const char* data = str.data();
    int len = (int)str.size();
    while (len--) {
        if (*data == '+') {
            *dest = ' ';
        } else if (*data == '%' && len >= 2 && isxdigit((int) * (data + 1))
                   && isxdigit((int) * (data + 2))) {
            *dest = (char) php_htoi(data + 1);
            data += 2;
            len -= 2;
        } else {
            *dest = *data;
        }
        data++;
        dest++;
    }

    out.resize(dest - out.data());
#endif
}

void StringUtil::URLDecode(const char* encoded_url, size_t encoded_url_len, char* decoded_url, size_t& decoded_url_len) {
    char* dest = decoded_url;
    const char* data = encoded_url;
    int len = (int)encoded_url_len;
    while (len--) {
        if (*data == '+') {
            *dest = ' ';
        } else if (*data == '%' && len >= 2 && isxdigit((int) * (data + 1))
                   && isxdigit((int) * (data + 2))) {
            *dest = (char) php_htoi(data + 1);
            data += 2;
            len -= 2;
        } else {
            *dest = *data;
        }
        data++;
        dest++;
    }

    decoded_url_len = dest - decoded_url;
}

void StringUtil::URLDecode(const char* encoded_url, size_t encoded_url_len, string& out) {
    out.resize(encoded_url_len);
    size_t decoded_url_len = encoded_url_len;
    URLDecode(encoded_url, encoded_url_len, &out[0], decoded_url_len);
    out.resize(decoded_url_len);
}

bool StringUtil::Equals(const string& str1, const string& str2, bool case_sensitive /*= true */) {
    if (case_sensitive) {
        return (str1 == str2) ? true : false;
    } else {
        return EqualsIgnoreCase(str1, str2);
    }
}

bool StringUtil::EqualsIgnoreCase(const string& str1, const string& str2) {
    if (str1.length() == str2.length()) {
        return std::equal(str1.begin() , str1.end() , str2.begin() , no_case_compare_char);
    }
    return false;
}

bool StringUtil::IsFloatNumber(string& s) {
    if (s.find('.') != string::npos
            || s.find('e') != string::npos
            || s.find('E') != string::npos) {
        return true;
    }

    return false;
}


string StringUtil::Rot13(const string& s) {
    return Rot13(s.data(), s.size());
}

string StringUtil::Rot13(const char* str, size_t len) {
    string ret;
    if (str == NULL || len == 0) {
        return ret;
    }

    int rot = 13;
    ret.resize(len);
    char* res = &ret[0];
    unsigned char c, e;

    for (size_t i = 0; i < len; i++) {
        c = str[i];
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
            res[i] = str[i];
        }
    }
    return ret;
}

void StringUtilW::Split(std::vector<wstring>& ret, const wstring& str, const wstring& delims, unsigned int maxSplits) {
    _stringUtilSplit(ret , str , delims , maxSplits);
}



void StringUtil::ToLower(string& str) {
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
        return tolower(c);
    });
}

void StringUtil::ToLower(wstring& str) {
    std::transform(str.begin(), str.end(), str.begin(), [](wchar_t c) {
        return towlower(c);
    });
}

void StringUtil::ToUpper(string& str) {
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
        return toupper(c);
    });
}

void StringUtil::ToUpper(wstring& str) {
    std::transform(str.begin(), str.end(), str.begin(), [](wchar_t c) {
        return towupper(c);
    });
}


}
