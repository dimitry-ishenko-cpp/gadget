////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef GADGET_ENCODER_HPP
#define GADGET_ENCODER_HPP

////////////////////////////////////////////////////////////////////////////////
#include <gadget++/call_chain.hpp>
#include <gadget++/types.hpp>
#include <gpio++/pin.hpp>

#include <asio/io_service.hpp>
#include <functional>

////////////////////////////////////////////////////////////////////////////////
namespace gadget
{

////////////////////////////////////////////////////////////////////////////////
class encoder
{
public:
    ////////////////////
    encoder(asio::io_service&, gpio::pin*, gpio::pin*);

    ////////////////////
    using rotated = std::function<void(encoder_step)>;
    using rotated_cw = std::function<void()>;
    using rotated_ccw = std::function<void()>;

    void on_rotated(rotated);
    void on_rotated_cw(rotated_cw);
    void on_rotated_ccw(rotated_ccw);

protected:
    ////////////////////
    gpio::pin* pin_= nullptr;
    gpio::pin* pin2_ = nullptr;

    gpio::state state_ = off;

    static constexpr auto none = static_cast<encoder_step>(0);
    encoder_step step_ = none;

    call_chain<rotated> rotated_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
