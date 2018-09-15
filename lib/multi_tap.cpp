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
    hold_timer_(rhs.hold_timer_.get_io_service())
{
    move_and_reset(rhs);
}

////////////////////////////////////////////////////////////////////////////////
multi_tap& multi_tap::operator=(multi_tap&& rhs)
{
    reset();

    taps_ = 0;
    holding_ = false;

    move_and_reset(rhs);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
void multi_tap::operator()(contact_state state)
{
    if(state == pressed)
    {
        ++taps_;
        holding_ = true;

        tap_timer_.expires_from_now(tap_time_);
        tap_timer_.async_wait([=](const asio::error_code& ec)
        {
            if(ec || holding_) return;

                 if(taps_ == 1) once_();
            else if(taps_ == 2) twice_();
            else if(taps_ == 3) thrice_();

            taps_ = 0;
        });

        hold_timer_.expires_from_now(hold_time_);
        hold_timer_.async_wait([=](const asio::error_code& ec)
        {
            if(ec) return;

                 if(taps_ == 1) once_hold_();
            else if(taps_ == 2) twice_hold_();
            else if(taps_ == 3) thrice_hold_();

            taps_ = 0;
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
multi_tap& multi_tap::on_tap_once(fn_tap fn)
{
    once_.add(std::move(fn));
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
multi_tap& multi_tap::on_tap_once_hold(fn_tap fn)
{
    once_hold_.add(std::move(fn));
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
multi_tap& multi_tap::on_tap_twice(fn_tap fn)
{
    twice_.add(std::move(fn));
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
multi_tap& multi_tap::on_tap_twice_hold(fn_tap fn)
{
    twice_hold_.add(std::move(fn));
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
multi_tap& multi_tap::on_tap_thrice(fn_tap fn)
{
    thrice_.add(std::move(fn));
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
multi_tap& multi_tap::on_tap_thrice_hold(fn_tap fn)
{
    thrice_hold_.add(std::move(fn));
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
void multi_tap::reset()
{
    asio::error_code ec;
    tap_timer_.cancel(ec);
    hold_timer_.cancel(ec);
}

////////////////////////////////////////////////////////////////////////////////
void multi_tap::move_and_reset(multi_tap& rhs)
{
    tap_time_ = rhs.tap_time_;
    hold_time_ = rhs.hold_time_;

    once_ = std::move(rhs.once_);
    once_hold_ = std::move(rhs.once_hold_);

    twice_ = std::move(rhs.twice_);
    twice_hold_ = std::move(rhs.twice_hold_);

    thrice_ = std::move(rhs.thrice_);
    thrice_hold_ = std::move(rhs.thrice_hold_);

    rhs.reset();
}

////////////////////////////////////////////////////////////////////////////////
}
