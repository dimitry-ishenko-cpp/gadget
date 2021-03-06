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

#include <asio/io_service.hpp>
#include <asio/system_timer.hpp>
#include <chrono>
#include <functional>

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

    ////////////////////
    using fn_tap = std::function<void()>;

    cid on_tap(fn_tap);
    cid on_tap_twice(fn_tap);
    cid on_tap_3x(fn_tap);

    cid on_hold(fn_tap);
    cid on_tap_hold(fn_tap);
    cid on_tap_twice_hold(fn_tap);

protected:
    ////////////////////
    asio::system_timer tap_timer_;
    nsec tap_time_ = 250ms;

    using fn_tap_ = std::function<void(int)>;
    call_chain<fn_tap_> tap_;

    asio::system_timer hold_timer_;
    nsec hold_time_ = 1200ms;

    using fn_hold_ = std::function<void(int)>;
    call_chain<fn_hold_> hold_;

    void state_changed(gpio::state);

    int press_ = 0;
    bool holding_ = false;

    ////////////////////
    void reset();
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
