////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include <gadget++/contact.hpp>

#include <stdexcept>
#include <utility>

////////////////////////////////////////////////////////////////////////////////
namespace gadget
{

////////////////////////////////////////////////////////////////////////////////
contact::contact(asio::io_service& io, gpio::pin* pin, flag flags) :
    contact(io, pin, adopt_pin)
{
    if(pin_) pin_->mode(in, flags);
}

////////////////////////////////////////////////////////////////////////////////
contact::contact(asio::io_service& io, gpio::pin* pin, adopt_pin_t) :
    multi_tap(io), pin_(pin), timer_(io)
{
    if(pin_) set_call();
}

////////////////////////////////////////////////////////////////////////////////
contact::~contact() { reset(); }

////////////////////////////////////////////////////////////////////////////////
contact::contact(contact&& rhs) :
    multi_tap(std::move(rhs)),
    pin_(rhs.pin_),
    time_(rhs.time_),
    timer_(rhs.timer_.get_io_service()),
    state_changed_(std::move(rhs.state_changed_))
{
    if(pin_) set_call();
    rhs.reset();
}

////////////////////////////////////////////////////////////////////////////////
contact& contact::operator=(contact&& rhs)
{
    reset();

    multi_tap::operator=(std::move(rhs));
    pin_ = rhs.pin_;
    state_ = off;
    time_ = rhs.time_;
    state_changed_ = std::move(rhs.state_changed_);
    if(pin_) set_call();

    rhs.reset();
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
contact_state contact::state()
{
    if(!pin_) throw std::logic_error(
        "contact: Cannot get state - empty instance"
    );
    return pin_->state() == off ? pressed : released;
}

////////////////////////////////////////////////////////////////////////////////
cid contact::on_state_changed(fn_state_changed fn)
{
    return state_changed_.add(std::move(fn));
}

////////////////////////////////////////////////////////////////////////////////
cid contact::on_press(fn_press fn)
{
    return on_state_changed(
        [fn_ = std::move(fn)](contact_state state)
        { if(state == pressed) fn_(); }
    );
}

////////////////////////////////////////////////////////////////////////////////
cid contact::on_release(fn_release fn)
{
    return on_state_changed(
        [fn_ = std::move(fn)](contact_state state)
        { if(state == released) fn_(); }
    );
}

////////////////////////////////////////////////////////////////////////////////
bool contact::remove_call(cid id)
{
    return state_changed_.remove(id);
}

////////////////////////////////////////////////////////////////////////////////
void contact::set_call()
{
    id_ = pin_->on_state_changed([=](gpio::state state)
    {
        timer_.expires_from_now(time_);
        timer_.async_wait([=](const asio::error_code& ec)
        {
            if(ec) return;

            if(state != state_)
            {
                state_ = state;
                state_changed_(state_ == off ? pressed : released);
                multi_tap::state_changed(state_);
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
        id_ = ncid;
        pin_ = nullptr;
    }
}

////////////////////////////////////////////////////////////////////////////////
}
