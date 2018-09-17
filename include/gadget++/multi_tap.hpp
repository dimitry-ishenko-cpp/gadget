////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef GADGET_MULTI_TAP_HPP
#define GADGET_MULTI_TAP_HPP

////////////////////////////////////////////////////////////////////////////////
#include <gadget++/types.hpp>
#include <gadget++/unique_function.hpp>

#include <asio/io_service.hpp>
#include <asio/system_timer.hpp>
#include <functional>
#include <memory>

////////////////////////////////////////////////////////////////////////////////
namespace gadget
{

////////////////////////////////////////////////////////////////////////////////
class multi_tap
{
public:
    ////////////////////
    explicit multi_tap(asio::io_service&);

    multi_tap(multi_tap&&);
    multi_tap& operator=(multi_tap&&);

    template<typename Rep, typename Period>
    void tap_time(std::chrono::duration<Rep, Period> time)
    {
        tap_time_ = std::chrono::duration_cast<nsec>(time);
    }
    auto tap_time() const noexcept { return tap_time_; }

    template<typename Rep, typename Period>
    void hold_time(std::chrono::duration<Rep, Period> time)
    {
        hold_time_ = std::chrono::duration_cast<nsec>(time);
    }
    auto hold_time() const noexcept { return hold_time_; }

    void operator()(contact_state);

    ////////////////////
    using fn_tap = unique_function<void()>;

    multi_tap& on_tap_once(fn_tap);
    multi_tap& on_tap_once_hold(fn_tap);

    multi_tap& on_tap_twice(fn_tap);
    multi_tap& on_tap_twice_hold(fn_tap);

    multi_tap& on_tap_thrice(fn_tap);
    multi_tap& on_tap_thrice_hold(fn_tap);

protected:
    ////////////////////
    asio::system_timer tap_timer_, hold_timer_;
    nsec tap_time_ = 200ms, hold_time_ = 2s;
    int taps_ = 0;
    bool holding_ = false;

    call_chain<fn_tap> once_, once_hold_;
    call_chain<fn_tap> twice_, twice_hold_;
    call_chain<fn_tap> thrice_, thrice_hold_;

    ////////////////////
    void reset();
    void move_and_reset(multi_tap&);
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
