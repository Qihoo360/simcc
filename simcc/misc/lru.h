/*
 * Implementation of an LRU cache with a maximum size.
 *
 * See http://code.google.com/p/lru-cache-cpp/ for usage and limitations.
 *
 * Licensed under the GNU LGPL: http://www.gnu.org/copyleft/lesser.html
 *
 * Pierre-Luc Brunelle, 2011
 * pierre-luc.brunelle@polytml.ca
 *
 * Modified by weizili, 2012/07/01
 *  1. Change hashtable to unordered_map as the container
 *  2. Add memory limit
 *  3. Add value deleter(optional)
 *  4. Change LRUCacheH4Value::Value : use a pointer to optimize the memory use
 *  5. MS-VC compatible compile pass
 */

#include <sstream>
#include <cassert>
#include <unordered_map>
#include <functional>

namespace simcc {

// Bucket
template<class K, class V>
struct LRUCacheH4Value {
    typedef LRUCacheH4Value<K, V> Value;

    LRUCacheH4Value()
        : older(NULL), newer(NULL) {}

    LRUCacheH4Value(const V& v, Value* o, Value* n)
        : value(v), older(o), newer(n) {}

    V value;
    const K* key;
    LRUCacheH4Value<K, V>* older; // The older element. It is the same as a previous node for a link list.
    LRUCacheH4Value<K, V>* newer; // The newer element. It is the same as a next node for a link list.
};


// Const Iterator
template<class K, class V>
class LRUCacheH4ConstIterator { //
public:
    typedef LRUCacheH4Value<K, V> Value;
    typedef LRUCacheH4ConstIterator<K, V> const_iterator;
    typedef Value& reference;
    typedef Value* pointer;

    enum Direction {
        kUnknown = 0,
        kMRU2LRU = 1, // newest to oldest
        kLRU2MRU = 2, // oldest to newest
    };

    LRUCacheH4ConstIterator() : ptr_(NULL), dir_(kUnknown) {}
    LRUCacheH4ConstIterator(const Value* ptr, Direction dir);

    const_iterator& operator++();
    const_iterator operator++(int);

    bool operator==(const const_iterator& other);
    bool operator!=(const const_iterator& other);

    const K& key() const;
    const V& value() const;

private:
    const Value* ptr_;
    Direction dir_;
};


template<class K, class V>
LRUCacheH4ConstIterator<K, V>::LRUCacheH4ConstIterator(
    const typename LRUCacheH4ConstIterator<K, V>::Value* ptr,
    typename LRUCacheH4ConstIterator<K, V>::Direction dir)
    : ptr_(ptr), dir_(dir) {
}

template<class K, class V>
LRUCacheH4ConstIterator<K, V>& LRUCacheH4ConstIterator<K, V>::operator++() {
    assert(ptr_);
    ptr_ = (dir_ == LRUCacheH4ConstIterator<K, V>::kMRU2LRU ? ptr_->older : ptr_->newer);
    return *this;
}


template<class K, class V>
LRUCacheH4ConstIterator<K, V> LRUCacheH4ConstIterator<K, V>::operator++(int) {
    const_iterator ret = *this;
    ++*this;
    return ret;
}


template<class K, class V>
bool LRUCacheH4ConstIterator<K, V>::operator==(const const_iterator& other) {
    return ptr_ == other.ptr_;
}


template<class K, class V>
bool LRUCacheH4ConstIterator<K, V>::operator!=(const const_iterator& other) {
    return ptr_ != other.ptr_;
}


template<class K, class V>
const K& LRUCacheH4ConstIterator<K, V>::key() const {
    assert(ptr_);
    return *(ptr_->key);
}

template<class K, class V>
const V& LRUCacheH4ConstIterator<K, V>::value() const {
    assert(ptr_);
    return ptr_->value;
}


//-------------------------------------------------------------
// default functor
//-------------------------------------------------------------
template<class V>
struct SizeofFunctor : public std::unary_function<V, size_t>
{
    size_t operator()(const V& v) const {
        return sizeof(v);
    }
};

template<class V>
struct NonFunctor : public std::unary_function<V, void>
{
    void operator()(V& v) const {}
};

// LRU Cache
template<
    class K,
    class V,
    class S = SizeofFunctor<V>,
    class D = NonFunctor<V>>
class LRUCacheH4 {
public:
    typedef LRUCacheH4ConstIterator<K, V> const_iterator;
    typedef LRUCacheH4ConstIterator<K, V> iterator;
    typedef typename LRUCacheH4ConstIterator<K, V>::Direction Direction;

public:
    LRUCacheH4(size_t max_item_count, size_t max_memery_size_bytes,
               S s = SizeofFunctor<V>(),
               D d = NonFunctor<V>());

    ~LRUCacheH4();

    void Insert(const K& key, const V& value) {
        insert(key, value);
    }
    const_iterator Find(const K& key) {
        return find(key);
    }
    const_iterator Find(const K& key) const {
        return find(key);
    }
    const_iterator MRUBegin() const {
        return mru_begin();
    }
    const_iterator LRUBegin() const {
        return lru_begin();
    }
    const_iterator End() const {
        return end();
    }


public:
    // compatible for STL interface
    void insert(const K& key, const V& value);
    const_iterator find(const K& key); // updates the MRU
    const_iterator find(const K& key) const; // does not update the MRU
    const_iterator mru_begin() const; // from MRU to LRU, from latest to oldest
    const_iterator lru_begin() const; // from LRU to MRU, from oldest to latest
    const_iterator end() const;
public:
    // WARNING : Only for test use, because this function cannot calculate memory size
    V& operator[](const K& key);

    // WARNING : Only for debug
    void Dump(std::ostream& os, typename const_iterator::Direction dir) const;

public:
    size_t size() const {
        return map_.size();
    }
    size_t memory_size() const {
        return memory_size_;
    }
    bool empty() const {
        return map_.empty();
    }
    size_t max_size() const {
        return max_size_;
    }
    size_t max_memory_size() const {
        return max_memory_size_;
    }
    void set_max_size(size_t v) {
        max_size_ = v;
    }
    void set_max_memory_size(size_t v) {
        max_memory_size_ = v;
    }

private:
    typedef LRUCacheH4Value<K, V> Value;
    typedef std::unordered_map<K, Value> map;

private:
    Value* UpdateOrInsert(const K& key);
    Value* Update(typename map::iterator it);
    Value* Insert(const K& key);

private:
    void TryRemoveLRU();
    size_t ValueSize(const V& value) {
        // The size of K and V is will be calculated by sizeof_
        auto user_size = sizeof_(value);
        auto local_size = sizeof(LRUCacheH4Value<K, V>);
        return user_size + local_size - sizeof(value);
    }

private:
    map map_;
    Value* mru_;// The newest element
    Value* lru_;// The oldest element
    size_t max_size_;// The max element size
    size_t max_memory_size_;// The max memory size
    size_t memory_size_;// The current memory size

    S sizeof_;
    D deleter_;

private:
    LRUCacheH4(const LRUCacheH4& rhs);
    LRUCacheH4& operator=(const LRUCacheH4& rhs);
};


// Reserve enough space to avoid resizing later on and thus invalidate iterators
template<class K, class V, class S, class D>
LRUCacheH4<K, V, S, D>::LRUCacheH4(size_t maxsize, size_t maxmemsize, S f, D d)
    : mru_(NULL)
    , lru_(NULL)
    , max_size_(maxsize)
    , max_memory_size_(maxmemsize)
    , memory_size_(0)
    , sizeof_(f)
    , deleter_(d) {
    if (max_size_ == 0) {
        max_size_ = 1;
    }

    if (max_memory_size_ == 0) {
        max_memory_size_ = size_t(-1);
    }
}

template<class K, class V, class S, class D>
LRUCacheH4<K, V, S, D>::~LRUCacheH4() {
    for (const_iterator it = lru_begin(); it != end(); ++it) {
        deleter_(const_cast<V&>(it.value()));
    }
}

template<class K, class V, class S, class D>
V& LRUCacheH4<K, V, S, D>::operator[](const K& key) {
    return UpdateOrInsert(key)->value;
}

template<class K, class V, class S, class D>
void LRUCacheH4<K, V, S, D>::insert(const K& key, const V& value) {
    UpdateOrInsert(key)->value = value;
    memory_size_ += ValueSize(value);
}

// updates MRU
template<class K, class V, class S, class D>
typename LRUCacheH4<K, V, S, D>::const_iterator LRUCacheH4<K, V, S, D>::find(const K& key) {
    typename map::iterator it = map_.find(key);
    if (it != map_.end()) {
        return const_iterator(Update(it), const_iterator::kMRU2LRU);
    } else {
        return end();
    }
}

// does not update MRU
template<class K, class V, class S, class D>
typename LRUCacheH4<K, V, S, D>::const_iterator LRUCacheH4<K, V, S, D>::find(const K& key) const {
    typename map::iterator it = map_.find(key);
    if (it != map_.end()) {
        return const_iterator(&*it, const_iterator::kMRU2LRU);
    } else {
        return end();
    }
}

template<class K, class V, class S, class D>
void LRUCacheH4<K, V, S, D>::Dump(std::ostream& os, typename const_iterator::Direction dir) const {
    os << __FUNCTION__ << " LRUCacheH4(" << size() << "/" << max_size() << ") memory:(" << memory_size_ << "/" << max_memory_size_ << "): MRU --> LRU: " << std::endl;
    const_iterator it = mru_begin();
    if (dir == const_iterator::kLRU2MRU) {
        it = lru_begin();
    }
    for (; it != end(); ++it) {
        os << it.key() << ": " << it.value() << std::endl;
    }
}

template<class K, class V, class S, class D>
typename LRUCacheH4<K, V, S, D>::const_iterator LRUCacheH4<K, V, S, D>::mru_begin() const {
    return const_iterator(mru_, const_iterator::kMRU2LRU);
}

template<class K, class V, class S, class D>
typename LRUCacheH4<K, V, S, D>::const_iterator LRUCacheH4<K, V, S, D>::lru_begin() const {
    return const_iterator(lru_, const_iterator::kLRU2MRU);
}

template<class K, class V, class S, class D>
typename LRUCacheH4<K, V, S, D>::const_iterator LRUCacheH4<K, V, S, D>::end() const {
    return const_iterator();
}

template<class K, class V, class S, class D>
typename LRUCacheH4<K, V, S, D>::Value* LRUCacheH4<K, V, S, D>::UpdateOrInsert(const K& key) {
    typename map::iterator it = map_.find(key);
    if (it != map_.end()) {
        typename LRUCacheH4<K, V, S, D>::Value* moved = Update(it);
        assert(memory_size_ >= ValueSize(moved->value));
        memory_size_ -= ValueSize(moved->value);
        deleter_(moved->value);
        return moved;
    } else {
        return Insert(key);
    }
}

template<class K, class V, class S, class D>
typename LRUCacheH4<K, V, S, D>::Value* LRUCacheH4<K, V, S, D>::Update(typename map::iterator it) {
    LRUCacheH4Value<K, V>& v = it->second;
    Value* older = v.older;
    Value* newer = v.newer;
    Value* moved = &(it->second);

    // possibly update the LRU
    if (moved == lru_ && lru_->newer) {
        lru_ = lru_->newer;
    }

    if (moved != mru_) {
        // "remove" key from current position
        if (older) {
            older->newer = newer;
        }

        if (newer) {
            newer->older = older;
        }

        // "insert" key to MRU position
        v.older = mru_;
        v.newer = NULL;
        mru_->newer = moved;
        mru_ = moved;
    }

    return moved;
}


template<class K, class V, class S, class D>
typename LRUCacheH4<K, V, S, D>::Value* LRUCacheH4<K, V, S, D>::Insert(const K& key) {
    TryRemoveLRU();

    // insert key to MRU position
    Value val = Value(V(), mru_, NULL);
    std::pair<typename map::iterator, bool> ret = map_.insert(typename map::value_type(key, val));
    Value* inserted = &(ret.first->second);
    inserted->key = &(ret.first->first);

    if (mru_) {
        mru_->newer = inserted;
    }

    mru_ = inserted;

    // possibly update the LRU
    if (!lru_) {
        lru_ = mru_;
    } else if (!lru_->newer) {
        lru_->newer = mru_;
    }

    return inserted;
}


template<class K, class V, class S, class D>
void LRUCacheH4<K, V, S, D>::TryRemoveLRU() {
    // if we have grown too large, remove LRU
    while (map_.size() >= max_size_ || memory_size_ >= max_memory_size_) {
        if (map_.size() <= 1) {
            return;
        }

        Value* old_lru = lru_;

        if (lru_->newer) {
            lru_ = lru_->newer;
            lru_->older = NULL;
        }

        //std::cout << __FUNCTION__ << " item_count=" << map_.size() << " memory_size_=" << memory_size_ << "  ValueSize(old_lru->second.value)=" << ValueSize(old_lru->second.value) << " remove old key: " << old_lru->first <<  std::endl;
        memory_size_ -= long(ValueSize(old_lru->value));
        deleter_(old_lru->value);
        map_.erase(*(old_lru->key));
    }
}
}
