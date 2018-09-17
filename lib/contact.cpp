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
    multi_tap(io), pin_(pin), timer_(io)
{
    set_call();
}

////////////////////////////////////////////////////////////////////////////////
contact::~contact() { reset_call(); }

////////////////////////////////////////////////////////////////////////////////
contact::contact(contact&& rhs) :
    multi_tap(std::move(rhs)),
    pin_(rhs.pin_), time_(rhs.time_), timer_(rhs.timer_.get_io_service()),
    state_changed_(std::move(rhs.state_changed_))
{
    set_call();
    rhs.reset_call();
}

////////////////////////////////////////////////////////////////////////////////
contact& contact::operator=(contact&& rhs)
{
    reset_call();

    multi_tap::operator=(std::move(rhs));
    pin_ = rhs.pin_; state_ = off; time_ = rhs.time_;
    state_changed_ = std::move(rhs.state_changed_);
    set_call();

    rhs.reset_call();
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
    if(pin_) id_ = pin_->on_state_changed([=](gpio::state state)
    {
        timer_.expires_from_now(time_);
        timer_.async_wait([=](const asio::error_code& ec)
        {
            if(ec) return;

            if(state != state_)
            {
                state_ = state;
                state_changed_(state_ == off ? pressed : released);
                multi_tap::operator()(state_);
            }
        });
    });
}

////////////////////////////////////////////////////////////////////////////////
void contact::reset_call()
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
