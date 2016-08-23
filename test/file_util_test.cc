
#include "test_common.h"
#include "wcpp/exp.h"
#include "wcpp/data_stream.h"
#include "wcpp/file_util.h"

#include <iostream>

using namespace wcpp;

namespace {
void is_directory_1() {
#ifdef H_OS_WINDOWS
    const char*  szPath = "c:\\windows";
    std::string strPath = "C:\\windows\\system32";
#else
    const char*  szPath = "/usr/local";
    std::string strPath = "/usr/local";
#endif

    H_TEST_ASSERT(FileUtil::IsDir(szPath));
    H_TEST_ASSERT(FileUtil::IsDir(strPath));
}

void is_directory_2() {
#ifdef H_OS_WINDOWS
    const char*  szPath = "c:\\windows\\system.ini";
    std::string strPath = "C:\\Windows\\System32\\drivers\\etc\\HOSTS";
#else
    const char*  szPath = "/etc/passwd";
    std::string strPath = "/etc/passwd";
#endif
    H_TEST_ASSERT(FileUtil::IsDir(szPath) == false);
    H_TEST_ASSERT(FileUtil::IsDir(strPath) == false);
}

void is_directory_3() {
#if _MSC_VER >= 1800

    int result = 0;
    struct _stat statbuf;
    char* filename = "C:/Windows/System32/drivers";

    // Get data associated with "crt_stat.c":
    result = _stat(filename, &statbuf);

    // Check if statistics are valid:
    if (result != 0) {
        perror("Problem getting information");
        switch (errno) {
        case ENOENT:
            printf("File %s not found.\n", filename);
            break;
        case EINVAL:
            printf("Invalid parameter to _stat.\n");
            break;
        default:
            /* Should never be reached. */
            printf("Unexpected error in _stat.\n");
        }
    } else {
        H_TEST_ASSERT((statbuf.st_mode & _S_IFDIR) == _S_IFDIR);
    }
#endif // 

}

}

TEST_UNIT(FileUtil_test) {
    is_directory_3();
    is_directory_1();
    is_directory_2();
}

TEST_UNIT(FileUtil_test_1) {
#ifdef H_OS_WINDOWS
    const char* path = "c:/windows";
#else
    const char* path = "/etc";
#endif
    struct stat st;
    int r = stat(path, &st);
    H_TEST_ASSERT(r == 0);
}

