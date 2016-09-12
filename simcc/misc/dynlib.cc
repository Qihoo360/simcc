#include "dynlib.h"

#include <sstream>

#ifndef H_OS_WINDOWS
#include <dlfcn.h>
#endif

namespace simcc {

DynLib::DynLib(const string& dll_path)
    : dll_path_(dll_path), handler_(NULL) {
}

DynLib::~DynLib() {
}

bool DynLib::IsLoaded() {
    return handler_ == NULL ? false : true;
}

bool DynLib::Load() {
    string name = dll_path_;
#ifdef H_OS_WINDOWS
    handler_ = (DYNLIB_HANDLE)::LoadLibraryA(name.c_str());
#else
    handler_ = (DYNLIB_HANDLE)::dlopen(name.c_str(), RTLD_LAZY | RTLD_LOCAL);
#endif

    if (handler_) {
        return true;
    }

    std::ostringstream ss;
    ss << __FUNCTION__ << " : Load dynamic library "
       << dll_path_ << " failed. "
       << "Error message : " << GetError();
    error_ = ss.str();
    return false;
}

bool DynLib::Unload() {
    if (!handler_) {
        std::ostringstream ss;
        ss << __FUNCTION__
           << " Could not unload dynamic library " << dll_path_
           << ". maybe it haven't loaded yet, because handler_=NULL";
        error_ = ss.str();
        return false;
    }

#ifdef H_OS_WINDOWS
    // If the function FreeLibrary() succeeds, the return value is nonzero.
    if (::FreeLibrary(handler_)) {
        return true;
    }
#else
    //The function dlclose() returns 0 on success, and non-zero on error.
    if (dlclose(handler_) == 0) {
        return true;
    }
#endif

    std::ostringstream ss;
    ss << __FUNCTION__ << " : Unload dynamic library "
       << dll_path_ << " failed. "
       << "Error message : " << GetError();
    error_ = ss.str();
    return false;
}

void* DynLib::GetSymbol(const string& name) {
    void* rp = NULL;

#ifdef H_OS_WINDOWS
    rp = (void*)::GetProcAddress(handler_, name.c_str());
#else
    rp = (void*)::dlsym(handler_, name.c_str());
#endif

    if (rp == NULL) {
        std::ostringstream ss;
        ss << __FUNCTION__
           << " : Could not get the symbol : " << name
           << ". Error Message : " << GetError();

        error_ = ss.str();
    }

    return rp;
}

string DynLib::GetError() {
#ifdef H_OS_WINDOWS
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        ::GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0,
        NULL
    );
    string ret = (char*)lpMsgBuf;
    LocalFree(lpMsgBuf);
    return ret;
#else
    return string(dlerror());
#endif
}

}
