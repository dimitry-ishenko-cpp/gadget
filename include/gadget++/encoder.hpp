////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef GADGET_ENCODER_HPP
#define GADGET_ENCODER_HPP

////////////////////////////////////////////////////////////////////////////////
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
    encoder(gpio::pin*, gpio::pin*);
   ~encoder();

    encoder(encoder&&);
    encoder& operator=(encoder&&);

    ////////////////////
    using fn_rotate = std::function<void(encoder_step)>;
    using fn_rotate_cw = std::function<void()>;
    using fn_rotate_ccw = std::function<void()>;

    cid on_rotate(fn_rotate);
    cid on_rotate_cw(fn_rotate_cw);
    cid on_rotate_ccw(fn_rotate_ccw);

    bool remove_call(cid);

protected:
    ////////////////////
    gpio::pin* pin1_;
    gpio::cid id_;

    gpio::pin* pin2_;
    gpio::state state_ = off;

    static constexpr auto nos = static_cast<encoder_step>(0);
    encoder_step step_ = nos;

    call_chain<fn_rotate> rotate_;

    ////////////////////
    void reset();
    void set_callback();
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
