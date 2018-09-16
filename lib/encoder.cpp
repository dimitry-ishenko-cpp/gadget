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
    move_and_reset(rhs);
}

////////////////////////////////////////////////////////////////////////////////
encoder& encoder::operator=(encoder&& rhs)
{
    reset();

    pin1_  = rhs.pin1_;
    pin2_  = rhs.pin2_;
    state_ = off;
    step_  = nos;

    move_and_reset(rhs);
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
    return rotate_cw_.add(std::move(fn));
}

////////////////////////////////////////////////////////////////////////////////
cid encoder::on_rotate_ccw(encoder::fn_rotate_ccw fn)
{
    return rotate_ccw_.add(std::move(fn));
}

////////////////////////////////////////////////////////////////////////////////
bool encoder::remove_call(cid id)
{
    return rotate_.remove(id)
        || rotate_cw_.remove(id)
        || rotate_ccw_.remove(id);
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
                if(step == step_)
                {
                    rotate_(step);

                         if(step == cw) rotate_cw_();
                    else if(step == ccw) rotate_ccw_();
                }

                step_ = nos;
            }
            else step_ = pin2_->state() ? ccw : cw;
        }
    });
}

////////////////////////////////////////////////////////////////////////////////
void encoder::move_and_reset(encoder& rhs)
{
    rotate_ = std::move(rhs.rotate_);
    rotate_cw_ = std::move(rhs.rotate_cw_);
    rotate_ccw_ = std::move(rhs.rotate_ccw_);

    rhs.reset();
}

////////////////////////////////////////////////////////////////////////////////
}
