#pragma once

#include "simcc/inner_pre.h"
#include "simcc/ref_object.h"

namespace simcc {

#ifdef H_OS_WINDOWS
    #define DYNLIB_HANDLE HMODULE
#else
    #define DYNLIB_HANDLE void*
#endif

// Dynamic library. It is used load and find symbol in the library.
// @remark You should keep the object alive before unload the loaded library.
class SIMCC_EXPORT DynLib : public RefObject {
public:
    // @param dll_path The full path for a library. e.g. "e:/project/hph/bin/test.dll", "/root/bin/test.so"
    DynLib(const string& dll_path);
    ~DynLib();

    // Actively load the library.
    // @return false if failed to load.
    bool Load();

    // Unload the library.
    // @return false if failed to unload
    bool Unload();

    // Query whether the library has been loaded.
    bool IsLoaded();

    // Get full path of the library
    const string& path(void) const {
        return dll_path_;
    }

    // Gets symbol in the library.
    // @return NULL if find nothing.
    void* GetSymbol(const string& func_name);


    // Gets the last loading error. It is used get the error message
    // when failed to load or unload library.
    const string& GetLastError() const {
        return error_;
    }

private:
    string dll_path_;
    DYNLIB_HANDLE handler_;
    string error_;

private:
    // Generate library load error. It generated from system.
    string GetError();

};

typedef RefPtr<DynLib> DynLibPtr;

}