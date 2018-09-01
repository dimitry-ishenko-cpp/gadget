////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include <gadget++/button.hpp>
#include <utility>

////////////////////////////////////////////////////////////////////////////////
namespace gadget
{

////////////////////////////////////////////////////////////////////////////////
button::button(asio::io_service& io, gpio::pin* pin) :
    pin_(pin), state_(state_from_gpio(pin->state())), timer_(io)
{
    pin_->on_state_changed([=](gpio::state gs)
    {
        timer_.expires_from_now(time_);
        timer_.async_wait([=](const asio::error_code& ec)
        {
            if(ec) return;

            auto state = state_from_gpio(gs);
            if(state != state_)
            {
                state_ = state;
                state_changed_.call(state_);
            }
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
    on_state_changed([fn_ = std::move(fn)](button_state state)
        { if(state == pressed) fn_(); }
    );
}

////////////////////////////////////////////////////////////////////////////////
void button::on_released(button::state_released fn)
{
    on_state_changed([fn_ = std::move(fn)](button_state state)
        { if(state == released) fn_(); }
    );
}

////////////////////////////////////////////////////////////////////////////////
}
