#pragma once

namespace simcc {

inline void DataStream::Swap(DataStream& r) {
    std::swap(buffer_ , r.buffer_);
    std::swap(self_created_ , r.self_created_);
    std::swap(capacity_ , r.capacity_);
    std::swap(write_index_ , r.write_index_);
    std::swap(read_index_ , r.read_index_);
    std::swap(status_ , r.status_);
}

//
#pragma pack(push,1)
template<typename TE1, typename TE2 >
struct Struct2Element {
    TE1    v1;
    TE2    v2;
};

template<typename TE1, typename TE2, typename TE3 >
struct Struct3Element {
    TE1    v1;
    TE2    v2;
    TE3    v3;
};

template<typename TE1, typename TE2, typename TE3, typename TE4 >
struct Struct4Element {
    TE1    v1;
    TE2    v2;
    TE3    v3;
    TE4    v4;
};
#pragma pack(pop)

// Serialize Struct2Element
template<typename TE1, typename TE2 >
inline simcc::DataStream& operator<< (simcc::DataStream& file, const Struct2Element<TE1, TE2 >& val) {
    file << (const TE1&)val.v1
         << (const TE2&)val.v2;

    return file;
}

// Serialize Struct2Element
template<typename TE1, typename TE2 >
inline simcc::DataStream& operator>> (simcc::DataStream& file,  Struct2Element<TE1, TE2 >& val) {
    file >> (TE1&)val.v1
         >> (TE2&)val.v2;

    return file;
}


// Serialize Struct2Element
template<typename TE1, typename TE2, typename TE3 >
inline simcc::DataStream& operator<< (simcc::DataStream& file, const Struct3Element<TE1, TE2, TE3 >& val) {
    file << (const TE1&)val.v1
         << (const TE2&)val.v2
         << (const TE3&)val.v3;
    return file;
}

// Serialize Struct2Element
template<typename TE1, typename TE2, typename TE3 >
inline simcc::DataStream& operator>> (simcc::DataStream& file,  Struct3Element<TE1, TE2, TE3 >& val) {
    file >> (TE1&)val.v1
         >> (TE2&)val.v2
         >> (TE3&)val.v3;
    return file;
}

template<typename TE1, typename TE2, typename TE3, typename TE4 >
inline simcc::DataStream& operator<< (simcc::DataStream& file, const Struct4Element<TE1, TE2, TE3, TE4 >& val) {
    file << (const TE1&)val.v1
         << (const TE2&)val.v2
         << (const TE3&)val.v3
         << (const TE4&)val.v4;
    return file;
}

template<typename TE1, typename TE2, typename TE3, typename TE4 >
inline simcc::DataStream& operator>> (simcc::DataStream& file,  Struct4Element<TE1, TE2, TE3, TE4 >& val) {
    file >> (TE1&)val.v1
         >> (TE2&)val.v2
         >> (TE3&)val.v3
         >> (TE4&)val.v4;
    return file;
}






// MemoryDataStream Implementation

inline DataStream::DataStream() : buffer_(NULL)
    , self_created_(false)
    , capacity_(0)
    , write_index_(0)
    , read_index_(0)
    , status_(0) {

}

inline DataStream::DataStream(size_t nBufferSize)
    : self_created_(true)
    , capacity_(nBufferSize)
    , write_index_(0)
    , read_index_(0)
    , status_(0) {
    buffer_ = (uint8*)malloc(capacity_);

    if (!buffer_) {
        capacity_ = 0;
    }
}
// Construct with outside memory and size.
inline DataStream::DataStream(void* pData, size_t nBufferSize, bool bDestroy)
    : buffer_((uint8_t*)pData)
    , self_created_(bDestroy)
    , capacity_(nBufferSize)
    , write_index_(0)
    , read_index_(0)
    , status_(0) {
}

inline DataStream::~DataStream() {
    if (buffer_ && self_created_) {
        free(buffer_);
    }
}

inline bool DataStream::IsReadBad() const {
    return (status_ & kReadBad) == 0  ? false : true;
}

inline bool DataStream::IsWriteBad() const {
    return (status_ & kWriteBad) == 0 ? false : true;
}

inline void DataStream::SetStatus(uint32_t nBits) {
    status_ |= nBits;
}

inline bool DataStream::Read(void* buf, size_t buf_len) {
    // check whether the file is bad.
    assert(buf && buffer_);

    if (IsReadBad()) {
        return false;
    }

    uint32 nNewPos = read_index_ + buf_len;

    if (nNewPos > (uint32)write_index_) {
        SetStatus(kReadBad);
        return false;
    }

    memcpy(buf, buffer_ + read_index_, buf_len);

    read_index_ = nNewPos;

    return true;
}

inline bool DataStream::Write(int8_t v)   {
    return Write(&v, sizeof(v));
}

inline bool DataStream::ReadLE(uint32* pu32) {
    if (this->GetReadableSize() < 4) {
        return false;
    }

#ifdef H_LITTLE_ENDIAN
    *this >> *pu32;
#else
    uint8* pc = (uint8*)pu32;
    *this >> pc[3];
    *this >> pc[2];
    *this >> pc[1];
    *this >> pc[0];
#endif

    return true;
}

inline bool DataStream::WriteLE(uint32 i) {
#ifdef H_LITTLE_ENDIAN
    *this << i;
#else
    *this << (char)((i & 0xff000000) >> 24);
    *this << (char)((i & 0x00ff0000) >> 16);
    *this << (char)((i & 0x0000ff00) >> 8);
    *this << (char)((i & 0x000000ff));
#endif
    return true;
}

inline bool DataStream::Write(const void* buf, size_t buf_len) {
    assert(buf);

    if (!Expand(buf_len)) {
        return false;
    }

    memcpy(buffer_ + write_index_, buf, buf_len);
    write_index_ += buf_len;
    return true;
}

inline DataStream& DataStream::seekg(int32 offset) {
    int32 nNewPos = read_index_ + offset;

    if (nNewPos > (int32)write_index_) {
        read_index_ = write_index_;
        SetStatus(kReadBad);

    } else if (nNewPos < 0) {

        read_index_ = 0;
        SetStatus(kReadBad);
    } else {
        read_index_ = nNewPos;
    }

    return *this;
}

inline uint32 DataStream::tellg() const {
    return read_index_;
}

inline DataStream& DataStream::seekp(int32 offset) {
    int64 new_pos = (int32)write_index_ + offset;

    if (new_pos < 0) {
        write_index_ = 0;
    } else {
        // pre-allocate size.
        if (new_pos > (simcc::int64)write_index_) {
            if (!Expand((uint32)new_pos - write_index_)) {
                return *this;
            }
        }

        // finished check, set the new size.
        write_index_ = new_pos;
    }

    return *this;
}

inline uint32 DataStream::tellp() const {
    return write_index_;
}

inline bool DataStream::reserve(size_t sz) {
    return Reserve(sz);
}

inline DataStream& DataStream::put(char ch) {
    Write(ch);
    return *this;
}

inline DataStream& DataStream::write(const void* buf, size_t buf_len) {
    Write(buf, buf_len);
    return *this;
}

inline void* DataStream::GetCache() const {
    assert(buffer_);
    return buffer_;
}

inline uint8 DataStream::CharAt(size_t index) const {
    assert(index < size());
    return *((int8*)GetCache() + index);
}

inline simcc::uint32 DataStream::GetReadableSize() const {
    return size() - (uint32)tellg();
}

inline void* DataStream::GetCurrentReadBuffer() const {
    return ((char*)GetCache()) + tellg();
}
inline void* DataStream::GetCurrentWriteBuffer() const {
    return ((char*)GetCache()) + tellp();
}

inline void DataStream::Reset() {
    write_index_ = 0;
    read_index_ = 0;
    status_ = kGood;
}

inline bool DataStream::Resize(size_t nSize) {
    // check size and assure enough buffer.
    if (nSize > capacity_) {
        if (!Expand(nSize + capacity_)) {
            return false;
        }
    } else if (nSize == 0) {
        /*
        这里添加 ‘nSize == 0’ 是为了确保分配内存给 buffer_ 。
        */
        if (!Expand(sizeof(long))) {
            return false;
        }
    }

    write_index_ = nSize;

    return true;
}


inline bool DataStream::Expand(uint32 nSizeToAdd) {
    uint32 nNewSize = write_index_ + nSizeToAdd + 1;

    // only if buffer is no sufficient, we reallocate it.
    if (nNewSize > (uint32)capacity_) {
        uint32 nNewBufferSize = nNewSize + (nNewSize >> 1);

        uint8* pNewBuf = (uint8*)malloc(nNewBufferSize);

        if (!pNewBuf) {
            SetStatus(kReadBad | kWriteBad);
            return false;
        }

        if (buffer_) {
            memcpy(pNewBuf, buffer_, capacity_);
        }


        if (self_created_) {
            free(buffer_);
        }

        buffer_ = pNewBuf;
        capacity_ = nNewBufferSize;
        self_created_ = true;
    }

    return true;
}


inline bool DataStream::Reserve(size_t new_size) {
    if (new_size > capacity_) {
        uint8* new_buf = (uint8*)malloc(new_size);

        if (!new_buf) {
            SetStatus(kReadBad | kWriteBad);
            return false;
        }

        capacity_ = new_size;

        if (buffer_) {
            memcpy(new_buf, buffer_, write_index_);
        }


        if (self_created_) {
            // the old buffer is created by myself, so free it
            free(buffer_);
        }

        buffer_ = new_buf;
        self_created_ = true;
    }

    return true;
}


template< typename T>
DataStream& simcc::DataStream::InternalWriteType(const T& val, std::true_type) {
    Write(&val, sizeof(T));
    return (*this);
}

template< typename T>
DataStream& simcc::DataStream::InternalReadType(T& val, std::true_type) {
    Read(&val, sizeof(T));
    return (*this);
}

template< typename _Kt >
DataStream& simcc::DataStream::InternalWriteVector(const std::vector< _Kt >& val, std::true_type) {
    // 1. write length
    *this << uint32(val.size());

    // 2. memory
    if (!val.empty()) {
        this->write(&(val[0]), uint32(sizeof(_Kt) * val.size()));
    }
    return *this;
}

template< typename _Kt >
DataStream& simcc::DataStream::InternalReadVector(std::vector< _Kt >& val, std::true_type) {
    // check whether the file is bad.
    if (IsReadBad()) {
        return *this;
    }

    // 1. write length
    uint32 nSize = 0;
    *this >> nSize;

    if (GetReadableSize() < nSize) {
        SetStatus(kReadBad);
        return *this;
    }

    val.resize(nSize);

    // 2. memory
    if (nSize) {
        Read(&(val[0]), sizeof(_Kt) * nSize);
    }
    return *this;
}

template< typename T>
DataStream& simcc::DataStream::operator<<(const T& val) {
    typedef  typename std::is_pod<T>::type T_type;
    return InternalWriteType(val, T_type());
    //return *this;
}


template< typename T>
DataStream& simcc::DataStream::operator >> (T& val) {
    typedef  typename std::is_pod<T>::type T_type;
    return InternalReadType(val, T_type());
    //return *this;
}

inline simcc::DataStream& DataStream::operator<<(const std::string& val) {
    uint32_t nStrLen = (uint32)val.length();

    // 1. write string length
    *this << nStrLen;

    // 2. write string
    Write(val.c_str(), (uint32)nStrLen);

    return *this;
}

inline simcc::DataStream& DataStream::operator<<(const char* szVal) {
    if (NULL == szVal) {
        return *this;
    }

    uint32 nStrLen = (uint32)strlen(szVal);

    // 1. write string length
    *this << (uint32&)nStrLen;

    // 2. write string
    Write(szVal, nStrLen);

    return *this;
}

inline simcc::DataStream& DataStream::operator >> (std::string& val) {
    // check whether the file is bad.
    if (IsReadBad()) {
        return *this;
    }

    // 1. get length
    uint32 nSize = 0;
    *this >> (uint32&)nSize;

    // 2. get file
    if (nSize <= (uint32)GetReadableSize()) {
        val.resize(nSize);

        if (nSize) {
            Read(&val[0], nSize);

            // assure last character is null
            val[nSize] = (char)0;
        }

    } else {
        SetStatus(kReadBad);
    }

    return *this;
}

inline simcc::DataStream& DataStream::operator<<(const DataStream& val) {
    // 1. write string length
    *this << (uint32)val.size();

    // 2. write string
    if (val.size() > 0) {
        Write((char*)val.GetCache(), val.size());
    }

    return *this;
}

inline simcc::DataStream& DataStream::operator >> (DataStream& val) {
    // check whether the file is bad.
    if (IsReadBad()) {
        return *this;
    }

    // 1. read length
    uint32 nSize = 0;
    *this >> nSize;

    //
    val.Reset();

    if (nSize == 0) {
        return *this;
    }

    if (nSize <= (uint32)GetReadableSize()) {
        // 2. read string
        val.Write(((char*)GetCache() + tellg()), nSize);

        // skip
        seekg(nSize);
    } else {
        SetStatus(kReadBad);
    }

    return *this;
}


template<  typename _Kt, typename _Val >
DataStream& simcc::DataStream::operator >> (std::pair<_Kt, _Val>& val) {
    (*this) >> val.first >> val.second;
    return *this;
}


template< typename _Kt >
inline DataStream& DataStream::operator<<(const std::vector< _Kt>& val) {
    // 1. write length
    //typedef  typename stdext::bool_type<stdext::is_pod<_Kt>::value>::type _Kt_type;
    typedef  typename std::is_pod<_Kt>::type _Kt_type;
    return InternalWriteVector(val, _Kt_type());
}

template< typename _Kt >
inline DataStream& DataStream::operator<<(const std::list< _Kt>& val) {
    // 1. write length
    *this << (uint32)val.size();

    typedef typename std::list< _Kt>::const_iterator ConstIterator;
    ConstIterator it(val.begin()), ite(val.end());

    for (; it != ite; ++it) {
        *this << (const _Kt&)*it;
    }

    return *this;
}

template<  typename _Kt, typename _Val >
inline DataStream& DataStream::operator<<(const std::pair<_Kt, _Val>& val) {
    (*this) << val.first << val.second;
    return *this;
}

template<  typename _Kt, typename _Val >
inline DataStream& DataStream::operator<<(const std::map< _Kt, _Val >& val) {
    // 1. write length
    *this << (uint32)val.size();

    // 2. elements.
    typedef typename std::map<_Kt, _Val >::const_iterator ConstIterator;
    ConstIterator it(val.begin()), ite(val.end());

    for (; it != ite; ++it) {
        // get key
        *this << static_cast<const _Kt&>(it->first);

        // get value
        *this << static_cast<const _Val&>(it->second);
    }

    return *this;
}

template< typename _Kt, typename _Val >
inline DataStream& DataStream::operator >> (std::map<_Kt, _Val>& val) {
    // check whether the file is bad.
    if (IsReadBad()) {
        return *this;
    }


    // 1. read length
    uint32 nSize = 0;
    *this >> (uint32&)nSize;

    if (GetReadableSize() < nSize) {
        SetStatus(kReadBad);
        return *this;
    }

    val.clear();

    for (uint32 i = 0; i < nSize; ++i) {
        if (GetReadableSize() == 0) {
            SetStatus(kReadBad);
            break;
        }

        // key,  value,
        _Kt key = _Kt();

        // get key
        *this >> (_Kt&)key;

        // get value
        *this >> (_Val&)val[key];
    }

    return *this;
}

template< typename _Kt >
inline DataStream& DataStream::operator >> (std::list< _Kt>& val) {
    // check whether the file is bad.
    if (IsReadBad()) {
        return *this;
    }

    // 1. read length
    uint32 nSize = 0;
    *this >> (uint32&)nSize;

    if (GetReadableSize() < nSize) {
        SetStatus(kReadBad);
        return *this;
    }

    val.clear();

    for (uint32 i = 0; i < nSize; ++i) {
        if (GetReadableSize() == 0) {
            SetStatus(kReadBad);
            break;
        }

        val.push_back(_Kt());
        *this >> (_Kt&)val.back();
    }

    return *this;
}

template< typename _Kt >
inline DataStream& DataStream::operator >> (std::vector< _Kt>& val) {
    typedef  typename std::is_pod<_Kt>::type _Kt_type;
    return InternalReadVector(val, _Kt_type());
}

template<class T>
inline DataStream& DataStream::operator<<(const std::set<T>& val) {
    (*this) << (simcc::uint32)val.size();

    typedef typename std::set<T>::const_iterator Iterator;
    Iterator end = val.end();

    for (Iterator i = val.begin(); i != end; ++i) {
        (*this) << (*i);
    }

    return (*this);
}

template<class T>
inline DataStream& DataStream::operator >> (std::set<T>& val) {
    uint32 nSize;
    (*this) >> nSize;

    if ((*this).GetReadableSize() < nSize) {
        (*this).SetStatus(DataStream::kReadBad);
        return (*this);
    }

    for (size_t i = 0; i < nSize; ++i) {
        if ((*this).GetReadableSize() == 0) {
            (*this).SetStatus(DataStream::kReadBad);
            val.clear();
            break;
        }

        T element = T();
        (*this) >> element;
        val.insert(element);
    }

    return (*this);
}

template< typename _Kt, typename _Val >
DataStream& DataStream::operator<<(const std::unordered_map<_Kt, _Val>& val) {
    // 1. write length
    *this << (uint32)val.size();

    // 2. elements.
    auto it(val.begin()), ite(val.end());

    for (; it != ite; ++it) {
        // get key
        *this << static_cast<const _Kt&>(it->first);

        // get value
        *this << static_cast<const _Val&>(it->second);
    }

    return *this;
}

template< typename _Kt, typename _Val >
DataStream& DataStream::operator >> (std::unordered_map<_Kt, _Val>& val) {
    // check whether the file is bad.
    if (IsReadBad()) {
        return *this;
    }

    // 1. read length
    uint32 nSize = 0;
    *this >> (uint32&)nSize;

    if (GetReadableSize() < nSize) {
        SetStatus(kReadBad);
        return *this;
    }

    val.clear();

    for (uint32 i = 0; i < nSize; ++i) {
        if (GetReadableSize() == 0) {
            SetStatus(kReadBad);
            break;
        }

        // key,  value,
        _Kt key = _Kt();

        // get key
        *this >> (_Kt&)key;

        // get value
        *this >> (_Val&)val[key];
    }

    return *this;
}


}


namespace std {
inline void swap(simcc::DataStream& l , simcc::DataStream& r) {
    l.Swap(r);
}
}

