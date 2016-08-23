#pragma once

#include "wcpp/inner_pre.h"

#include <list>
#include <string>

#ifdef H_OS_WINDOWS
    #include <direct.h>
#endif

namespace wcpp {

// File access utility. Provide many help methods to make it easy to do some file operation.
// @remark The instance is created when the module is initialized.
class WCPP_EXPORT FileUtil {
public:
    // Query whether the file name is a valid directory. e.g. ".", ".." are invalid
    // directory name.
    static bool IsValidDirName(const char* szName);

    // Query whether strFileName is a absolute path.
    static bool IsAbsolutePathName(const string& strPathName);

    // Gets absolute path name.
    // @param strPathName If already is a absolute path name return it, else build
    // absolute path name with current path.
    static string GetAbsolutePathName(const string& strPathName);

    // Clean returns the shortest path name equivalent to path
    // by purely lexical processing.  It applies the following rules
    // iteratively until no further processing can be done:
    //
    //	1. Replace multiple Separator elements with a single one.
    //	2. Eliminate each . path name element (the current directory).
    //	3. Eliminate each inner .. path name element (the parent directory)
    //	   along with the non-.. element that precedes it.
    //	4. Eliminate .. elements that begin a rooted path:
    //	   that is, replace "/.." by "/" at the beginning of a path,
    //	   assuming Separator is '/'.
    //
    // The returned path ends in a slash only if it represents a root directory,
    // such as "/" on Unix or `C:\` on Windows.
    //
    // If the result of this process is an empty string, Clean
    // returns the string ".".
    //
    // See also Rob Pike, ``Lexical File Names in Plan 9 or
    // Getting Dot-Dot Right,''
    // http://plan9.bell-labs.com/sys/doc/lexnames.html
    static string Clean(const string& path);

    // @brief splitting a fully qualified filename into file name and base path dir.
    // @param qualifiedName Path is standardized as in <code>NormalizePath</code>
    // @param strFileName[out]
    // @param strBasePathDir[out]
    // @param with_trailing_slash[in] true, End of the string With a SLASH "/".
    static void SplitFileName(const std::wstring& qualifiedName,
                              std::wstring& strFileName, std::wstring& strBasePathDir, bool with_trailing_slash = false);
    static void SplitFileName(const string& qualifiedName,
                              string& strFileName, string& strBasePathDir, bool with_trailing_slash = false);



    // Concatenate tow path into one, and return it.
    static string Join(const string& prefix, const string& postfix);
    static std::wstring Join(const std::wstring& prefix, const std::wstring& postfix);


    // @brief Method for standardizing paths - use forward slashes "/" only
    // @param strOrignal
    // @param with_trailing_slash
    //         true, End of the string With a SLASH "/".
    //         false, without a SLASH "/" at the end of the string
    // @return std::string
    static string NormalizePath(const string& strOrignal, bool with_trailing_slash = true);
    static std::wstring NormalizePath(const std::wstring& strOrignal, bool with_trailing_slash = true);

    // @brief Gets path's parent directory name. The path can be a normal file or a directory
    // e.g. "D:\test\aab.jpg" ==> "D:/test/" or "D:/test"
    // e.g. "D:/test/abc/" ==> "D:/test/" or "D:/test"
    // @warning The return parent director name is suitable for <code>NormalizePath</code>
    //         That means it uses forward slash "/" as path separator
    // @note It take care of relative path.
    // @param with_trailing_slash
    //         true, End of the string With a SLASH "/".
    //         false, without a SLASH "/" at the end of the string
    // @return string,
    static string getParentDirName(const string& strFileName, bool with_trailing_slash = true);

    // Gets file name without path..
    // e.g. "D:/test/aab.jpg" ==> "aab.jpg"
    static string GetFileName(const string& strFileName);

    // Gets base file name. the file extension will be removed.
    // e.g. "D:/test/aab.jpg" ==> "aab"
    static string GetFileNameWithoutExt(const string& strFileName);

    // Gets file name extension. If can not find the last '.', null string
    // will be returned. e.g. "D:/test/aab.jpg" ==> "jpg"
    //
    static string GetFileNameExtension(const string& strFileName);

    // Gets file path name without end '/'.
    // e.g. "D:/test/aab.jpg" ==> "D:/test"
    static string GetFileDirName(const string& strFileName);


    // Quickly remove file or directory from local file system.
    // @param strFileName if end with '/', it is a directory, all the children will be
    // remove, else is a file and the file will be removed.
    // @return bool True if successfully.
    static bool Unlink(const string& strFileName) ;
    static bool Remove(const string& strFileName) {
        return Unlink(strFileName);
    }


    // @brief Remove the empty directory. If it is not empty or it is not directory, we do nothing and just return true.
    // @remark Empty directory definition: it doesn't has any normal file.
    // e.g. /home/a_dir/b_dir/c_dir
    //     Condition: c_dir has no files in it, and b_dir only has c_dir and a_dir only has b_dir
    //     If we call RemoveDirIfEmpty( "/home/a_dir/b_dir", false ), them b_dir will be deleted and its child c_dir deleted
    //     If we call RemoveDirIfEmpty( "/home/a_dir/b_dir", true ), them a_dir, b_dir, c_dir will all be deleted.
    // @param bRecurcively True, we will delete its parent directory if its parent directory is also empty
    // @return bool True if successfully.
    static bool RemoveDirIfEmpty(const string& strDirName, bool bRecursivelyDeleteEmptyParentDir = true);

    // Make directory hierarchy in local file system.
    // Its behavior is similar with the command 'mkdir -p'
    // @param strDirPath Absolute directory file name. e.g. '/home/weizili/test'
    // @return true if successfully
    static bool Mkdir(const string& strDirPath);

    static bool Rmdir(const string& strDirName);
    static bool Rmdir(const char* strDirName);


    // Determines if a file exists and could be opened.
    // @note The file CAN be a directory
    // @param filename is the string identifying the file which should be tested for existence.
    // @return Returns true if file exists, and false if it does not exist or an error occurred.
    static bool IsFileExist(const std::string& strFileName);

    static bool IsReadable(const std::string& strFileName);
    static bool IsWriteable(const std::string& strFileName);
    static bool IsExecutable(const std::string& strFileName);

    // @brief copy a file to another place. This function has the same feature like DOS Command 'copy' or Linux Command 'cp'
    // @warning If the destination file is exist, it will be replaced.
    // @warning strDestFile MUST BE a file, NOT a directory
    // @param strSrcFile The source file
    // @param strDestFile The target file
    // @param bOverride True, this will override the old existent file.
    // @return bool True if successfully
    static bool CopyFile(const std::string& strSrcFile, const std::string& strDestFile, bool bOverride = true) ;

    // @brief Query whether the given path is a directory.
    static bool IsDir(const char* strFileName);
    static bool IsDir(const std::string& strFileName);

    // @brief Walk the directory and get a list of files, excluded directory.
    // @param strDirName The directory name
    // @param files[out] The list of files are stored here. The file name is with the full path name(include the directory name)
    // @param recursively Whether walk the subdirectories.
    // @param filter Pattern to match against; which can include simple '*' wildcards
    static void GetFiles(const std::string& strDirName, std::list<std::string>& files, bool recursively = true, const std::string& filter = "*");

    // @brief Walk the directory and get a list of files, not include directory.
    // @param strDirName The directory name
    // @param files[out] The list of files are stored here. The file name is with the full path name(include the directory name)
    // @param depth The depth to walk the subdirectories. 0 means only walk the top dir strDirName
    // @param filter Pattern to match against; which can include simple '*' wildcards
    static void GetFiles(const std::string& strDirName, std::list<std::string>& files, int depth, const std::string& filter = "*");


    // Extract strings from the a file line-by-line,
    // every line content as a list element will be inserted to a list.
    static bool ReadFile(const char* szFileName, std::list<std::string>& lines);

    // @brief Write the data into a file.
    //  The file is created if it does not exist, otherwise it is truncated
    // @param const char* filepath - The file path where the data is stored
    // @param const void* content - The file content
    // @param const size_t len - The length of the content
    // @return true if successfully
    static bool WriteFile(const char* filepath, const void* content, const size_t len);
    static bool WriteFile(const std::string& filepath, const void* content, const size_t len);
};

}

#include "wcpp/file_util.inl.h"
