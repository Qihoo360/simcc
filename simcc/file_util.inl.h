#pragma once

#ifdef H_OS_WINDOWS
#include <direct.h>
#include <io.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#endif

namespace simcc {

inline bool FileUtil::IsValidDirName(const char* szName) {
    return !(szName [0] == ('.') ||
             (szName [1] != 0 && (szName[1] == '.' && szName[2] == 0)));
}

// Query whether strFileName is a absolute path.
inline bool FileUtil::IsAbsolutePathName(const string& path) {
    if (path.size() == 0) {
        return false;
    }

#ifdef H_OS_WINDOWS
    if (isalpha(static_cast<unsigned char>(path[0])) && path[1] == ':') {
        return true;
    } else {
        return false;
    }

#else
    return path[0] == '/';
#endif

}

inline string FileUtil::GetAbsolutePathName(const string& strPathName) {
    string strTmp = strPathName;

    if (!IsAbsolutePathName(strTmp)) {
        //
        char szDir[_MAX_PATH];
#ifdef H_OS_WINDOWS
        if (_getcwd(szDir, _MAX_PATH)) {
            strTmp =  string(szDir) + ("/") + strTmp;
        }
#else
        getcwd(szDir, _MAX_PATH);
        strTmp =  string(szDir) + ("/") + strTmp;
#endif

    }

    // convert file name here.
    NormalizePath(strTmp);

    return strTmp;
}

inline string FileUtil::GetFileName(const string& strFileName) {
    // since '/' equal to L'/', we can cast to either one.
    string::size_type i = strFileName.find_last_of((string::value_type)'/');

    if (i == string::npos) {
        return strFileName;
    }

    return strFileName.substr(i + 1);
}

inline string FileUtil::GetFileNameWithoutExt(const string& strFileName) {
    // since '/' equal to L'/', we can cast to either one.
    string::size_type iStart = strFileName.find_last_of((string::value_type)'/');

    if (iStart == string::npos) {
        iStart = 0;
    } else {
        // skip the '/'
        ++iStart;
    }

    return strFileName.substr(iStart, strFileName.find_last_of((string::value_type)'.') - iStart);
}
inline string FileUtil::GetFileNameExtension(const string& strFileName) {
    string::size_type endPos = strFileName.find_last_of((string::value_type)'.');

    if (endPos < strFileName.length() - 1) {
        return strFileName.substr(endPos + 1);
    }

    return string();
}
inline string FileUtil::GetFileDirName(const string& strFileName) {
    // since '/' equal to L'/', we can cast to either one.
    string::size_type i = strFileName.find_last_of((string::value_type)'/');

    if (i == string::npos) {
        return string();
    }

    return strFileName.substr(0, i);
}

inline bool FileUtil::IsDir(const char* szFileName) {
    if (!szFileName) {
        return false;
    }

    const string strFileName(szFileName);

    return IsDir(strFileName);
}

inline bool FileUtil::IsDir(const string& strFileName) {
    //for the 'stat' API, the parameter strFileName SHOULD not contain a trailing backslash
    string strStandardisePath = NormalizePath(strFileName, false);

    struct stat st;
    if (0 != stat(strStandardisePath.c_str(), &st)) {
        return false;
    }

#ifdef H_OS_WINDOWS
    if ((st.st_mode & _S_IFDIR)  == _S_IFDIR)
#else
    if (S_ISDIR(st.st_mode))
#endif
    {
        return true;
    }

    return false;
}

inline bool FileUtil::RemoveDirIfEmpty(const string& strDirName, bool bRecursivelyDeleteEmptyParentDir /*= true*/) {
    if (!IsDir(strDirName)) {
        return true;
    }

    bool retval = true;

    std::list<string> files;
    GetFiles(strDirName, files, true);

    if (files.size() == 0) {
        if (FileUtil::Rmdir(strDirName.c_str()) != 0) {
            retval = false;
        }
    }

    if (bRecursivelyDeleteEmptyParentDir) {
        string strParentDir = getParentDirName(strDirName);

        if (!RemoveDirIfEmpty(strParentDir, bRecursivelyDeleteEmptyParentDir)) {
            retval = false;
        }
    }

    return retval;
}

inline string FileUtil::getParentDirName(const string& strFileName, bool with_trailing_slash /*= true*/) {
    string path = NormalizePath(strFileName, false);

    if (!IsAbsolutePathName(strFileName)) {
        path = GetAbsolutePathName(path);
    }

    size_t pos = path.find_last_of('/');

    if (pos == 0 || pos == string::npos) {
        if (with_trailing_slash) {
            path += "/";
        }

        return path;
    } else {
        if (with_trailing_slash) {
            string retp = string(path.c_str(), pos) + "/";
            return retp;
        } else {
            return string(path.c_str(), pos);
        }
    }
}

inline bool FileUtil::Rmdir(const string& strDirName) {
    return Rmdir(strDirName.c_str());
}

inline bool FileUtil::Rmdir(const char* strDirName) {
#ifdef H_OS_WINDOWS
    if (::_rmdir(strDirName) == 0)
#else
    if (::rmdir(strDirName) == 0)
#endif
    {
        return true;
    }


    return false;
}

}


