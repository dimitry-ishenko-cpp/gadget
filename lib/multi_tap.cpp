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
    tap_timer_(io), long_timer_(io)
{ }

////////////////////////////////////////////////////////////////////////////////
void multi_tap::operator()(contact_state state)
{
    if(state == pressed)
    {
        ++taps_;
        tapped_ = true;

        tap_timer_.expires_from_now(tap_time_);
        tap_timer_.async_wait([=](const asio::error_code& ec)
        {
            if(ec || tapped_) return; // long tap

                 if(taps_ == 1) once_();
            else if(taps_ == 2) twice_();
            else if(taps_ == 3) thrice_();

            taps_ = 0;
        });

        long_timer_.expires_from_now(long_time_);
        long_timer_.async_wait([=](const asio::error_code& ec)
        {
            if(ec) return;

                 if(taps_ == 1) long_once_();
            else if(taps_ == 2) long_twice_();
            else if(taps_ == 3) long_thrice_();

            taps_ = 0;
        });
    }
    else
    {
        asio::error_code ec;
        long_timer_.cancel(ec);
        tapped_ = false;
    }

}

////////////////////////////////////////////////////////////////////////////////
multi_tap& multi_tap::on_tapped_once(tapped fn)
{
    once_.add(std::move(fn));
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
multi_tap& multi_tap::on_tapped_twice(tapped fn)
{
    twice_.add(std::move(fn));
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
multi_tap& multi_tap::on_tapped_thrice(tapped fn)
{
    thrice_.add(std::move(fn));
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
multi_tap& multi_tap::on_long_tapped_once(tapped fn)
{
    long_once_.add(std::move(fn));
    return *this;
}

multi_tap& multi_tap::on_long_tapped_twice(tapped fn)
{
    long_twice_.add(std::move(fn));
    return *this;

}

multi_tap& multi_tap::on_long_tapped_thrice(tapped fn)
{
    long_thrice_.add(std::move(fn));
    return *this;

}

////////////////////////////////////////////////////////////////////////////////
}
