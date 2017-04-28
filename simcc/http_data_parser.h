#pragma once

#include "memmem.h"
#include "slice.h"
#include "ini_parser.h"

namespace simcc {
class URIParser {
public:
    typedef std::map<std::string, std::string> ssmap;

    template<class _TMAP>
    static bool Parse(const char* d, size_t dlen, _TMAP& kv) {
#ifdef H_OS_WINDOWS
        const char* p = strchr((char*)d, '?');
#else
        const char* p = strchr(d, '?');
#endif
        if (!p) {
            return true;
        }

        if (p >= d + dlen) {
            return true;
        }

        int32_t len = (int32_t)dlen + d - p - 1;
        if (len <= 0) {
            return true;
        }

        bool r = true;
        INIParser ini;
        ini.set_compatible(true);
        if (!ini.Parse(p + 1, len, "&", "=")) {
            r = false;
        }

        kv.insert(ini.GetDefaultKeyValueMap().begin(), ini.GetDefaultKeyValueMap().end());

        return r;
    }



    template<class _TMAP>
    static bool Parse(const char* d, size_t dlen, std::string& uri, _TMAP* kv) {
#ifdef H_OS_WINDOWS
        const char* p = strchr((char*)d, '?');
#else
        const char* p = strchr(d, '?');
#endif
        if (!p) {
            uri.assign(d, dlen);
            return true;
        }

        if (p >= d + dlen) {
            uri.assign(d, dlen);
            return true;
        }

        uri.assign(d, p - d);

        int32_t len = (int32_t)dlen + d - p - 1;
        if (len <= 0) {
            return true;
        }

        if (!kv) {
            return true;
        }

        bool r = true;
        INIParser ini;
        ini.set_compatible(true);
        if (!ini.Parse(p + 1, len, "&", "=")) {
            r = false;
        }

        kv->insert(ini.GetDefaultKeyValueMap().begin(), ini.GetDefaultKeyValueMap().end());

        return r;
    }
};


class HttpMultiDataParser {
#ifndef CRLF
#define CRLF   "\x0d\x0a"
#endif
public:
    typedef std::map<std::string, std::string> ssmap;
    template<class _T_string, class _TMAP>
    static bool Parse(const void* d, size_t dlen, _TMAP& kv) {
        const char* end = (const char*)d + dlen;
        const char* p = (const char*)d;
        static std::string tag_name_prefix = "Content-Disposition: form-data; name=\"";
        static std::string tag_kvsep = CRLF CRLF;
        static std::string tag_value_end = CRLF "-----------------------------";
        while (p < end) {
            const char* found = static_cast<const char*>(memmem(p, end - p, tag_name_prefix.data(), tag_name_prefix.size()));
            if (!found || found >= end) {
                break;
            }

            found += tag_name_prefix.size();
            if (found >= end) {
                break;
            }

            p = found;
            while (p < end && *p != '"') ++p;
            if (p >= end) {
                break;
            }

            if (p <= found) {
                continue;
            }

            _T_string key(found, p - found);
            if (memcmp(p + 1, tag_kvsep.data(), tag_kvsep.size()) != 0) {
                //continue;
                return false;//TODO strict check
            }

            const char* value = p + 1 + tag_kvsep.size();
            if (value >= end) {
                break;
            }

            p = value;
            found = static_cast<const char*>(memmem(p, end - p, tag_value_end.data(), tag_value_end.size()));

            if (!found || found >= end) {
                break;
            }

            {
                _T_string& v = kv[key];
                if (v.empty()) {
                    v = _T_string(value, found - value);
                }
            }
            p = value;
        }

        return true;
    }


    template<class _T_string, class _TMAP>
    static bool Parse(const void* d, size_t dlen, const std::string& http_multipart_boundary, _TMAP& kv) {
        const char* end = (const char*)d + dlen;
        const char* p = (const char*)d;
        static std::string tag_name_prefix = "Content-Disposition: form-data; name=\"";
        static std::string tag_kvsep = CRLF CRLF;
        std::string tag_value_end(http_multipart_boundary.size() + 2 + 2, 0);
        tag_value_end = CRLF;
        tag_value_end.append("--", 2).append(http_multipart_boundary);
        while (p < end) {
            const char* found = static_cast<const char*>(memmem(p, end - p, tag_name_prefix.data(), tag_name_prefix.size()));
            if (!found || found >= end) {
                break;
            }

            found += tag_name_prefix.size();
            if (found >= end) {
                break;
            }

            p = found;
            while (p < end && *p != '"') ++p;
            if (p >= end) {
                break;
            }

            if (p <= found) {
                continue;
            }

            _T_string key(found, p - found);
            if (memcmp(p + 1, tag_kvsep.data(), tag_kvsep.size()) != 0) {
                //continue;
                return false;//TODO strict check
            }

            const char* value = p + 1 + tag_kvsep.size();
            if (value >= end) {
                break;
            }

            p = value;
            found = static_cast<const char*>(memmem(p, end - p, tag_value_end.data(), tag_value_end.size()));

            if (!found || found >= end) {
                break;
            }

            {
                _T_string& v = kv[key];
                if (v.empty()) {
                    v = _T_string(value, found - value);
                }
            }
            p = value;
        }

        return true;
    }



    template<class _T_string>
    static bool Extract(const void* d, size_t dlen, const _T_string& key, _T_string& out_value) {
        const char* end = (const char*)d + dlen;
        const char* p = (const char*)d;
        static std::string tag_name_prefix = "Content-Disposition: form-data; name=\"";
        static std::string tag_kvsep = CRLF CRLF;
        static std::string tag_value_end = CRLF "-----------------------------";
        while (p < end) {
            const char* found = static_cast<const char*>(memmem(p, end - p, tag_name_prefix.data(), tag_name_prefix.size()));
            if (!found || found >= end) {
                break;
            }

            found += tag_name_prefix.size();
            if (found >= end) {
                break;
            }

            p = found;
            while (p < end && *p != '"') ++p;
            if (p >= end) {
                break;
            }

            if (p <= found) {
                continue;
            }

            if (memcmp(p + 1, tag_kvsep.data(), tag_kvsep.size()) != 0) {
                continue;
            }

            const char* value_begin = p + 1 + tag_kvsep.size();
            if (value_begin >= end) {
                break;
            }

            if (memcmp(key.data(), found, p - found) != 0) {
                continue;
            }

            p = value_begin;
            found = static_cast<const char*>(memmem(p, end - p, tag_value_end.data(), tag_value_end.size()));

            if (!found || found >= end) {
                break;
            }

            out_value = _T_string(value_begin, found - value_begin);
            return true;
        }

        return false;
    }
};
}


