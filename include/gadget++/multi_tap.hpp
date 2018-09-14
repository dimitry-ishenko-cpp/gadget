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
#include <functional>

////////////////////////////////////////////////////////////////////////////////
namespace gadget
{

////////////////////////////////////////////////////////////////////////////////
class multi_tap
{
public:
    ////////////////////
    multi_tap(asio::io_service&);

    template<typename Rep, typename Period>
    void tap_time(std::chrono::duration<Rep, Period> time)
    {
        tap_time_ = std::chrono::duration_cast<nsec>(time);
    }
    auto tap_time() const noexcept { return tap_time_; }

    template<typename Rep, typename Period>
    void long_time(std::chrono::duration<Rep, Period> time)
    {
        long_time_ = std::chrono::duration_cast<nsec>(time);
    }
    auto long_time() const noexcept { return long_time_; }

    void operator()(contact_state);

    ////////////////////
    using tapped = std::function<void()>;

    multi_tap& on_tapped_once(tapped);
    multi_tap& on_tapped_twice(tapped);
    multi_tap& on_tapped_thrice(tapped);

    multi_tap& on_long_tapped_once(tapped);
    multi_tap& on_long_tapped_twice(tapped);
    multi_tap& on_long_tapped_thrice(tapped);

protected:
    ////////////////////
    asio::system_timer tap_timer_, long_timer_;
    nsec tap_time_ = 200ms, long_time_ = 2s;
    int taps_ = 0;
    bool tapped_ = false;

    call_chain<tapped> once_, twice_, thrice_;
    call_chain<tapped> long_once_, long_twice_, long_thrice_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
