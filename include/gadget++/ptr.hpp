////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef GADGET_PTR_HPP
#define GADGET_PTR_HPP

////////////////////////////////////////////////////////////////////////////////
#include <cstddef>
#include <utility>

////////////////////////////////////////////////////////////////////////////////
namespace gadget
{

////////////////////////////////////////////////////////////////////////////////
template<typename T>
class ptr
{
    T* p_ = nullptr;

public:
    ////////////////////
    constexpr ptr() noexcept = default;
    constexpr ptr(std::nullptr_t) noexcept { }

    template<typename U>
    ptr(U* p) noexcept : p_(p) { }

    ptr(const ptr& rhs) noexcept : p_(rhs.get()) { }
    ptr(ptr&& rhs) noexcept : p_(rhs.release()) { }

    ptr& operator=(const ptr& rhs) noexcept { reset(rhs.get()); return *this; }
    ptr& operator=(ptr&& rhs) noexcept { reset(rhs.release()); return *this; }

    template<typename U>
    ptr(const ptr<U>& rhs) noexcept : p_(rhs.get()) { }

    template<typename U>
    ptr(ptr<U>&& rhs) noexcept : p_(rhs.release()) { }

    template<class U>
    ptr& operator=(const ptr<U>& rhs) noexcept { reset(rhs.get()); return *this; }

    template<class U>
    ptr& operator=(ptr<U>&& rhs) noexcept { reset(rhs.release()); return *this; }

    ptr& operator=(std::nullptr_t) noexcept { reset(); return *this; }

    T* release() noexcept { T* p = p_; reset(); return p; }
    void reset(T* p = nullptr) noexcept { p_ = p; }
    void swap(ptr& rhs) noexcept { using std::swap; swap(p_, rhs.p_); }

    T* get() const noexcept { return p_; }
    explicit operator bool() const noexcept { return get() != nullptr; }

    T& operator*() const { return *get(); }
    T* operator->() const noexcept { return get(); }
};

////////////////////////////////////////////////////////////////////////////////
template <typename T>
inline void swap(ptr<T>& x, ptr<T>& y) noexcept { x.swap(y); }

template<typename T, typename U> bool operator==(const ptr<T>& x, const ptr<U>& y) noexcept { return x.get() == y.get(); }
template<typename T, typename U> bool operator!=(const ptr<T>& x, const ptr<U>& y) noexcept { return !(x == y); }
template<typename T, typename U> bool operator< (const ptr<T>& x, const ptr<U>& y) noexcept { return x.get() < y.get(); }
template<typename T, typename U> bool operator> (const ptr<T>& x, const ptr<U>& y) noexcept { return y < x; }
template<typename T, typename U> bool operator<=(const ptr<T>& x, const ptr<U>& y) noexcept { return !(y < x); }
template<typename T, typename U> bool operator>=(const ptr<T>& x, const ptr<U>& y) noexcept { return !(x < y); }

template<typename T> bool operator==(const ptr<T>& x, std::nullptr_t) noexcept { return  !x; }
template<typename T> bool operator==(std::nullptr_t, const ptr<T>& y) noexcept { return  !y; }
template<typename T> bool operator!=(const ptr<T>& x, std::nullptr_t) noexcept { return !!x; }
template<typename T> bool operator!=(std::nullptr_t, const ptr<T>& y) noexcept { return !!y; }
template<typename T> bool operator< (const ptr<T>& x, std::nullptr_t) noexcept { return x.get() < nullptr; }
template<typename T> bool operator< (std::nullptr_t, const ptr<T>& y) noexcept { return nullptr < y.get(); }
template<typename T> bool operator> (const ptr<T>& x, std::nullptr_t) noexcept { return nullptr < x; }
template<typename T> bool operator> (std::nullptr_t, const ptr<T>& y) noexcept { return y < nullptr; }
template<typename T> bool operator<=(const ptr<T>& x, std::nullptr_t) noexcept { return !(nullptr < x); }
template<typename T> bool operator<=(std::nullptr_t, const ptr<T>& y) noexcept { return !(y < nullptr); }
template<typename T> bool operator>=(const ptr<T>& x, std::nullptr_t) noexcept { return !(x < nullptr); }
template<typename T> bool operator>=(std::nullptr_t, const ptr<T>& y) noexcept { return !(nullptr < y); }

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
