#pragma once

#include "wcpp/inner_pre.h"
#include "wcpp/type_traits_ext.h"
#include <string.h>

#include <string>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>

namespace wcpp {

// The class encapsulates data stream in memory.
class WCPP_EXPORT DataStream {
public:
    enum Status {
        kGood = 0,
        kReadBad = 1 << 1,
        kWriteBad = 1 << 2,
    };

public:
    DataStream();

    // Construct with memory size.
    explicit DataStream(size_t buf_size);

    // Construct with outside memory and size.
    // @note if need_free is true, the memory pointed by d will be released by calling free(...)
    //       when this instance is released, so d MUST be allocated by call to malloc(...)
    explicit DataStream(void* d, size_t len, bool need_free);

    ~DataStream();

    // Assure convert to text.
    // @note Add ending null to the stream, but do not change the data size.
    void ToText();

    // Query whether the read operation is bad.
    bool IsReadBad()const;

    // Query whether the write operation is bad.
    bool IsWriteBad()const;

    // Sets stats of the file.
    // @param bits - kWriteBad or kReadBad or 0 or kWriteBad|kReadBad
    void SetStatus(uint32_t bits);

    // Open the raw file and read all the file data to this memory data stream
    // @note It is only a helper method.
    bool ReadFile(const std::string& filename);

    // @brief: Helper method to save data to a disk file.
    // @param[in]: const std::string& filename, the path name of the file, it can include dir path
    // @return: bool
    bool WriteFile(const std::string& filename);

    // Read data form data stream, the result is stored in buf
    // @remark The read count is in len,it may be lesser than the buf_len,
    //         the len will be zero if read nothing or there is an error occurred
    // @param  buf: the buffer which the result will be stored
    // @param  buf_len: the buf length in byte
    // @return the return value is true if read succeed, else false
    bool Read(void* buf, size_t buf_len);

    // Write a value to data stream
    // @param  v: the value which will be written
    // @note   the return value is true if write succeed, else false
    bool Write(int8_t ch);
    
    // Read 4 bytes from this data stream, the data in this stream is Little Endian
    //  In little endian CPU system:
    //      char* pc = pU32;
    //      buffer_[0] -> pc[0]
    //      buffer_[1] -> pc[1]
    //      buffer_[2] -> pc[2]
    //      buffer_[3] -> pc[3]
    //  In big endian CPU system:
    //      char* pc = pint;
    //      buffer_[0] -> pc[3]
    //      buffer_[1] -> pc[2]
    //      buffer_[2] -> pc[1]
    //      buffer_[3] -> pc[0]
    // @param  pint : the value will stored there
    // @return the return value is true if read succeed, else false
    bool ReadLE(uint32* pU32);

    // Write 4 bytes int data to this data stream, the data in this stream is Little Endian
    //  In little endian CPU system:
    //      char* pc = &i;
    //      pc[0] -> buffer_[0]
    //      pc[1] -> buffer_[1]
    //      pc[2] -> buffer_[2]
    //      pc[3] -> buffer_[3]
    //  In big endian CPU system:
    //      char* pc = &i;
    //      pc[3] -> buffer_[0]
    //      pc[2] -> buffer_[1]
    //      pc[1] -> buffer_[2]
    //      pc[0] -> buffer_[3]
    // @param  i : the int which will be written
    // @return true if write successfully, else false
    bool WriteLE(uint32 i);

    // Write data to data stream
    // @param buf: the buffer where stored the data to be written to the stream
    // @param buf_len: the buf length in byte
    // @return true if write successfully, else false
    bool Write(const void* buf, size_t buf_len);

    // Sets the capacity of the stream to at least size,
    //  the stream's data buffer maybe extend.
    // @note It will not change any flag of the stream if successfully
    // @return false if failed to allocate enough memory
    //   and set the reading and writing flag as kReadBad and kWriteBad
    bool Reserve(size_t size);

    // Get the total length of data in byte which has been written into this stream
    size_t size() const {
        return tellp();
    }
    const char* data() const {
        return reinterpret_cast<const char*>(GetCache());
    }

    // get the buffer base address pointer, don't delete the returned pointer
    void* GetCache() const;

    // return the char at the index of the data stream
    uint8 CharAt(size_t index) const;

    // Returns the number of bytes of the unread portion of the buffer
    uint32 GetReadableSize() const;

    // Gets buffer pointer to the current read position.
    void* GetCurrentReadBuffer() const;
    void* GetCurrentWriteBuffer() const;

    // Reset the data stream
    void Reset();

    void Swap(DataStream& r);

    bool Resize(size_t len);

    // Expand size of memory to the current stack.
    // @return false if failed to allocate new memory.
    bool Expand(uint32 nSizeToAdd);

    static bool IsContentEquals(const DataStream& first, const DataStream& second);

    // The same interfaces for std::ostream/istream
public:
    bool put(char ch);

    bool write(const void* buf, size_t buf_len);

    // Move the stream pointer for read
    // @remark   after seek, the read pointer' position is at the stream buffer' base address + start + offset,
    // or at the stream buffer' base address if the result is lesser than the base address
    // or at the end of stream buffer if the result exceeded the end
    //
    // @param  start: the position where the pointer will moving
    // @param  offset: the offset to move
    uint32 seekg(int32 offset);

    // get current read position
    uint32 tellg()const;

    // Move the stream pointer for write
    // @remark   after seek, the write pointer' position is at the stream buffer' base address + start + offset,
    // or at the stream buffer' base address if the result is lesser than the base address
    // or at the end of stream buffer if the result exceeded the end
    //
    // @param  offset: the offset to move
    //      if offset < 0, this function doesn't case about m_nSize
    uint32 seekp(int32 offset);

    // get current write position
    uint32 tellp()const;
    
    bool reserve(size_t size);

private:
    template< typename T>
    DataStream& InternalWriteType(const T& val, std::true_type);

    template< typename T>
    DataStream& InternalReadType(T& val, std::true_type);

    template< typename _Kt >
    DataStream& InternalWriteVector(const std::vector< _Kt >& val, std::true_type);

    template< typename _Kt >
    DataStream& InternalReadVector(std::vector< _Kt >& val, std::true_type);


    // @brief 不要给这些函数增加具体的实现代码。如果一个结构体没有实现序列化函数，在编译链接阶段，会报错。
    template< typename T>
    DataStream& InternalWriteType(const T& val, std::false_type);
    template< typename T>
    DataStream& InternalReadType(T& val, std::false_type);

public:
    template< typename T> DataStream& operator<<(const T& val);
    template< typename T> DataStream& operator>> (T& val);

    DataStream& operator<<(const std::string& val);
    DataStream& operator>>(std::string& val);

    DataStream& operator<<(const char* szVal);

    DataStream& operator<<(const DataStream& val);
    DataStream& operator>>(DataStream& val);

    template<  typename _Kt, typename _Val >
    DataStream& operator>>(std::pair<_Kt, _Val>& val);
    template<  typename _Kt, typename _Val >
    DataStream& operator<<(const std::pair<_Kt, _Val>& val);

    template< typename _Kt >
    DataStream& operator<<(const std::vector< _Kt>& val);
    template< typename _Kt >
    DataStream& operator>>(std::vector< _Kt>& val);

    template< typename _Kt >
    DataStream& operator<<(const std::list< _Kt>& val);
    template< typename _Kt >
    DataStream& operator>>(std::list< _Kt>& val);


    template<  typename _Kt, typename _Val >
    DataStream& operator<<(const std::map< _Kt, _Val >& val);
    template< typename _Kt, typename _Val >
    DataStream& operator>>(std::map<_Kt, _Val>& val);

    template<class T>
    DataStream& operator<<(const std::set<T>& val);
    template<class T>
    DataStream& operator>>(std::set<T>& val);

    template< typename _Kt, typename _Val >
    DataStream& operator<<(const std::unordered_map<_Kt, _Val>& val);
    template< typename _Kt, typename _Val >
    DataStream& operator>>(std::unordered_map<_Kt, _Val>& val);
private:
    uint8_t* buffer_;   // Buffer to hold all the data. It can expand when it is wrote and is not large enough to hold more data.
    bool self_created_;   // Whether the buffer is created by this instance itself.
    uint32_t capacity_;   // Size of buffer_.
    uint32_t write_index_;  // Current write data cursor in the buffer.
    uint32_t read_index_;   // Current read data cursor in the buffer.
    uint32_t status_;   // status of the file.

private:
    // Hide copy constructor
    DataStream(const DataStream&);
    DataStream& operator= (const DataStream&);
};

};


#include "data_stream.inl.h"
