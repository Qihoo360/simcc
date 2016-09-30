#pragma once

#include "simcc/inner_pre.h"

#include <list>
#include <string>

#ifdef H_OS_WINDOWS
    #include <direct.h>
#endif

namespace simcc {

// File access utility class which provides many help methods 
// to make it easy to do operations against file.
class SIMCC_EXPORT FileUtil {
public:
    // Query whether the file name is a valid directory. e.g. ".", ".." are invalid
    // directory name.
    static bool IsValidDirName(const char* name);

    // Query whether strFileName is a absolute path.
    static bool IsAbsolutePath(const string& path);

    // Gets absolute path name.
    // @param path If it is already a absolute path, we return it or else we build
    // a absolute path name with current path.
    static string GetAbsolutePathName(const string& path);

    // This method is copied from Golang project
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
    // @param file_name[out]
    // @param base_dir[out]
    // @param with_trailing_slash[in] true, End of the string With a SLASH "/".
    static void SplitFileName(const wstring& filepath,
                              wstring& file_name, wstring& base_dir, bool with_trailing_slash = false);
    static void SplitFileName(const string& filepath,
                              string& file_name, string& base_dir, bool with_trailing_slash = false);

    // Concatenate tow path into one, and return it.
    static string Join(const string& prefix, const string& postfix);
    static wstring Join(const wstring& prefix, const wstring& postfix);

    // @brief Method for standardizing paths - use forward slashes "/" only
    // @param filepath
    // @param with_trailing_slash
    //         true, End of the string With a SLASH "/".
    //         false, without a SLASH "/" at the end of the string
    // @return string
    static string NormalizePath(const string& filepath, bool with_trailing_slash = true);
    static wstring NormalizePath(const wstring& filepath, bool with_trailing_slash = true);

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
    static string GetParentDir(const string& filepath, bool with_trailing_slash = true);

    // Gets file name without path..
    // e.g. "D:/test/aab.jpg" ==> "aab.jpg"
    static string GetFileName(const string& filepath);

    // Gets base file name. the file extension will be removed.
    // e.g. "D:/test/aab.jpg" ==> "aab"
    static string GetFileNameWithoutExt(const string& filepath);

    // Gets file name extension. If can not find the last '.', null string
    // will be returned. e.g. "D:/test/aab.jpg" ==> "jpg"
    static string GetFileNameExtension(const string& filepath);

    // Gets file path name without end '/'.
    // e.g. "D:/test/aab.jpg" ==> "D:/test"
    static string GetFileDirName(const string& filepath);

    // Quickly remove file or directory from local file system.
    // @param strFileName if end with '/', it is a directory, all the children will be
    // remove, else is a file and the file will be removed.
    // @return bool True if successfully.
    static bool Unlink(const string& filepath) ;
    static bool Remove(const string& filepath) {
        return Unlink(filepath);
    }

    // @brief Remove the empty directory. If it is not empty or it is not directory, we do nothing and just return true.
    // @remark Empty directory definition: it doesn't has any normal file.
    // e.g. /home/a_dir/b_dir/c_dir
    //     Condition: c_dir has no files in it, and b_dir only has c_dir and a_dir only has b_dir
    //     If we call RemoveDirIfEmpty( "/home/a_dir/b_dir", false ), them b_dir will be deleted and its child c_dir deleted
    //     If we call RemoveDirIfEmpty( "/home/a_dir/b_dir", true ), them a_dir, b_dir, c_dir will all be deleted.
    // @param recursively_delete_empty_parent_dir True, we will delete its parent directory if its parent directory is also empty
    // @return bool True if successfully.
    static bool RemoveDirIfEmpty(const string& dir, bool recursively_delete_empty_parent_dir = true);

    // Make directory hierarchy in local file system.
    // Its behavior is similar with the command 'mkdir -p'
    // @param dir Absolute directory file name. e.g. '/home/weizili/test'
    // @return true if successfully
    static bool Mkdir(const string& dir);

    static bool Rmdir(const string& dir);
    static bool Rmdir(const char* dir);

    // Determines if a file exists and could be opened.
    // @note The file CAN be a directory
    // @param filename is the string identifying the file which should be tested for existence.
    // @return Returns true if file exists, and false if it does not exist or an error occurred.
    static bool IsFileExist(const string& filepath);

    static bool IsReadable(const string& filepath);
    static bool IsWriteable(const string& filepath);
    static bool IsExecutable(const string& filepath);

    // @brief copy a file to another place. This function has the same feature like DOS Command 'copy' or Linux Command 'cp'
    // @warning If the destination file is exist, it will be replaced.
    // @warning dest_file MUST BE a file, NOT a directory
    // @param src_file The source file
    // @param src_file The target file
    // @param override True, this will override the old existent file.
    // @return bool True if successfully
    static bool CopyFile(const string& src_file, const string& dest_file, bool override = true) ;

    // @brief Query whether the given path is a directory.
    static bool IsDir(const char* filepath);
    static bool IsDir(const string& filepath);

    // @brief Walk the directory and get a list of files, excluded directory.
    // @param dir The directory path
    // @param files[out] The list of files are stored here. The file name is with the full path name(include the directory name)
    // @param recursively Whether walk the subdirectories.
    // @param filter Pattern to match against; which can include simple '*' wildcards
    static void GetFiles(const string& dir, std::list<string>& files, bool recursively = true, const string& filter = "*");

    // @brief Walk the directory and get a list of files, not include directory.
    // @param dir The directory path
    // @param files[out] The list of files are stored here. The file name is with the full path name(include the directory name)
    // @param depth The depth to walk the subdirectories. 0 means only walk the top dir strDirName
    // @param filter Pattern to match against; which can include simple '*' wildcards
    static void GetFiles(const string& dir, std::list<string>& files, int depth, const string& filter = "*");

    // Extract strings from the a file line-by-line,
    // every line content as a list element will be inserted to a list.
    static bool ReadFile(const char* filepath, std::list<string>& lines);

    // @brief Write the data into a file.
    //  The file is created if it does not exist, otherwise it is truncated
    // @param const char* filepath - The file path where the data is stored
    // @param const void* content - The file content
    // @param const size_t len - The length of the content
    // @return true if successfully
    static bool WriteFile(const char* filepath, const void* content, const size_t len);
    static bool WriteFile(const string& filepath, const void* content, const size_t len);
};

}

#include "simcc/file_util.inl.h"
