////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef GADGET_UNIQUE_FUNCTION_HPP
#define GADGET_UNIQUE_FUNCTION_HPP

////////////////////////////////////////////////////////////////////////////////
#include <functional>
#include <utility>

////////////////////////////////////////////////////////////////////////////////
namespace gadget
{

////////////////////////////////////////////////////////////////////////////////
template<typename T>
class unique_function : public std::function<T>
{
    ////////////////////
    template<typename Fn>
    struct wrapper
    {
        wrapper(Fn&& f) : f_(std::move(f)) { }

        wrapper(const wrapper&) { throw 0; }
        wrapper& operator=(const wrapper&) { throw 0; }

        wrapper(wrapper&&) = default;
        wrapper& operator=(wrapper&&) = default;

        template<typename... Args>
        auto operator()(Args&&... args) { return f_(std::forward<Args>(args)...); }

        Fn f_;
    };

    template<typename U>
    using if_copyable = std::enable_if_t< std::is_copy_constructible<U>::value >;

    template<typename U>
    using if_movable = std::enable_if_t< !std::is_copy_constructible<U>::value
        && std::is_move_constructible<U>::value
    >;

    using base = std::function<T>;

public:
    ////////////////////
    unique_function() noexcept = default;
    unique_function(std::nullptr_t) noexcept : base(nullptr) { }

    template<typename Fn, typename = if_copyable<Fn>>
    unique_function(const Fn& f) : base(f) { }

    template<typename Fn, typename = if_movable<Fn>>
    unique_function(Fn&& f) : base(wrapper<Fn>(std::move(f))) { }

    unique_function(unique_function&&) = default;
    unique_function& operator=(unique_function&&) = default;

    unique_function& operator=(std::nullptr_t) { base::operator=(nullptr); return *this; }

    template<typename Fn, typename = if_copyable<Fn>>
    unique_function& operator=(const Fn& f) { base::operator=(f); return *this; }

    template<typename Fn, typename = if_movable<Fn>>
    unique_function& operator=(Fn&& f)
    { base::operator=(wrapper<Fn>(std::move(f))); return *this; }

    ////////////////////
    using base::operator();
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
