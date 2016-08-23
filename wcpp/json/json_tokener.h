// Created: 2009/08/07
// Author: weizili

#pragma once
#include "wcpp/tokener.h"
#include "json.h"

namespace wcpp {
namespace json {

class Object;

// A JSONTokener takes a source string and extracts characters and tokens from
// it. It is used by the JSONObject and JSONArray constructors to parse
// JSON source strings.

class JSONTokener : public wcpp::Tokener {
public:
    // Construct a JSONTokener from a string. You can use this token to
    // build a JSONObject
    // 
    // @param ps     A source string.
    // @param ps_len The length of the source string
    //   if it equals -1, we will use strlen(ps) to calculate the length.
    JSONTokener(const std::string& s);

    JSONTokener(const char* ps, const wcpp::int32 ps_len = -1);

    ~JSONTokener();

    // Get the characters up to the next close quote character.
    // Backslash processing is done. The formal JSON format does not
    // allow strings in single quotes, but an implementation is allowed to
    // accept them.
    // @param quote The quoting character, either
    //      <code>"</code>(wcpp::f64 quote) or
    //      <code>'</code>(single quote).
    // @param rs the string we needed
    // @return true, if no error happened
    //   false, if Unterminated string.
    bool NextString(char quote, std::string& rs);

    bool NextString(char quote, bool parse_protobuf, std::string& rs);

    // Get the next value. The value can be a Boolean, Double, Integer,
    // JSONArray, JSONObject, Long, or std::string, or the JSONObject.NULL object.
    // @return An object. or NULL if something wrong
    Object* NextValue(JSONParser* parser);

    // @brief skip comment strings
    //   Skip c-style or cpp-style comment
    // @note when return false, we don't skip any character
    // @return true, if skip successfully
    //         false, if the comment format is wrong.
    bool SkipComment();
    bool SkipCStyleComment();   /* this is a c-style comment*/
    bool SkipCppStyleComment(); // this is a cpp-style comment

    template<typename _stream>
    bool ConvertUnicodeToUTF8(wcpp::uint32 unicode, _stream& stream);

    // Convert an unicode escape string sequence to an unicode number
    bool DecodeUnicodeEscapeSequence(wcpp::uint32& unicode);

    // Convert an unicode number to UTF8 string
    static std::string ConvertUnicodeToUTF8(wcpp::uint32 unicode);

    // Convert an unicode number to an unicode escape string sequence. e.g: 0x8912 --> "\\u8912"
    static void EncodeUnicodeNumber(wcpp::uint32 codepoint, wcpp::DataStream& stream);

    static void EncodeUnicodeNumber(wcpp::uint32 codepoint, char encbuf[/*12*/], wcpp::uint32& encbuf_len);

private:
    // Convert an unicode 4 bytes escape string sequence to an unicode number
    bool DecodeUnicode4BytesSequence(wcpp::uint32& unicode);


private:
    enum { kDefaultBufferSize = 512 };
    wcpp::DataStream buf_; // The data cache buffer to improve performance
};


inline JSONTokener::JSONTokener(const std::string& s)
    : Tokener(s)
    , buf_(kDefaultBufferSize) {
}

inline JSONTokener::JSONTokener(const char* ps, const wcpp::int32 len)
    : Tokener(ps, len)
    , buf_(kDefaultBufferSize) {
}

inline JSONTokener::~JSONTokener() {
}

inline bool JSONTokener::NextString(char quote, bool parse_protobuf, std::string& rs) {
    buf_.Reset();

#define Z16 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    static const char escape[256] = {
        Z16, Z16, 0, 0, '\"', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '/',
        Z16, Z16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '\\', 0, 0, 0,
        0, 0, '\b', 0, 0, 0, '\f', 0, 0, 0, 0, 0, 0, 0, '\n', 0,
        0, 0, '\r', 0, '\t', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        Z16, Z16, Z16, Z16, Z16, Z16, Z16, Z16
    };
#undef Z16
    register char c = 0;
    for (;;) {
        c = Next();

        switch (c) {
        case 0:
            //fprintf( stderr, "Unterminated string\n" );
            return false;
        case '\\':  // Escape character
            c = Next();
#if 1
            if (escape[(unsigned char)c]) {
                buf_.Write(escape[(unsigned char)c]);
            } else if (c == 'u') {
                // e.g. \u1524 is a unicode character
                wcpp::uint32 unicode = 0;
                if (!DecodeUnicodeEscapeSequence(unicode)) {
                    //fprintf( stderr, "Illegal unicode escape.\n" );
                    return false;
                }

                if (!JSONTokener::ConvertUnicodeToUTF8(unicode, buf_)) {
                    //fprintf( stderr, "Illegal escape unicode [%c]\n", c );
                    return false;
                }
            } else {
                if (parse_protobuf) {
                    //TODO convert "\346\226\260\346\265\252\347\275\221" to a utf8 string
                    buf_.Write('\\');
                    buf_.Write(c);
                } else {
                    //fprintf( stderr, "Illegal escape.[%c]\n", c );
                    return false;
                }
            }
#else
            switch (c) {
            case '"':
                buf_ << c;
                break;
            case '\'':
                buf_ << c;
                break;
            case '/':
                buf_ << c;
                break;
            case '\\':
                buf_ << c;
                break;
            case 'b':
                buf_ << '\b';
                break;
            case 'f':
                buf_ << '\f';
                break;
            case 'n':
                buf_ << '\n';
                break;
            case 'r':
                buf_ << '\r';
                break;
            case 't':
                buf_ << '\t';
                break;
            case 'u': {
                // e.g. \u1524 is a unicode character
                wcpp::uint32 unicode = 0;
                if (!DecodeUnicodeEscapeSequence(unicode)) {
                    //fprintf( stderr, "Illegal unicode escape.\n" );
                    return false;
                }

                if (!JSONTokener::ConvertUnicodeToUTF8(unicode, buf_)) {
                    return false;
                }
            }
            break;
            default:
                //fprintf( stderr, "Illegal escape.[%c]\n", c );
                return false;
            }
#endif
            break;
        default:

            if (c == quote) {
                rs.assign(buf_.data(), buf_.size());
                return true;
            }

            buf_.Write(c);
            break;
        }
    } // end of for ( ;; )
}

inline bool JSONTokener::NextString(char quote, std::string& rs) {
    return NextString(quote, false, rs);
}

inline Object* JSONTokener::NextValue(JSONParser* parser) {
    if (!SkipComment()) {
        //printf("wrong format of comment");
        parser->set_error(JSONParser::kCommentFormatError, this);
        return 0;
    }

    char c = NextClean();
    switch (c) {
    case '"':
    case '\'': {
        JSONString* jstring = new JSONString();
        if (!NextString(c, jstring->value())) {
            delete jstring;
            jstring = NULL;
            parser->set_error(JSONParser::kJSONStringNotQuoted, this->GetCurrentPosition());
        }
        return jstring;
    }
    case '{':
        Back();
        {
            JSONObject* jobj = new JSONObject();
            if (jobj->Parse(this) > 0 && jobj->error() == JSONParser::kNoError) {
                return jobj;
            } else {
                parser->set_error(jobj->error(), this->GetCurrentPosition());
                delete jobj;
                return NULL;
            }
        }
    case '[':
    case '(':
        Back();
        {
            JSONArray* jarray = new JSONArray();
            if (jarray->Parse(this) > 0 && jarray->error() == JSONParser::kNoError) {
                return jarray;
            } else {
                parser->set_error(jarray->error(), this);
                delete jarray;
                return NULL;
            }
        }
    default:
        //Handle unquoted text like: true, false, or null, or it can be a number.
        break;
    }

    /*
     * Handle unquoted text. This could be the values true, false, or
     * null, or it can be a number. An implementation (such as this one)
     * is allowed to also accept non-standard forms.
     *
     * Accumulate characters until we reach the end of the text or a
     * formatting character.
     */

    //the static table of ",:]}/\\\"[{;=#" and space, tab and control characters
    static char specialchars[256] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };

    const char* startpos = GetCurrent() - 1;
    while (specialchars[(int)c]) {
        c = Next();
    }

    Back();

    if (GetCurrent() <= startpos) {
        //printf( "Miss value\n" );
        parser->set_error(JSONParser::kBlankValue, this);
        return NULL;
    }

    return JSONObject::ConvertToObject(startpos, GetCurrent() - startpos, parser, this);
}


inline bool JSONTokener::SkipComment() {
    char c = NextClean();
    if (c != '/') {
        Back();
        return true;
    }

    c = Next();
    bool successful = false;

    if (c == '*') {
        successful = SkipCStyleComment();
    } else if (c == '/') {
        successful = SkipCppStyleComment();
    }

    if (successful) {
        return SkipComment();
    } else {
        Back();
        return false;
    }
}

inline bool JSONTokener::SkipCStyleComment() {
    const char* iCurrentPos = GetCurrent();
    while (!IsEnd()) {
        if (Next() == '*' && Current() == '/') {
            Next();//skip '/'
            return true;
        }
    }

    SetCurrent(iCurrentPos);
    return false;
}


inline bool JSONTokener::SkipCppStyleComment() {
    const char* iCurrentPos = GetCurrent();
    {
        if (SkipToNextLine()) {
            return true;
        }
    }

    SetCurrent(iCurrentPos);
    return false;
}


inline std::string JSONTokener::ConvertUnicodeToUTF8(wcpp::uint32 unicode) {
    std::string result;

    // based on description from http://en.wikipedia.org/wiki/UTF-8

    if (unicode <= 0x7f) {
        result.resize(1);
        result[0] = static_cast<char>(unicode);
    } else if (unicode <= 0x7FF) {
        result.resize(2);
        result[1] = static_cast<char>(0x80 | (0x3f & unicode));
        result[0] = static_cast<char>(0xC0 | (0x1f & (unicode >> 6)));
    } else if (unicode <= 0xFFFF) {
        result.resize(3);
        result[2] = static_cast<char>(0x80 | (0x3f & unicode));
        result[1] = 0x80 | static_cast<char>((0x3f & (unicode >> 6)));
        result[0] = 0xE0 | static_cast<char>((0xf & (unicode >> 12)));
    } else if (unicode <= 0x10FFFF) {
        result.resize(4);
        result[3] = static_cast<char>(0x80 | (0x3f & unicode));
        result[2] = static_cast<char>(0x80 | (0x3f & (unicode >> 6)));
        result[1] = static_cast<char>(0x80 | (0x3f & (unicode >> 12)));
        result[0] = static_cast<char>(0xF0 | (0x7 & (unicode >> 18)));
    }

    return result;
}


template<typename _stream>
inline bool JSONTokener::ConvertUnicodeToUTF8(wcpp::uint32 unicode, _stream& stream) {
    // based on description from http://en.wikipedia.org/wiki/UTF-8
    if (unicode <= 0x7f) {
        //             stream.resize( 1 );
        //             stream[0] = static_cast<char>( unicode );
        stream.put(static_cast<char>(unicode));
    } else if (unicode <= 0x7FF) {
        //             stream.resize( 2 );
        //             stream[1] = static_cast<char>( 0x80 | ( 0x3f & unicode ) );
        //             stream[0] = static_cast<char>( 0xC0 | ( 0x1f & ( unicode >> 6 ) ) );
        stream.put(0xC0 | ((unicode >> 6) & 0xFF));
        stream.put(0x80 | ((unicode & 0x3F)));
    } else if (unicode <= 0xFFFF) {
        stream.put(0xE0 | ((unicode >> 12) & 0xFF));
        stream.put(0x80 | ((unicode >> 6) & 0x3F));
        stream.put(0x80 | (unicode & 0x3F));
        //             stream.resize( 3 );
        //             stream[2] = static_cast<char>( 0x80 | ( 0x3f & unicode ) );
        //             stream[1] = 0x80 | static_cast<char>( ( 0x3f & ( unicode >> 6 ) ) );
        //             stream[0] = 0xE0 | static_cast<char>( ( 0xf & ( unicode >> 12 ) ) );
    } else if (unicode <= 0x10FFFF) {
        stream.put(0xF0 | ((unicode >> 18) & 0xFF));
        stream.put(0x80 | ((unicode >> 12) & 0x3F));
        stream.put(0x80 | ((unicode >> 6) & 0x3F));
        stream.put(0x80 | (unicode & 0x3F));
        //             stream.resize( 4 );
        //             stream[3] = static_cast<char>( 0x80 | ( 0x3f & unicode ) );
        //             stream[2] = static_cast<char>( 0x80 | ( 0x3f & ( unicode >> 6 ) ) );
        //             stream[1] = static_cast<char>( 0x80 | ( 0x3f & ( unicode >> 12 ) ) );
        //             stream[0] = static_cast<char>( 0xF0 | ( 0x7 & ( unicode >> 18 ) ) );
    } else {
        return false;
    }

    return true;
}

inline bool JSONTokener::DecodeUnicodeEscapeSequence(wcpp::uint32& unicode) {
    if (!DecodeUnicode4BytesSequence(unicode)) {
        return false;
    }

    if (unicode >= 0xD800 && unicode <= 0xDBFF) {
        // surrogate pairs
        if (GetReadableSize() < 6) {
            return false;
        }

        unsigned int surrogatePair;

        if (Next() == '\\' && Next() == 'u') {
            if (DecodeUnicode4BytesSequence(surrogatePair)) {
                unicode = 0x10000 + ((unicode & 0x3FF) << 10) + (surrogatePair & 0x3FF);
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    return true;
}

inline bool JSONTokener::DecodeUnicode4BytesSequence(wcpp::uint32& unicode) {
    enum { Unicode_Len = 4 };// an unicode has 4 bytes

    if (GetReadableSize() < Unicode_Len) {
        return false;
    }

    unicode = 0;

    for (int index = 0; index < Unicode_Len; ++index) {
        char c = Next();
        unicode = unicode << 4;

        if (c >= '0'  &&  c <= '9') {
            unicode += c - '0';
        } else if (c >= 'a'  &&  c <= 'f') {
            unicode += c - 'a' + 10;
        } else if (c >= 'A'  &&  c <= 'F') {
            unicode += c - 'A' + 10;
        } else {
            return false;
        }
    }

    return true;
}

inline void JSONTokener::EncodeUnicodeNumber(wcpp::uint32 codepoint, char encbuf[/*12*/], wcpp::uint32& encbuf_len) {
    char* begin = (char*)encbuf;

    // codepoint is in BMP
    if (codepoint < 0x10000) {
        begin[0] = '\\';
        begin[1] = 'u';

        //uint16-t high byte
        char _0h = ((codepoint & 0xF000) >> 12);
        if (_0h <= 9) {
            begin[2] = _0h + '0';
        } else {
            begin[2] = _0h + 'a' - 10;
        }

        char _0l = ((codepoint & 0x0F00) >> 8);
        if (_0l <= 9) {
            begin[3] = _0l + '0';
        } else {
            begin[3] = _0l + 'a' - 10;
        }


        //uint16-t low byte
        char _1h = ((codepoint & 0xF0) >> 4);
        if (_1h <= 9) {
            begin[4] = _1h + '0';
        } else {
            begin[4] = _1h + 'a' - 10;
        }

        char _1l = (codepoint & 0x0F);
        if (_1l <= 9) {
            begin[5] = _1l + '0';
        } else {
            begin[5] = _1l + 'a' - 10;
        }

        encbuf_len = 6;
    } else { // not in BMP -> construct a UTF-16 surrogate pair
        codepoint -= 0x10000;
        int32_t first = 0xD800 | ((codepoint & 0xffc00) >> 10);
        int32_t last = 0xDC00 | (codepoint & 0x003ff);

        //Write First
        {

            begin[0] = '\\';
            begin[1] = 'u';

            //uint16-t high byte
            char _0h = ((first & 0xF000) >> 12);
            if (_0h <= 9) {
                begin[2] = _0h + '0';
            } else {
                begin[2] = _0h + 'a' - 10;
            }

            char _0l = ((first & 0x0F00) >> 8);
            if (_0l <= 9) {
                begin[3] = _0l + '0';
            } else {
                begin[3] = _0l + 'a' - 10;
            }


            //uint16-t low byte
            char _1h = ((first & 0xF0) >> 4);
            if (_1h <= 9) {
                begin[4] = _1h + '0';
            } else {
                begin[4] = _1h + 'a' - 10;
            }

            char _1l = (first & 0x0F);
            if (_1l <= 9) {
                begin[5] = _1l + '0';
            } else {
                begin[5] = _1l + 'a' - 10;
            }
        }


        //Write last
        {
            begin[6] = '\\';
            begin[7] = 'u';

            //uint16-t high byte
            char _0h = ((last & 0xF000) >> 12);
            if (_0h <= 9) {
                begin[8] = _0h + '0';
            } else {
                begin[8] = _0h + 'a' - 10;
            }

            char _0l = ((last & 0x0F00) >> 8);
            if (_0l <= 9) {
                begin[9] = _0l + '0';
            } else {
                begin[9] = _0l + 'a' - 10;
            }


            //uint16-t low byte
            char _1h = ((last & 0xF0) >> 4);
            if (_1h <= 9) {
                begin[10] = _1h + '0';
            } else {
                begin[10] = _1h + 'a' - 10;
            }

            char _1l = (last & 0x0F);
            if (_1l <= 9) {
                begin[11] = _1l + '0';
            } else {
                begin[11] = _1l + 'a' - 10;
            }
        }

        encbuf_len = (12);
    }
}

inline void JSONTokener::EncodeUnicodeNumber(wcpp::uint32 codepoint, wcpp::DataStream& stream) {
    //Ensure there is enough memory to hold the data
    stream.Expand(12);

    char* begin = (char*)stream.GetCurrentWriteBuffer();

    // codepoint is in BMP
    if (codepoint < 0x10000) {
        begin[0] = '\\';
        begin[1] = 'u';

        //uint16-t high byte
        char _0h = ((codepoint & 0xF000) >> 12);
        if (_0h <= 9) {
            begin[2] = _0h + '0';
        } else {
            begin[2] = _0h + 'a' - 10;
        }

        char _0l = ((codepoint & 0x0F00) >> 8);
        if (_0l <= 9) {
            begin[3] = _0l + '0';
        } else {
            begin[3] = _0l + 'a' - 10;
        }


        //uint16-t low byte
        char _1h = ((codepoint & 0xF0) >> 4);
        if (_1h <= 9) {
            begin[4] = _1h + '0';
        } else {
            begin[4] = _1h + 'a' - 10;
        }

        char _1l = (codepoint & 0x0F);
        if (_1l <= 9) {
            begin[5] = _1l + '0';
        } else {
            begin[5] = _1l + 'a' - 10;
        }

        stream.seekp(6);
    } else { // not in BMP -> construct a UTF-16 surrogate pair
        codepoint -= 0x10000;
        int32_t first = 0xD800 | ((codepoint & 0xffc00) >> 10);
        int32_t last = 0xDC00 | (codepoint & 0x003ff);

        //Write First
        {

            begin[0] = '\\';
            begin[1] = 'u';

            //uint16-t high byte
            char _0h = ((first & 0xF000) >> 12);
            if (_0h <= 9) {
                begin[2] = _0h + '0';
            } else {
                begin[2] = _0h + 'a' - 10;
            }

            char _0l = ((first & 0x0F00) >> 8);
            if (_0l <= 9) {
                begin[3] = _0l + '0';
            } else {
                begin[3] = _0l + 'a' - 10;
            }


            //uint16-t low byte
            char _1h = ((first & 0xF0) >> 4);
            if (_1h <= 9) {
                begin[4] = _1h + '0';
            } else {
                begin[4] = _1h + 'a' - 10;
            }

            char _1l = (first & 0x0F);
            if (_1l <= 9) {
                begin[5] = _1l + '0';
            } else {
                begin[5] = _1l + 'a' - 10;
            }
        }


        //Write last
        {
            begin[6] = '\\';
            begin[7] = 'u';

            //uint16-t high byte
            char _0h = ((last & 0xF000) >> 12);
            if (_0h <= 9) {
                begin[8] = _0h + '0';
            } else {
                begin[8] = _0h + 'a' - 10;
            }

            char _0l = ((last & 0x0F00) >> 8);
            if (_0l <= 9) {
                begin[9] = _0l + '0';
            } else {
                begin[9] = _0l + 'a' - 10;
            }


            //uint16-t low byte
            char _1h = ((last & 0xF0) >> 4);
            if (_1h <= 9) {
                begin[10] = _1h + '0';
            } else {
                begin[10] = _1h + 'a' - 10;
            }

            char _1l = (last & 0x0F);
            if (_1l <= 9) {
                begin[11] = _1l + '0';
            } else {
                begin[11] = _1l + 'a' - 10;
            }
        }

        stream.seekp(12);
    }
}
}
}

