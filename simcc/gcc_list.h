#pragma once

#include "simcc/platform_config.h"

#include <list>

namespace simcc {

    // A list with the same interface with std::list.
    // For gcc compiler, it has higher performance for list::size()

#ifdef H_OS_WINDOWS
template < typename T >
class list : public std::list<T> {};
#else
template < typename _Tp >
class list : private std::list<_Tp > {
    typedef std::list<_Tp > BaseClass;
    size_t m_list_size;
public:
    // type
    typedef typename BaseClass::allocator_type          allocator_type;
    typedef typename BaseClass::const_iterator          const_iterator;
    typedef typename BaseClass::const_pointer           const_pointer;
    typedef typename BaseClass::const_reference         const_reference;
    typedef typename BaseClass::const_reverse_iterator  const_reverse_iterator;
    typedef typename BaseClass::difference_type         difference_type;
    typedef typename BaseClass::iterator                iterator;
    typedef typename BaseClass::pointer                 pointer;
    typedef typename BaseClass::reference               reference;
    typedef typename BaseClass::reverse_iterator        reverse_iterator;
    typedef typename BaseClass::size_type               size_type;
    typedef typename BaseClass::value_type              value_type;

    // member function
    using BaseClass::front;
    using BaseClass::back;
    using BaseClass::begin;
    using BaseClass::end;
    using BaseClass::empty;
    using BaseClass::rbegin;
    using BaseClass::rend;
    using BaseClass::get_allocator;
    using BaseClass::max_size;
    using BaseClass::reverse;
    using BaseClass::sort;
public:
    /**
    *  Erases all the elements.  Note that this function only erases
    *  the elements, and that if the elements themselves are
    *  pointers, the pointed-to memory is not touched in any way.
    *  Managing the pointer is the user's responsibility.
    */
    void clear() {
        m_list_size = 0;
        BaseClass::clear();
    }


    /**
    *  @brief  Removes first element.
    *
    *  This is a typical stack operation.  It shrinks the %list by
    *  one.  Due to the nature of a %list this operation can be done
    *  in constant time, and only invalidates iterators/references to
    *  the element being removed.
    *
    *  Note that no data is returned, and if the first element's data
    *  is needed, it should be retrieved before pop_front() is
    *  called.
    */
    void pop_front() {
        erase(begin());
    }
    /**
    *  @brief  Removes last element.
    *
    *  This is a typical stack operation.  It shrinks the %list by
    *  one.  Due to the nature of a %list this operation can be done
    *  in constant time, and only invalidates iterators/references to
    *  the element being removed.
    *
    *  Note that no data is returned, and if the last element's data
    *  is needed, it should be retrieved before pop_back() is called.
    */
    void pop_back() {
        iterator __tmp = end();
        erase(--__tmp);
    }
    /**
    *  @brief  Remove element at given position.
    *  @param  position  Iterator pointing to element to be erased.
    *  @return  An iterator pointing to the next element (or end()).
    *
    *  This function will erase the element at the given position and thus
    *  shorten the %list by one.
    *
    *  Due to the nature of a %list this operation can be done in
    *  constant time, and only invalidates iterators/references to
    *  the element being removed.  The user is also cautioned that
    *  this function only erases the element, and that if the element
    *  is itself a pointer, the pointed-to memory is not touched in
    *  any way.  Managing the pointer is the user's responsibilty.
    */
    iterator erase(iterator __position) {
        m_list_size --;
        return BaseClass::erase(__position);
    }
    /**
    *  @brief  Remove a range of elements.
    *  @param  first  Iterator pointing to the first element to be erased.
    *  @param  last  Iterator pointing to one past the last element to be
    *                erased.
    *  @return  An iterator pointing to the element pointed to by @a last
    *           prior to erasing (or end()).
    *
    *  This function will erase the elements in the range @a
    *  [first,last) and shorten the %list accordingly.
    *
    *  Due to the nature of a %list this operation can be done in
    *  constant time, and only invalidates iterators/references to
    *  the element being removed.  The user is also cautioned that
    *  this function only erases the elements, and that if the
    *  elements themselves are pointers, the pointed-to memory is not
    *  touched in any way.  Managing the pointer is the user's
    *  responsibilty.
    */
    iterator erase(iterator __first, iterator __last) {
        while (__first != __last) {
            erase(__first++);
        }

        return __last;
    }

    void resize(size_type __new_size) {
        this->resize(__new_size, _Tp());
        m_list_size = __new_size ;
    }
    /**
    *  @brief Resizes the %list to the specified number of elements.
    *  @param new_size Number of elements the %list should contain.
    *  @param x Data with which new elements should be populated.
    *
    *  This function will %resize the %list to the specified number
    *  of elements.  If the number is smaller than the %list's
    *  current size the %list is truncated, otherwise the %list is
    *  extended and new elements are populated with given data.
    */
    void resize(size_type __new_size, const _Tp& __x) {
        iterator __i = begin();
        size_type __len = 0;

        for (; __i != end() && __len < __new_size; ++__i, ++__len)
            ;

        if (__len == __new_size) {
            erase(__i, end());
        } else {                      // __i == end()
            insert(end(), __new_size - __len, __x);
        }
    }
    /**
    *  @brief  Inserts given value into %list before specified iterator.
    *  @param  position  An iterator into the %list.
    *  @param  x  Data to be inserted.
    *  @return  An iterator that points to the inserted data.
    *
    *  This function will insert a copy of the given value before
    *  the specified location.  Due to the nature of a %list this
    *  operation can be done in constant time, and does not
    *  invalidate iterators and references.
    */
    iterator insert(iterator __position, const _Tp& __x) {
        m_list_size++;
        return BaseClass::insert(__position , __x);
    }

    /**
    *  @brief  Inserts a number of copies of given data into the %list.
    *  @param  position  An iterator into the %list.
    *  @param  n  Number of elements to be inserted.
    *  @param  x  Data to be inserted.
    *
    *  This function will insert a specified number of copies of the
    *  given data before the location specified by @a position.
    *
    *  Due to the nature of a %list this operation can be done in
    *  constant time, and does not invalidate iterators and
    *  references.
    */
    void
    insert(iterator __position, size_type __n, const value_type& __x) {
        m_list_size += __n;
        return BaseClass::insert(__position , __n , __x);
    }


    /**
    *  @brief  Inserts a range into the %list.
    *  @param  position  An iterator into the %list.
    *  @param  first  An input iterator.
    *  @param  last   An input iterator.
    *
    *  This function will insert copies of the data in the range [@a
    *  first,@a last) into the %list before the location specified by
    *  @a position.
    *
    *  Due to the nature of a %list this operation can be done in
    *  constant time, and does not invalidate iterators and
    *  references.
    */
    template <class _InputIterator>
    void insert(iterator __pos, _InputIterator __first, _InputIterator __last) {
        m_list_size += std::distance(__first , __last);
        BaseClass::insert(__pos , __first, __last);
    }
    /**  Returns the number of elements in the %list.  */
    size_type size() const {
        return m_list_size;
    }
    /**
    *  @brief  Add data to the end of the %list.
    *  @param  x  Data to be added.
    *
    *  This is a typical stack operation.  The function creates an
    *  element at the end of the %list and assigns the given data to
    *  it.  Due to the nature of a %list this operation can be done
    *  in constant time, and does not invalidate iterators and
    *  references.
    */
    void push_back(const _Tp& __x) {
        insert(end(), __x);
    }
    /**
    *  @brief  Add data to the front of the %list.
    *  @param  x  Data to be added.
    *
    *  This is a typical stack operation.  The function creates an
    *  element at the front of the %list and assigns the given data
    *  to it.  Due to the nature of a %list this operation can be
    *  done in constant time, and does not invalidate iterators and
    *  references.
    */
    void push_front(const _Tp& __x) {
        insert(begin(), __x);
    }
    /**
    *  @brief  Remove all elements equal to value.
    *  @param  value  The value to remove.
    *
    *  Removes every element in the list equal to @a value.
    *  Remaining elements stay in list order.  Note that this
    *  function only erases the elements, and that if the elements
    *  themselves are pointers, the pointed-to memory is not
    *  touched in any way.  Managing the pointer is the user's
    *  responsibilty.
    */
    void remove(const _Tp& value) {
        iterator __first = begin();
        iterator __last = end();

        while (__first != __last) {
            iterator __next = __first;
            ++__next;

            if (*__first == value) {
                erase(__first);
            }

            __first = __next;
        }
    }
    /**
    *  @brief  Remove consecutive duplicate elements.
    *
    *  For each consecutive set of elements with the same value,
    *  remove all but the first one.  Remaining elements stay in
    *  list order.  Note that this function only erases the
    *  elements, and that if the elements themselves are pointers,
    *  the pointed-to memory is not touched in any way.  Managing
    *  the pointer is the user's responsibilty.
    */
    void unique() {
        iterator __first = begin();
        iterator __last = end();

        if (__first == __last) {
            return;
        }

        iterator __next = __first;

        while (++__next != __last) {
            if (*__first == *__next) {
                erase(__next);
            } else {
                __first = __next;
            }

            __next = __first;
        }
    }
    /**
    *  @brief  Remove consecutive elements satisfying a predicate.
    *  @param  BinaryPredicate  Binary predicate function or object.
    *
    *  For each consecutive set of elements [first,last) that
    *  satisfy predicate(first,i) where i is an iterator in
    *  [first,last), remove all but the first one.  Remaining
    *  elements stay in list order.  Note that this function only
    *  erases the elements, and that if the elements themselves are
    *  pointers, the pointed-to memory is not touched in any way.
    *  Managing the pointer is the user's responsibilty.
    */
    template <class _BinaryPredicate>
    void unique(_BinaryPredicate __binary_pred) {
        iterator __first = begin();
        iterator __last = end();

        if (__first == __last) {
            return;
        }

        iterator __next = __first;

        while (++__next != __last) {
            if (__binary_pred(*__first, *__next)) {
                erase(__next);
            } else {
                __first = __next;
            }

            __next = __first;
        }
    }

    /**
    *  @brief  Remove all elements satisfying a predicate.
    *  @param  Predicate  Unary predicate function or object.
    *
    *  Removes every element in the list for which the predicate
    *  returns true.  Remaining elements stay in list order.  Note
    *  that this function only erases the elements, and that if the
    *  elements themselves are pointers, the pointed-to memory is
    *  not touched in any way.  Managing the pointer is the user's
    *  responsibilty.
    */
    template <class _Predicate>
    void remove_if(_Predicate __pred) {
        iterator __first = begin();
        iterator __last = end();

        while (__first != __last) {
            iterator __next = __first;
            ++__next;

            if (__pred(*__first)) {
                erase(__first);
            }

            __first = __next;
        }
    }

    /**
    *  @brief  Assigns a given value to a %list.
    *  @param  n  Number of elements to be assigned.
    *  @param  val  Value to be assigned.
    *
    *  This function fills a %list with @a n copies of the given
    *  value.  Note that the assignment completely changes the %list
    *  and that the resulting %list's size is the same as the number
    *  of elements assigned.  Old data may be lost.
    */
    void assign(size_type __n, const _Tp& __val) {
        iterator __i = begin();

        for (; __i != end() && __n > 0; ++__i, --__n) {
            *__i = __val;
        }

        if (__n > 0) {
            insert(end(), __n, __val);
        } else {
            erase(__i, end());
        }
    }

    /**
    *  @brief  Assigns a range to a %list.
    *  @param  first  An input iterator.
    *  @param  last   An input iterator.
    *
    *  This function fills a %list with copies of the elements in the
    *  range [@a first,@a last).
    *
    *  Note that the assignment completely changes the %list and
    *  that the resulting %list's size is the same as the number of
    *  elements assigned.  Old data may be lost.
    */
    template <class _InputIterator>
    void assign(_InputIterator __first2, _InputIterator __last2) {
        iterator __first1 = begin();
        iterator __last1 = end();

        for (; __first1 != __last1 && __first2 != __last2; ++__first1, ++__first2) {
            *__first1 = *__first2;
        }

        if (__first2 == __last2) {
            erase(__first1, __last1);
        } else {
            insert(__last1, __first2, __last2);
        }
    }
    /**
    *  @brief  Swaps data with another %list.
    *  @param  x  A %list of the same element and allocator types.
    *
    *  This exchanges the elements between two lists in constant
    *  time.  Note that the global std::swap() function is
    *  specialized such that std::swap(l1,l2) will feed to this
    *  function.
    */
    void swap(list<_Tp>&   __x) {
        std::swap(m_list_size , __x.m_list_size);
        BaseClass::swap(__x);
    }

    /**
    *  @brief  Default constructor creates no elements.
    */
    explicit list(const allocator_type& _Al = allocator_type())
        : BaseClass(_Al)
        , m_list_size(0) {
    }


    /**
    *  @brief  Create a %list with copies of an exemplar element.
    *  @param  n  The number of elements to initially create.
    *  @param  value  An element to copy.
    *
    *  This constructor fills the %list with @a n copies of @a value.
    */
    list(size_type _Count, const _Tp& _Val, const allocator_type& _Al = allocator_type())
        : BaseClass(_Count , _Val , _Al)
        , m_list_size(_Count) {
    }

    /**
    *  @brief  %list copy constructor.
    *  @param  x  A %list of identical element and allocator types.
    *
    *  The newly-created %list uses a copy of the allocation object used
    *  by @a x.
    */
    list(const list& __x)
        : BaseClass(__x.get_allocator())
        , m_list_size(0) {
        this->insert(begin(), __x.begin(), __x.end());
    }



    /**
    *  @brief  Builds a %list from a range.
    *  @param  first  An input iterator.
    *  @param  last  An input iterator.
    *
    *  Create a %list consisting of copies of the elements from
    *  [@a first,@a last).  This is linear in N (where N is
    *  distance(@a first,@a last)).
    *
    *  @if maint
    *  We don't need any dispatching tricks here, because insert does all of
    *  that anyway.
    *  @endif
    */
    template<class InputIterator >
    list(InputIterator _First, InputIterator _Last, const allocator_type& _Al = allocator_type())
        : m_list_size(0)
        , BaseClass(_Al) {
        this->insert(begin(), _First, _Last);
    }

    /**
    *  @brief  %list assignment operator.
    *  @param  x  A %list of identical element and allocator types.
    *
    *  All the elements of @a x are copied, but unlike the copy
    *  constructor, the allocator object is not copied.
    */
    list& operator=(const list& __x) {
        assign(__x.begin() , __x.end());
        return *this;
    }
    /**
    *  @brief  Insert contents of another %list.
    *  @param  position  Iterator referencing the element to insert before.
    *  @param  x  Source list.
    *
    *  The elements of @a x are inserted in constant time in front of
    *  the element referenced by @a position.  @a x becomes an empty
    *  list.
    */
    void splice(iterator __position, list& __x) {
        if (!__x.empty()) {
            this->m_list_size += __x.size();
            __x.m_list_size = 0;
            BaseClass::splice(__position , __x);
        }
    }
    /**
    *  @brief  Insert element from another %list.
    *  @param  position  Iterator referencing the element to insert before.
    *  @param  x  Source list.
    *  @param  i  Iterator referencing the element to move.
    *
    *  Removes the element in list @a x referenced by @a i and
    *  inserts it into the current list before @a position.
    */
    void splice(iterator __position, list& __x, iterator __i) {
        if (__i == __x.end()) {
            return;
        }

        this->m_list_size++;
        __x.m_list_size--;
        BaseClass::splice(__position , __x , __i);
    }
    /**
    *  @brief  Insert range from another %list.
    *  @param  position  Iterator referencing the element to insert before.
    *  @param  x  Source list.
    *  @param  first  Iterator referencing the start of range in x.
    *  @param  last  Iterator referencing the end of range in x.
    *
    *  Removes elements in the range [first,last) and inserts them
    *  before @a position in constant time.
    *
    *  Undefined if @a position is in [first,last).
    */
    void splice(iterator __position, list& __x , iterator __first, iterator __last) {
        if (__first != __last) {
            const difference_type nSize = distance(__first , __last);
            this->m_list_size += nSize;
            __x.m_list_size -= nSize;
            BaseClass::splice(__position , __x , __first , __last);
        }
    }
};
#endif



}

