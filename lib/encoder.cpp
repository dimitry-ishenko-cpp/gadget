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
encoder::encoder(asio::io_service&, gpio::pin* pin, gpio::pin* pin2) :
    gadget_base(pin), pin2_(pin2)
{
    reset_cid(pin_->on_state_changed([=](gpio::state state)
    {
        if(state != state_)
        {
            if((state_ = state))
            {
                auto step = pin2_->state() ? cw : ccw;
                if(step == step_) rotated_(step);

                step_ = none;
            }
            else step_ = pin2_->state() ? ccw : cw;
        }
    }));
}

////////////////////////////////////////////////////////////////////////////////
cid encoder::on_rotated(encoder::rotated fn)
{
    return rotated_.add(std::move(fn));
}

////////////////////////////////////////////////////////////////////////////////
cid encoder::on_rotated_cw(encoder::rotated_cw fn)
{
    return on_rotated(
        [fn_ = std::move(fn)](encoder_step step)
        { if(step == cw) fn_(); }
    );
}

////////////////////////////////////////////////////////////////////////////////
cid encoder::on_rotated_ccw(encoder::rotated_ccw fn)
{
    return on_rotated(
        [fn_ = std::move(fn)](encoder_step step)
        { if(step == ccw) fn_(); }
    );
}

////////////////////////////////////////////////////////////////////////////////
bool encoder::remove_call(cid id) { return rotated_.remove(id); }

////////////////////////////////////////////////////////////////////////////////
}
