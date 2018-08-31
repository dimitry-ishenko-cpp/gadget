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
encoder::encoder(asio::io_service&, gpio::pin* pin1, gpio::pin* pin2) :
    pin1_(pin1), pin2_(pin2)
{
    pin1_->on_state_changed([=](gpio::state gs)
    {
        if(gs != state_)
        {
            if((state_ = gs))
            {
                auto step = pin2_->state() ? gadget::cw : gadget::ccw;
                if(step == step_) rotated_.call(step);

                step_ = none;
            }
            else step_ = pin2_->state() ? gadget::ccw : gadget::cw;
        }
    });
}

////////////////////////////////////////////////////////////////////////////////
void encoder::on_rotated(encoder::rotated fn)
{
    rotated_.add(std::move(fn));
}

////////////////////////////////////////////////////////////////////////////////
void encoder::on_rotated_cw(encoder::rotated_cw fn)
{
    on_rotated([fn_ = std::move(fn)](encoder_direction dir)
        { if(dir == gadget::cw) fn_(); }
    );
}

////////////////////////////////////////////////////////////////////////////////
void encoder::on_rotated_ccw(encoder::rotated_ccw fn)
{
    on_rotated([fn_ = std::move(fn)](encoder_direction dir)
        { if(dir == gadget::ccw) fn_(); }
    );
}

////////////////////////////////////////////////////////////////////////////////
}
