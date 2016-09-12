#include "simcc/file_util.h"
#include "simcc/data_stream.h"
#include "simcc/string_util.h"

#include <fstream>
#include <sstream>

#ifndef H_OS_WINDOWS
#include <unistd.h>
#endif


namespace simcc {

string FileUtil::Clean(const string& path) {
    // Not Tested yet.

    std::vector<std::string> v;
    StringUtil::Split(v, path, "/\\", 0);

    std::ostringstream newpath;

    // in case of root dir.
    if (path[0] == '/') {
        newpath << (char)'/';
    }

    // erase null.
    std::list<std::string> vv;
    for (auto it = v.rbegin(), ite = v.rend(); it != ite; ++it) {
        if (*it == ".") {
            continue;
        } else if (*it == "..") {
            // erase none '.' parent dir.
            while (++it != ite
                   && *it == ".") {
                // do nothing.
            }

            // if first of the path is "../" , should reserve it.
            if (it == ite) {
                vv.push_front("..");
                break;
            }
        } else {
            vv.push_front(*it);
        }
    }

    // build string
    for (auto it = vv.begin(), ite = vv.end(); it != ite;) {
        newpath << *it;
        if (++it == ite) {
            break;
        }
        newpath << (char)'/';
    }

    return newpath.str();
}

string FileUtil::Join(const std::string& prefix, const std::string& postfix) {
    std::string pre = prefix;
    std::string post = postfix;
    pre = NormalizePath(pre, false);
    post = NormalizePath(post, false);
    StringUtil::Trim(post, "/", true, false);
    if (StringUtil::EndsWith(postfix, "/")) {
        post += "/";
    }
    return pre + "/" + post;
}

std::wstring FileUtil::Join(const std::wstring& prefix, const std::wstring& postfix) {
    typedef std::wstring MyString;
    MyString pre = prefix;
    MyString post = postfix;
    pre = NormalizePath(pre, false);
    post = NormalizePath(post, false);
    StringUtil::Trim(post, L"/", true, false);
    if (StringUtil::EndsWith(postfix, L"/")) {
        post += L"/";
    }
    return pre + L"/" + post;
}

string FileUtil::NormalizePath(const string& init, bool with_trailing_slash /*= true*/) {
    if (init.length() < 1) {
        return init;
    }

    string path = init;

    std::replace(path.begin(), path.end(), '\\', '/');

    if (path[path.length() - 1] != '/' && with_trailing_slash) {
        path += "/";
    } else if (path[path.length() - 1] == '/' && !with_trailing_slash) {
        StringUtil::Trim(path, "/", false, true);
    }

#ifdef H_OS_WINDOWS
    StringUtil::ToLower(path);
#endif

    return path;
}


std::wstring FileUtil::NormalizePath(const std::wstring& init, bool with_trailing_slash /*= true*/) {
    if (init.length() < 1) {
        return init;
    }

    std::wstring path = init;
    std::replace(path.begin(), path.end(), L'\\', L'/');
    if (path[path.length() - 1] != L'/' && with_trailing_slash) {
        path += L"/";
    } else if (path[path.length() - 1] == L'/' && !with_trailing_slash) {
        StringUtil::Trim(path, L"/", false, true);
    }

#ifdef H_OS_WINDOWS
    StringUtil::ToLower(path);
#endif
    return path;
}

void FileUtil::SplitFileName(const string& filepath,
                             string& base, string& dir_path, bool with_trailing_slash /*= false */) {
    string path = filepath;
    // Replace \ to /
    std::replace(path.begin(), path.end(), '\\', '/');
    // split based on final /
    size_t i = path.find_last_of('/');
    if (i == string::npos) {
        dir_path = "";
        base = filepath;
    } else {
        base = path.substr(i + 1, path.size() - i - 1);
        if (with_trailing_slash) {
            dir_path = path.substr(0, i + 1);
        } else {
            dir_path = path.substr(0, i);
        }
    }

#ifdef H_OS_WINDOWS
    StringUtil::ToLower(base);
    StringUtil::ToLower(dir_path);
#endif
}

void FileUtil::SplitFileName(const std::wstring& filepath,
                             std::wstring& base, std::wstring& dir_path, bool with_trailing_slash /*= false */) {
    std::wstring path = filepath;
    // Replace \ with / first
    std::replace(path.begin(), path.end(), L'\\', L'/');
    // split based on final /
    size_t i = path.find_last_of(L'/');

    if (i == std::wstring::npos) {
        dir_path = L"";
        base = filepath;
    } else {
        base = path.substr(i + 1, path.size() - i - 1);
        if (with_trailing_slash) {
            dir_path = path.substr(0, i + 1);
        } else {
            dir_path = path.substr(0, i);
        }
    }

#ifdef H_OS_WINDOWS
    StringUtil::ToLower(base);
    StringUtil::ToLower(dir_path);
#endif
}

bool FileUtil::Unlink(const string& filepath) {
    bool is_dir = FileUtil::IsDir(filepath);

    if (is_dir) {
        assert(0 && "not supported");
        return false;
    }

    // remove self
    if (is_dir) {
        return Rmdir(filepath.c_str()) == 0;
    } else {
#ifdef H_OS_WINDOWS
        return ::DeleteFileA(filepath.c_str()) ? true : false;
#else
        return remove(filepath.c_str()) == 0;
#endif
    }
}

bool FileUtil::Mkdir(const string& dirpath) {
    size_t nCurSplit = 0, nNextSplit = 0;

    int iret = 0;
    (void)iret;

    do {

        // get current dir name.
        nCurSplit = dirpath.find_first_of((string::value_type)'/', nNextSplit);
        nNextSplit = dirpath.find_first_of((string::value_type)'/', nCurSplit + 1);

#ifdef H_OS_WINDOWS
        // current dir
        iret = ::_mkdir(dirpath.substr(0, nNextSplit).c_str());
#else
        // current dir
        iret = ::mkdir(dirpath.substr(0, nNextSplit).c_str(), 0755);
#endif

        // next dir
        nCurSplit = nNextSplit + 1;

    } while (nNextSplit != string::npos);

    return true;
}

#ifdef H_OS_WINDOWS
// 00
// Existence only
//
// 02
// Write-only
//
// 04
// Read-only
//
// 06
// Read and write
#define F_OK  0
#define W_OK  2
#define R_OK  4
#define RW_OK 6
#define X_OK  1
#endif

bool FileUtil::IsFileExist(const std::string& strFileName) {
    if (access(strFileName.c_str(), F_OK) != 0) {
        return false;
    }
    return true;
}

bool FileUtil::IsReadable(const std::string& strFileNmae) {
    if (access(strFileNmae.c_str(), R_OK) != 0) {
        return false;
    }
    return true;
}

bool FileUtil::IsWriteable(const std::string& strFileNmae) {
    if (access(strFileNmae.c_str(), W_OK) != 0) {
        return false;
    }
    return true;
}

bool FileUtil::IsExecutable(const std::string& strFileNmae) {
    if (access(strFileNmae.c_str(), X_OK) != 0) {
        return false;
    }
    return true;
}

bool FileUtil::CopyFile(const std::string& strSrcFile, const std::string& strDestFile, bool bOverride/* = true */) {
    if (IsDir(strSrcFile)) {
        return false;
    }

    string strDestFilePath = strDestFile;

    if (IsDir(strDestFile)) {
        strDestFilePath = Join(strDestFile, GetFileName(strSrcFile));
    }

    if (IsFileExist(strDestFilePath) && !bOverride) {
        return false;
    }

    DataStream databuf;
    databuf.ReadFile(strSrcFile);
    databuf.WriteFile(strDestFile);
    return true;
}

bool FileUtil::ReadFile(const char* szFileName, std::list<std::string>& lines) {
    if (!szFileName) {
        return false;
    }

    std::ifstream ifs(szFileName);
    if (! ifs) {
        return false;
    }

    string line;

    while (!ifs.eof()) {
        getline(ifs, line);
        lines.push_back(line);
    }

    return true;
}

bool FileUtil::WriteFile(const string& filepath, const void* content, const size_t len) {
    return WriteFile(filepath.c_str(), content, len);
}

bool FileUtil::WriteFile(const char* filepath, const void* content, const size_t len) {
    FILE* fp = ::fopen(filepath, "w+");

    if (fp == NULL) {
        fprintf(stderr, "%s : could not open file \"%s\" for write\n", __func__, filepath);
        return false;
    }

    ::fwrite(content, sizeof(char), len, fp);
    ::fflush(fp);
    ::fclose(fp);

    return true;
}

void FileUtil::GetFiles(const string& strDirName, std::list<std::string>& files, bool recursively/* = true*/, const string& filter/* = "*.*"*/) {
#ifdef H_OS_WINDOWS
    struct _finddata_t finfo; //file info

    std::string p;
    intptr_t hFile = _findfirst(FileUtil::Join(strDirName, "*").c_str(), &finfo);
    if (-1 == hFile) {
        return;
    }

    do {
        if ((finfo.attrib & _A_SUBDIR)) {
            if (recursively && strcmp(finfo.name, ".") != 0 && strcmp(finfo.name, "..") != 0) {
                GetFiles(FileUtil::Join(strDirName, finfo.name), files, recursively, filter);
            }
        } else {
            string strFileName = FileUtil::Join(strDirName, finfo.name);
            if (StringUtil::Match(strFileName, filter)) {
                files.push_back(strFileName);
            }
        }
    } while (_findnext(hFile, &finfo) == 0);

    _findclose(hFile);

#else
    struct dirent** namelist;
    int n = scandir(strDirName.c_str(), &namelist, 0, alphasort);

    if (n < 0) {
        return;
    }

    for (int i = 0; i < n; ++i) {
        if (namelist[i]->d_type == DT_DIR) {
            if (recursively && strcmp(namelist[i]->d_name, ".") != 0 && strcmp(namelist[i]->d_name, "..") != 0) {
                GetFiles(FileUtil::Join(strDirName, namelist[i]->d_name), files, recursively, filter);
            }
        } else {
            string strFileName = FileUtil::Join(strDirName, namelist[i]->d_name);

            if (StringUtil::Match(strFileName, filter)) {
                files.push_back(strFileName);
            }
        }
        free(namelist[i]);
    }
    free(namelist);
#endif
}

void FileUtil::GetFiles(const string& strDirName, std::list<std::string>& files, int depth /*= 0*/, const string& filter/* = "*"*/) {
    if (depth < 0) {
        return;
    }

#ifdef H_OS_WINDOWS
    struct _finddata_t finfo; //file info

    std::string p;

    //if ( ( hFile = _findfirst( p.assign( strDirName ).append( "/*" ).append( "*" ).c_str(), &fileinfo ) ) != -1 )
    intptr_t hFile = _findfirst(FileUtil::Join(strDirName, "*").c_str(), &finfo);
    if (-1 == hFile) {
        return;
    }

    do {
        string strFileName = FileUtil::Join(strDirName, finfo.name);
        if ((finfo.attrib & _A_SUBDIR)) {
            if (strcmp(finfo.name, ".") != 0 && strcmp(finfo.name, "..") != 0) {
                if (StringUtil::Match(strFileName, filter)) {
                    files.push_back(strFileName);
                }

                if (depth > 0) {
                    GetFiles(strFileName, files, depth - 1, filter);
                }
            }
        } else {
            if (StringUtil::Match(strFileName, filter)) {
                files.push_back(strFileName);
            }
        }
    } while (_findnext(hFile, &finfo) == 0);

    _findclose(hFile);

#else
    struct dirent** namelist = NULL;
    int n = scandir(strDirName.c_str(), &namelist, 0, alphasort);

    if (n < 0) {
        std::ostringstream log;
        log << "scandir failed. return value = -1, errno=" << errno;
        //H_LOG_NAME_DEBUG( "FileUtil", log.str() );
        return;
    }

    for (int i = 0; i < n; ++i) {
        string strFileName = FileUtil::Join(strDirName, namelist[i]->d_name);
        if (namelist[i]->d_type == DT_DIR) {
            if (strcmp(namelist[i]->d_name, ".") != 0 && strcmp(namelist[i]->d_name, "..") != 0) {
                if (StringUtil::Match(strFileName, filter)) {
                    files.push_back(strFileName);
                }

                if (depth > 0) {
                    GetFiles(strFileName, files, depth - 1, filter);
                }
            }
        } else {
            if (StringUtil::Match(strFileName, filter)) {
                files.push_back(strFileName);
            }
        }
        free(namelist[i]);
    }
    free(namelist);
#endif
}

}
