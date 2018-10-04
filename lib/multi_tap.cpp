////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include <gadget++/multi_tap.hpp>

////////////////////////////////////////////////////////////////////////////////
namespace gadget
{

////////////////////////////////////////////////////////////////////////////////
multi_tap::multi_tap(asio::io_service& io) :
    tap_timer_(io), hold_timer_(io)
{ }

////////////////////////////////////////////////////////////////////////////////
multi_tap::multi_tap(multi_tap&& rhs) :
    tap_timer_(rhs.tap_timer_.get_io_service()),
    tap_time_(rhs.tap_time_),
    hold_timer_(rhs.hold_timer_.get_io_service()),
    hold_time_(rhs.hold_time_)
{
    rhs.reset();
}

////////////////////////////////////////////////////////////////////////////////
multi_tap& multi_tap::operator=(multi_tap&& rhs)
{
    reset();

    tap_time_ = rhs.tap_time_;
    tap_ = std::move(rhs.tap_);
    hold_time_ = rhs.hold_time_;
    hold_ = std::move(rhs.hold_);
    press_ = 0;
    holding_ = false;

    rhs.reset();
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
cid multi_tap::on_tap(fn_tap fn)
{
    return tap_.add([fn_ = std::move(fn)](int press){ if(press == 1) fn_(); });
}

////////////////////////////////////////////////////////////////////////////////
cid multi_tap::on_tap_twice(fn_tap fn)
{
    return tap_.add([fn_ = std::move(fn)](int press){ if(press == 2) fn_(); });
}

////////////////////////////////////////////////////////////////////////////////
cid multi_tap::on_tap_3x(fn_tap fn)
{
    return tap_.add([fn_ = std::move(fn)](int press){ if(press == 3) fn_(); });
}

////////////////////////////////////////////////////////////////////////////////
cid multi_tap::on_hold(fn_tap fn)
{
    return hold_.add([fn_ = std::move(fn)](int press){ if(press == 1) fn_(); });
}

////////////////////////////////////////////////////////////////////////////////
cid multi_tap::on_tap_hold(fn_tap fn)
{
    return hold_.add([fn_ = std::move(fn)](int press){ if(press == 2) fn_(); });
}

////////////////////////////////////////////////////////////////////////////////
cid multi_tap::on_tap_twice_hold(fn_tap fn)
{
    return hold_.add([fn_ = std::move(fn)](int press){ if(press == 3) fn_(); });
}

////////////////////////////////////////////////////////////////////////////////
void multi_tap::state_changed(gpio::state state)
{
    if(state == off)
    {
        ++press_;
        holding_ = true;

        tap_timer_.expires_from_now(tap_time_);
        tap_timer_.async_wait([=](const asio::error_code& ec)
        {
            if(ec || holding_) return;

            tap_(press_);
            press_ = 0;
        });

        hold_timer_.expires_from_now(hold_time_);
        hold_timer_.async_wait([=](const asio::error_code& ec)
        {
            if(ec) return;

            hold_(press_);
            press_ = 0;
        });
    }
    else
    {
        asio::error_code ec;
        hold_timer_.cancel(ec);
        holding_ = false;
    }
}

////////////////////////////////////////////////////////////////////////////////
void multi_tap::reset()
{
    asio::error_code ec;
    tap_timer_.cancel(ec);
    hold_timer_.cancel(ec);
}

////////////////////////////////////////////////////////////////////////////////
}
