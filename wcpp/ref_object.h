#pragma once

#include "wcpp/inner_pre.h"

#include <atomic>
#include <algorithm>

namespace wcpp {

// Interface provides referencing counted function. 
class WCPP_EXPORT RefCountable {
public:
    // Constructor & Destructor.
    RefCountable() {};
    virtual ~RefCountable() {};

    // Increments the reference count
    virtual void Ref() const = 0;

    // Decrements the reference count
    virtual void Release() const = 0;

    // Gets the reference count
    virtual int RefCount() const = 0;
};

class WCPP_EXPORT RefObject : public RefCountable {
public:
    RefObject(): ref_count_(0) {}

    virtual ~RefObject() {}

    // Increase the reference count by 1
    virtual void Ref() const;

    // Decrease the reference count by 1
    virtual void Release() const;

    // Get the reference count
    int RefCount() const {
        return ref_count_;
    }

protected:
    mutable std::atomic<int> ref_count_;
};

// Pointer class
template<class T>
class RefPtr {
public:
    typedef T object_type;
protected:
    T* ptr_;
public:
    RefPtr(): ptr_(0) {}

    RefPtr(object_type* rep) {
        ptr_ = rep;
        if (ptr_) {
            ptr_->Ref();
        }
    }

    // Get Pointer from other type
    template< class U >
    RefPtr(U* rep) {
        ptr_ = dynamic_cast<object_type*>(rep);
        if (ptr_) {
            ptr_->Ref();
        }
    }

    RefPtr(const RefPtr& r) {
        ptr_ = r.get();
        if (ptr_) {
            ptr_->Ref();
        }
    }

    template< class U >
    RefPtr(const RefPtr<U>& r) {
        ptr_ = dynamic_cast<object_type*>(r.get());
        if (ptr_) {
            ptr_->Ref();
        }
    }

    RefPtr& operator=(const RefPtr& r) {
        Bind(r.get());
        return *this;
    }

    // assigner
    RefPtr& operator=(object_type* rep) {
        Bind(rep);
        return *this;
    }

    // assigner
    template< class U >
    RefPtr& operator=(const RefPtr<U>& r) {
        Bind(dynamic_cast<object_type*>(r.getPointer()));
        return *this;
    }

    ~RefPtr() {
        SetNull();
    }

    operator object_type* ()const {
        return ptr_;
    }
    object_type& operator*() const {
        assert(ptr_);
        return *ptr_;
    }
    object_type* operator->() const {
        assert(ptr_);
        return ptr_;
    }

    bool IsNull(void) const {
        return ptr_ == 0;
    }

    // Bind rep to the RefPtr
    void Bind(object_type* rep) {
        if (rep) {
            rep->Ref();
        }
        SetNull();
        ptr_ = rep;
    }

    // Clear reference
    void SetNull() {
        if (ptr_) {
            T* thiz = ptr_;
            ptr_ = NULL;
            thiz->Release();
        }
    }

    void Swap(wcpp::RefPtr<T>& r) {
        std::swap(this->ptr_ , r.ptr_);
    }

    // The same interface with std::shared_ptr
    object_type* get() const { return ptr_; }
    int use_count() const;
    void reset(object_type* rep = NULL);
};

template<class T, class U>
inline bool operator==(RefPtr<T> const& a, RefPtr<U> const& b) {
    return a.get() == b.get();
}

template<class T, class U>
inline bool operator!=(RefPtr<T> const& a, RefPtr<U> const& b) {
    return a.get() != b.get();
}

template<class T>
inline int RefPtr<T>::use_count() const {
    if (ptr_) {
        return ptr_->RefCount();
    }
    return 0;
}

template<class T>
inline void RefPtr<T>::reset(object_type* rep /*= NULL*/) {
    Bind(rep);
}

inline void RefObject::Ref() const {
    ++ref_count_;
}

inline void RefObject::Release() const {
    if (--ref_count_ <= 0) {
        delete this;
    }
}

typedef RefPtr<RefCountable> RefPointer;
typedef RefPtr<RefObject> RefObjectPtr;

}



#ifndef H_REFERENCE_METHODS
#define H_REFERENCE_METHODS( ConcreteClass )                                \
    public:                                                                 \
        virtual void Ref() const      { return ConcreteClass::Ref(); }      \
        virtual void Release() const  { return ConcreteClass::Release(); }  \
        virtual int  RefCount() const { return ConcreteClass::RefCount(); }
#endif

namespace std {
template<class T>
void swap(wcpp::RefPtr<T>& l , wcpp::RefPtr<T>& r) {
    return l.Swap(r);
}
}


