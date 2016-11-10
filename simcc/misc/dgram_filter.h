#pragma once

#include <thread>
#include <mutex>
#include <memory>

#include "lru.h"
#include "md5.h"
#include "simcc/ini_parser.h"

namespace simcc {

// kIntervalSeconds 时间段内，同一个IP过来的同一个数据包，最多只接收 max_threshold 个，其他都会被过滤丢弃掉。
//
// 底层实现是线程安全的，采用LRU队列来保存数据包（dgram）。
//
// 参数 lru_max_item_count 和 lru_max_memery_size_mb 是LRU相关的参数，设置LRU最大占用的内存使用量.

template<int kIntervalSeconds = 60>
class DgramFilter {
    struct Stat {
        std::mutex mutex; // the lock for Stat member fields

        time_t   update_time; // 最新的更新 Stat 状态的时间，秒
        uint32_t total; // 总的请求个数
        uint16_t count[kIntervalSeconds]; // 存储每一秒的请求个数

        Stat() : update_time(time(NULL)), total(0) {
            memset(count, 0, sizeof(count));
        }

        ~Stat() {
        }

        void Reset() {
            update_time = time(NULL);
            total = 0;
            memset(count, 0, sizeof(count));
        }

        void ClearOneBucket(int index) {
            index = index % kIntervalSeconds;
            assert(this->total >= this->count[index]);

            if (this->total < this->count[index]) {
                fprintf(stderr, "%s Logic ERROR : total < this->count[%d]", __FUNCTION__, index);
                return;
            }

            this->total -= this->count[index];
            this->count[index] = 0;
        }


        void Inc() {
            this->count[this->update_time % kIntervalSeconds]++;
            this->total++;
        }
    };

    struct Deletor : std::unary_function<Stat*, void> {
        void operator()(Stat*& v) const {
            delete v;
            v = NULL;
        }
    };

    struct Sizeof : std::unary_function<Stat*, size_t> {
        size_t operator()(const Stat* v) const {
            return sizeof(string) + simcc::MD5::kBinDigestLength + sizeof(v) + sizeof(*v);
        }
    };

    typedef LRUCacheH4< string, Stat*, Sizeof, Deletor > LRUCache;
    typedef std::shared_ptr<LRUCache> LRUCachePtr;

public:
    DgramFilter()
        : enable_(false)
        , max_threshold_(100)
        , block_second_(60 * 10)
        , lru_max_item_count_(50000)
        , lru_max_memery_size_bytes_(100 * 1024 * 1024)
        , debug_(false) {
    }

    ~DgramFilter() {
    }

    bool Initialize(bool enable,
                    uint32_t max_threshold,
                    size_t lru_max_item_count,
                    size_t lru_max_memery_size_mb,
                    uint32_t block_second) {
        enable_ = enable;
        max_threshold_ = max_threshold;
        block_second_ = block_second;
        lru_max_item_count_ = lru_max_item_count;
        lru_max_memery_size_bytes_ = lru_max_memery_size_mb * 1024 * 1024;

        if (enable_) {
            lru_.reset(new LRUCache(lru_max_item_count_,
                                    lru_max_memery_size_bytes_,
                                    Sizeof(), Deletor()));
        }

        return true;
    }

    // return true if we need filter this package
    bool IsNeedFilter(const string& data, const string& ip) {
        return IsNeedFilter((const void*)(data.data()), data.size(), ip);
    }

    // @brief
    // @param[in] data -
    // @param[in] len -
    // @param[in] ip -
    // @return bool -
    bool IsNeedFilter(const void* data, size_t len, const string& ip) {
        if (!enable_ || len == 0) {
            return false;
        }

        Stat* stat = NULL;
        string md5 = CheckSum(data, len, ip);
        assert(!md5.empty());
        time_t now  = time(NULL);

        std::lock_guard<std::mutex> mutex_guard(mutex_);
        auto it = lru_->Find(md5);
        if (it == lru_->end()) {
            stat = new Stat();
            lru_->insert(md5, stat);
        } else {
            stat = const_cast<Stat*>(it.value());
        }

        if (stat->total >= max_threshold_) {
            if (stat->update_time + block_second_ >= now) {
                return true;
            } else {
                stat->Reset();
            }
        }

        Update(now, stat);
        return false;
    }

public:
    // Debug interface
    string Dump() const {
        std::ostringstream os;
        std::lock_guard<std::mutex> mutex_guard(mutex_);
        os << "tid=" << std::this_thread::get_id()
           << " LRUCacheH4(" << lru_->size() << "/" << lru_->max_size() << ")"
           << " memory:(" << lru_->memory_size() << "/" << lru_->max_memory_size() <<  "):"
           << " LRU --> MRU: " << std::endl;

        for (auto it = lru_->lru_begin(); it != lru_->end(); ++it) {
            Stat* stat = it.value();
            string key = it.key();
            os <<  "key=" << key << " update_time=" << stat->update_time << " total=" << stat->total << std::endl;
        }

        return os.str();
    }

    void set_debug(bool v) {
        debug_ = v;
    }

public:
    static bool Initialize(const simcc::INIParser& ini, DgramFilter* filter) {
        bool dgram_filter_enable = ini.GetBool("dgram_filter_enable", false);
        int64_t dgram_filter_max_threshold = ini.GetInteger("dgram_filter_max_threshold", 100);
        int64_t dgram_filter_lru_max_item_count = ini.GetInteger("dgram_filter_lru_max_item_count", 1024 * 1024);
        int64_t dgram_filter_lru_max_memery_size_mb = ini.GetInteger("dgram_filter_lru_max_memery_size_mb", 128 * 1024 * 1024);
        int64_t dgram_filter_block_second = ini.GetInteger("dgram_filter_block_second", 180);

        return filter->Initialize(
                   dgram_filter_enable,
                   dgram_filter_max_threshold,
                   dgram_filter_lru_max_item_count,
                   dgram_filter_lru_max_memery_size_mb,
                   dgram_filter_block_second);
    }

private:

    void Update(time_t now, Stat* stat) const {
        if (now >= stat->update_time + kIntervalSeconds) {
            // one more round trip, clear the expired data
            stat->Reset();
            stat->Inc();
            return;
        }

        if (now == stat->update_time) {
            stat->Inc();
            return;
        }

        int last_index = stat->update_time % kIntervalSeconds + kIntervalSeconds;
        int current_index = now % kIntervalSeconds + kIntervalSeconds;
        assert(last_index != current_index);

        for (int i = last_index + 1; i <= current_index; ++i) {
            stat->ClearOneBucket(i);
        }

        stat->update_time = now;
        stat->Inc();
    }

    string CheckSum(const void* data, size_t len, const string& ip) const {
        simcc::MD5 md5;
        md5.Update(data, len);

        if (!ip.empty()) {
            md5.Update(ip.data(), ip.size());
        }

        if (debug_) {
            return md5.Finalizeh();
        } else {
            return md5.Finalize();
        }
    }

private:
    std::mutex mutex_; // the lock for lru_
    LRUCachePtr lru_;

    bool enable_;
    uint32_t max_threshold_;
    uint32_t block_second_;

    size_t lru_max_item_count_;
    size_t lru_max_memery_size_bytes_;

    bool debug_;
};

}


