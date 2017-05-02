#pragma once

#include "simcc/inner_pre.h"
#include "simcc/slice.h"

namespace simcc {

// A Tokener takes a source string and extracts characters and tokens from it.
// It is used to help to parse strings.

class Tokener {
public:
    Tokener() {
        Reset("", 0);
    }

    Tokener(const string& s);

    Tokener(const char* ps, const int32 ps_len = -1);

    // Calling this method is similar to reconstruct a new Tokener
    void Reset(const char* ps, const int32 ps_len = -1);

    ~Tokener();

    // Back up one character. This provides a sort of lookahead capability,
    // so that you can test for a digit or letter before attempting to parse
    // the next number or identifier.
    bool Back();

    // Back up several characters.
    // @param backstep - The count of back up steps
    bool Back(int backstep);

    // Get the next character in the source string.
    // This function will make the current position flag move to next position
    // @return The next character, or 0 if past the end of the source string.
    char Next();

    // Get the current character in the source string.
    //
    // @return The current character, or 0 if past the end of the source string.
    char Current();

    // Get the next char in the string, skipping whitespace.
    // @return  A character, or 0 if there are no more characters.
    char NextClean();

    // Get the characters up to the next character <code>quote</code>.
    // Backslash processing is done.
    // If <code>quote</code> is not found, we do nothing and just
    // return a string empty "".
    // @param quote The quoting character, either
    //      <code>"</code>(double quote) or
    //      <code>'</code>(single quote) or
    //      other character
    // @return the string
    string NextString(char quote);
    Slice NextSlice(char quote);

    // Get the characters up to the next white space( or tab, \n, \r, \0 ) character.
    //
    // @return the string
    string NextString();

    Slice NextSlice();

    // Skip characters until the next character is the requested character.
    // If the requested character is not found, no characters are skipped.
    // @param to A character to skip to.
    // @return The requested character, or zero if the requested character
    // is not found.
    char SkipTo(char to);

    // Skip characters until come to a new line.
    // @remark If there is no new line, this function will do nothing.
    // @return true if successfully or false when failed.
    bool SkipToNextLine();

    // Skip characters until the previous character is the requested character.
    // If the requested character is not found, no characters are skipped.
    // @param to A character to skip to.
    // @return The requested character, or zero if the requested character
    // is not found.
    char SkipBackTo(char to);

    // Skip all whitespace, tab, \n
    // @return false if arrived end of string.
    bool SkipSpaces();

    // Get the hex value of a character (base16).
    // @param c A character between '0' and '9' or between 'A' and 'F' or
    // between 'a' and 'f'.
    // @return  An int between 0 and 15, or -1 if c was not a hex digit.
    static int DehexChar(char c);

    // Gets current read position in the buffer. It also serve as length of buffer parsed.
    uint32 GetCurrentPosition() const {
        return current_ - data_;
    }

    // Gets current read position in the buffer.
    const char* GetCurrent() const {
        return current_;
    }

    // Query whether it is the end of the string.
    bool IsEnd() const {
        return (current_ >= end_ || *current_ == '\0');
    }

    // Gets size of data that can read from the current read position. 
    int32 GetReadableSize() const {
        return (int32)(end_ - current_);
    }

    const char* data() const {
        return data_;
    }

    size_t size() const {
        return end_ - data_;
    }

protected:
    void SetCurrent(size_t pos) {
        current_ = data_ + pos;
    }

    void SetCurrent(const char* pos) {
        current_ = pos;
    }

private:
    const char* data_; // the source text string to be parsed
    const char* current_; // At first, set it the very beginning of data_
    const char* end_; // Not include the byte which is pointed by end_
};

inline Tokener::Tokener(const string& s) {
    Reset(s.c_str(), s.length());
}

inline Tokener::Tokener(const char* ps, const int32 len) {
    Reset(ps, len);
}

inline void Tokener::Reset(const char* ps, const int32 len) {
    end_ = len + ps;
    if (len < 0 && ps) {
        end_ = ps + strlen(ps);
    }

    data_   = const_cast<const char*>(ps);
    current_ = data_;
}

inline Tokener::~Tokener() {
}

inline bool Tokener::Back() {
    if (current_ <= data_) {
        fprintf(stderr, "Stepping back two steps is not supported\n");
        return false;
    }

    --current_;
    return true;
}

inline bool Tokener::Back(int backstep) {
    if (current_ - backstep < data_) {
        fprintf(stderr, "Stepping back two steps is not supported\n");
        return false;
    }

    current_ -= backstep;
    return true;
}

inline int Tokener::DehexChar(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }

    if (c >= 'A' && c <= 'F') {
        return c - ('A' - 10);
    }

    if (c >= 'a' && c <= 'f') {
        return c - ('a' - 10);
    }

    return -1;
}

inline char Tokener::Next() {
    if (IsEnd()) {
        // End of string
        ++current_;
        return 0;
    }

    return *current_++;
}

inline char Tokener::Current() {
    if (IsEnd()) {
        return 0;
    }

    return *current_;
}

inline char Tokener::NextClean() {
    for (char c = 0; current_ < end_;) {
        c = *current_++;

        /** c < 0 means This is not an ASCII character, like GBK code */
        if (c > ' ' || c == 0 || c < 0) {
            return c;
        }
    }

    return 0;
}

inline string Tokener::NextString() {
    return NextSlice().ToString();
}

inline Slice Tokener::NextSlice() {
    if (IsEnd()) {
        return "";
    }

    const char* startpos = current_;

    for (unsigned char c = 0; current_ < end_;) {
        c = *current_++;

        if (c <= ' ') {
            return Slice(startpos, current_ - startpos - 1);
        }

        if (IsEnd()) {
            return Slice(startpos, end_ - startpos);
        }
    }

    assert(false && "Logic ERROR. The routine SHOULD NOT come there.");
    return Slice("", 0);
}

inline string Tokener::NextString(char quote) {
    return NextSlice(quote).ToString();
}

inline Slice Tokener::NextSlice(char quote) {
    const char* startpos = current_;

    while (*current_++ != quote) {
        if (IsEnd()) {
            current_ = startpos;
            return Slice("", 0);
        }
    }

    assert(current_ > startpos);
    return Slice(startpos, current_ - startpos - 1);
}

inline char Tokener::SkipTo(char to) {
    register char c = 0;
    const char* startIndex = this->current_;

    do {
        c = Next();

        if (c == 0) {
            current_ = startIndex;
            return c;
        }
    } while (c != to);

    Back();
    return c;
}

inline char Tokener::SkipBackTo(char to) {

    const char* startIndex = this->current_;

    do {
        if (!Back()) {
            current_ = startIndex;
            return 0;
        }
    } while (Current() != to);
    char c = Current();
    Next();
    return c;
}

inline bool Tokener::SkipSpaces() {
    for (char c = 0; current_ < end_;) {
        c = *current_++;

        if (c > ' ' || c <= 0) {
            Back();
            return true;
        } else {
            continue;
        }
    }

    return false;
}


inline bool Tokener::SkipToNextLine() {
    /**
     *  Windows line ending char is 0x0D0A
     *  Linux/Unix line ending char is 0x0A
     *  So, we just SkipTo(0x0A)
     */

    char c = SkipTo((char)0x0a);
    if (c == 0) {
        return false;
    }

    // skip the last char of this line, and then go to a new line
    Next();
    return true;
}


}



