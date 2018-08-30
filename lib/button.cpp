////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include <gadget++/button.hpp>

////////////////////////////////////////////////////////////////////////////////
namespace gadget
{

////////////////////////////////////////////////////////////////////////////////
button::button(asio::io_context& io, gpio::pin* pin) :
    pin_(pin), state_(state_from_gpio(pin->state())), timer_(io)
{
    pin_->on_state_changed([&](gpio::state gs)
    {
        timer_.expires_from_now(time_);
        timer_.async_wait([&](const asio::error_code& ec)
        {
            if(ec) return;

            auto state = state_from_gpio(gs);
            if(state != state_) state_changed_.call(state_ = state);
        });
    });
}

////////////////////////////////////////////////////////////////////////////////
void button::on_state_changed(button::state_changed fn)
{
    state_changed_.add(std::move(fn));
}

////////////////////////////////////////////////////////////////////////////////
void button::on_pressed(button::state_pressed fn)
{
    on_state_changed([fn_ = std::move(fn)](gadget::state state)
        { if(state == gadget::pressed) fn_(); }
    );
}

////////////////////////////////////////////////////////////////////////////////
void button::on_released(button::state_released fn)
{
    on_state_changed([fn_ = std::move(fn)](gadget::state state)
        { if(state == gadget::released) fn_(); }
    );
}

////////////////////////////////////////////////////////////////////////////////
}
