////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include <gadget++/encoder.hpp>
#include <utility>

////////////////////////////////////////////////////////////////////////////////
namespace gadget
{

////////////////////////////////////////////////////////////////////////////////
encoder::encoder(gpio::pin* pin1, gpio::pin* pin2) :
    pin1_(pin1), pin2_(pin2)
{
    set_callback();
}

////////////////////////////////////////////////////////////////////////////////
encoder::~encoder() { pin1_->remove(id_); }

////////////////////////////////////////////////////////////////////////////////
encoder::encoder(encoder&& rhs) :
    encoder(rhs.pin1_, rhs.pin2_)
{
    rotate_ = std::move(rhs.rotate_);
    rhs.reset();
}

////////////////////////////////////////////////////////////////////////////////
encoder& encoder::operator=(encoder&& rhs)
{
    reset();

    pin1_   = rhs.pin1_;
    pin2_   = rhs.pin2_;
    state_  = off;
    step_   = nos;

    rotate_ = std::move(rhs.rotate_);
    rhs.reset();

    return *this;
}

////////////////////////////////////////////////////////////////////////////////
cid encoder::on_rotate(encoder::fn_rotate fn)
{
    return rotate_.add(std::move(fn));
}

////////////////////////////////////////////////////////////////////////////////
cid encoder::on_rotate_cw(encoder::fn_rotate_cw fn)
{
    return on_rotate(
        [fn_ = std::move(fn)](encoder_step step)
        { if(step == cw) fn_(); }
    );
}

////////////////////////////////////////////////////////////////////////////////
cid encoder::on_rotate_ccw(encoder::fn_rotate_ccw fn)
{
    return on_rotate(
        [fn_ = std::move(fn)](encoder_step step)
        { if(step == ccw) fn_(); }
    );
}

////////////////////////////////////////////////////////////////////////////////
bool encoder::remove_call(cid id)
{
    return rotate_.remove(id);
}

////////////////////////////////////////////////////////////////////////////////
void encoder::reset()
{
    if(pin1_)
    {
        pin1_->remove(id_);
        pin1_ = nullptr;
    }
    pin2_ = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
void encoder::set_callback()
{
    if(pin1_) id_ = pin1_->on_state_changed([=](gpio::state state)
    {
        if(state != state_)
        {
            if((state_ = state))
            {
                auto step = pin2_->state() ? cw : ccw;
                if(step == step_) rotate_(step);

                step_ = nos;
            }
            else step_ = pin2_->state() ? ccw : cw;
        }
    });
}

////////////////////////////////////////////////////////////////////////////////
}
