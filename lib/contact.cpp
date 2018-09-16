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
    pin_(pin), timer_(io)
{
    set_callback();
}

////////////////////////////////////////////////////////////////////////////////
contact::~contact() { reset(); }

////////////////////////////////////////////////////////////////////////////////
contact::contact(contact&& rhs) :
    contact(rhs.timer_.get_io_service(), rhs.pin_)
{
    move_and_reset(rhs);
}

////////////////////////////////////////////////////////////////////////////////
contact& contact::operator=(contact&& rhs)
{
    reset();

    pin_ = rhs.pin_;
    state_ = nos;
    set_callback();

    move_and_reset(rhs);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
cid contact::on_state_changed(fn_state_changed fn)
{
    return state_changed_.add(std::move(fn));
}

////////////////////////////////////////////////////////////////////////////////
cid contact::on_press(fn_press fn)
{
    return press_.add(std::move(fn));
}

////////////////////////////////////////////////////////////////////////////////
cid contact::on_release(fn_release fn)
{
    return release_.add(std::move(fn));
}

////////////////////////////////////////////////////////////////////////////////
bool contact::remove_call(cid id)
{
    return state_changed_.remove(id)
        || press_.remove(id)
        || release_.remove(id);
}

////////////////////////////////////////////////////////////////////////////////
void contact::set_callback()
{
    if(pin_) id_ = pin_->on_state_changed([=](gpio::state state)
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

                     if(state_ == pressed) press_();
                else if(state_ == released) release_();
            }
        });
    });
}

////////////////////////////////////////////////////////////////////////////////
void contact::reset()
{
    asio::error_code ec;
    timer_.cancel(ec);

    if(pin_)
    {
        pin_->remove(id_);
        pin_ = nullptr;
    }
}

////////////////////////////////////////////////////////////////////////////////
void contact::move_and_reset(contact& rhs)
{
    time_ = rhs.time_;

    state_changed_ = std::move(rhs.state_changed_);
    press_ = std::move(rhs.press_);
    release_ = std::move(rhs.release_);

    rhs.reset();
}

////////////////////////////////////////////////////////////////////////////////
}
