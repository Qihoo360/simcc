#include "simcc/inner_pre.h"

#include "simcc/data_stream.h"

#include <algorithm>

#ifdef H_OS_WINDOWS
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif


namespace simcc {

void DataStream::ToText() {
    Write('\0');
    seekp(-1);
}

bool DataStream::ReadFile(/*const string& strFileName*/ const string& strPathName) {
// #ifdef H_OS_WINDOWS
//      string strPathName = StringUtil::utf8ToMbs(strFileName);
//         std::replace( strPathName.begin(), strPathName.end(), '\\', '/' );
// #else
//         string strPathName = strFileName;
// #endif
    // clear old data.
    if (self_created_ && buffer_) {
        free(buffer_);
        buffer_ = NULL;
        capacity_ = 0;
        self_created_ = true;
    }

    write_index_ = 0;
    read_index_  = 0;

    if (strPathName.size() < 1) {
        return false;
    }

#ifdef H_OS_WINDOWS
    const char* mode = "rb";
#else
    const char* mode = "r";
#endif

    FILE* pf = fopen(strPathName.c_str(), mode);
    struct stat st;
    if (!pf || 0 != stat(strPathName.c_str(), &st)) {
        if (pf) {
            fclose(pf);
        }

        return false;
    }

    // allocate memory.
    buffer_ = (uint8*)malloc(st.st_size);

    if (!buffer_) { //st.st_size may be 0
        fclose(pf);
        return false;
    }

    self_created_ = true;
    capacity_ = st.st_size;

    size_t remain = capacity_;

    while (remain > 0 && !feof(pf)) {

        size_t readn = fread(buffer_ + (capacity_ - remain) ,
                             sizeof(char), remain, pf);

        remain -= readn;

        if (ferror(pf)) {
            if (pf) {
                fclose(pf);
            }

            return false;
        }
    }

    fclose(pf);

    assert(remain == 0);
    seekp((int32)capacity_);

    return true;
}

static void createDir(const string& strFileName) {
    string strPathName = strFileName;

    std::replace(strPathName.begin(), strPathName.end(), '\\', '/');

    size_t nCurSplit = 0, nNextSplit = 0;

    do {
        //  "/abc/ab/abc",   "e:/abac/adc"   "abcc/abc/a",   "abc"
        // get current dir name.
        nCurSplit = strPathName.find_first_of((string::value_type)'/', nNextSplit);

        if (nCurSplit != 0 && nCurSplit != string::npos) {
            // current dir
#ifdef H_OS_WINDOWS
            _mkdir(strPathName.substr(0, nCurSplit).c_str());
#else
            mkdir(strPathName.substr(0, nCurSplit).c_str(), 0777);
#endif
        }

        nNextSplit = nCurSplit + 1;

    } while (nCurSplit != string::npos);
}

bool DataStream::WriteFile(const string& filepath) {
    // create directory.
    createDir(filepath);

#ifdef H_OS_WINDOWS
    const char* mode = "wb+";
#else
    const char* mode = "w+";
#endif

    // open file.
    FILE* fp = fopen(filepath.c_str(), (mode));
    if (!fp) {
        return false;
    }

    size_t writen = fwrite(buffer_, 1, write_index_, fp);
    fclose(fp);

    if (writen < (uint32)write_index_) {
        return false;
    }

    return true;
}

bool DataStream::IsContentEquals(const DataStream& first, const DataStream& second) {
    size_t sz = first.size();
    if (sz != second.size()) {
        return false;
    }

    for (size_t i = 0; i < sz; i++) {
        if (first.CharAt(i) != second.CharAt(i)) {
            return false;
        }
    }

    return true;
}

}
