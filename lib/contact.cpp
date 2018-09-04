////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include <gadget++/contact.hpp>
#include <utility>

////////////////////////////////////////////////////////////////////////////////
namespace gadget
{

////////////////////////////////////////////////////////////////////////////////
contact::contact(asio::io_service& io, gpio::pin* pin) :
    gadget_base(pin), state_(to_contact_state(pin->state())), timer_(io)
{
    register_callback();
}

////////////////////////////////////////////////////////////////////////////////
cid contact::on_state_changed(contact::state_changed fn)
{
    return state_changed_.add(std::move(fn));
}

////////////////////////////////////////////////////////////////////////////////
cid contact::on_pressed(contact::state_pressed fn)
{
    return on_state_changed(
        [fn_ = std::move(fn)](contact_state state)
        { if(state == pressed) fn_(); }
    );
}

////////////////////////////////////////////////////////////////////////////////
cid contact::on_released(contact::state_released fn)
{
    return on_state_changed(
        [fn_ = std::move(fn)](contact_state state)
        { if(state == released) fn_(); }
    );
}

////////////////////////////////////////////////////////////////////////////////
bool contact::remove_call(cid id) { return state_changed_.remove(id); }

////////////////////////////////////////////////////////////////////////////////
void contact::register_callback()
{
    reset_callback(pin_->on_state_changed([=](gpio::state state)
    {
        timer_.expires_from_now(time_);
        timer_.async_wait([=](const asio::error_code& ec)
        {
            if(ec) return;

            auto new_state = to_contact_state(state);
            if(new_state != state_)
            {
                state_ = new_state;
                state_changed_(state_);
            }
        });
    }));
}

////////////////////////////////////////////////////////////////////////////////
}
