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
    template<typename Fn, typename En = void>
    struct wrap;

    // specialization for CopyConstructible Fn
    template<typename Fn>
    struct wrap<Fn, std::enable_if_t< std::is_copy_constructible<Fn>::value >>
    {
        template<typename... Args>
        auto operator()(Args&&... args) { return fn(std::forward<Args>(args)...); }

        Fn fn;
    };

    // specialization for MoveConstructible-only Fn
    template<typename Fn>
    struct wrap<Fn, std::enable_if_t< !std::is_copy_constructible<Fn>::value
        && std::is_move_constructible<Fn>::value >>
    {
        wrap(Fn&& fn) : fn(std::forward<Fn>(fn)) { }

        wrap(wrap&&) = default;
        wrap& operator=(wrap&&) = default;

        // these two functions are instantiated by std::function
        // and are never called;
        // hack to initialize non-DefaultContructible fn
        wrap(const wrap& rhs) : fn(const_cast<Fn&&>(rhs.fn)) { throw 0; }
        wrap& operator=(wrap&) { throw 0; }

        template<typename... Args>
        auto operator()(Args&&... args) { return fn(std::forward<Args>(args)...); }

        Fn fn;
    };

    ////////////////////
    using base = std::function<T>;

public:
    ////////////////////
    unique_function() noexcept = default;
    unique_function(std::nullptr_t) noexcept : base(nullptr) { }

    template<typename Fn>
    unique_function(Fn&& fn) : base(wrap<Fn>{ std::forward<Fn>(fn) }) { }

    unique_function(unique_function&&) = default;
    unique_function& operator=(unique_function&&) = default;

    unique_function& operator=(std::nullptr_t) { base::operator=(nullptr); return *this; }

    template<typename Fn>
    unique_function& operator=(Fn&& fn)
    { base::operator=(wrap<Fn>{ std::forward<Fn>(fn) }); return *this; }

    ////////////////////
    using base::operator();
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
