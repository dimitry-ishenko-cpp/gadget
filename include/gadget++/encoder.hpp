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
#include <gpio++/pin.hpp>

#include <asio/io_service.hpp>
#include <functional>

////////////////////////////////////////////////////////////////////////////////
namespace gadget
{

////////////////////////////////////////////////////////////////////////////////
enum encoder_direction { ccw = -1, cw = 1 };

////////////////////////////////////////////////////////////////////////////////
class encoder
{
public:
    ////////////////////
    encoder(asio::io_service&, gpio::pin*, gpio::pin*);

    encoder(const encoder&) = delete;
    encoder& operator=(const encoder&) = delete;

    ////////////////////
    using rotated = std::function<void(encoder_direction)>;
    using rotated_cw = std::function<void()>;
    using rotated_ccw = std::function<void()>;

    void on_rotated(rotated);
    void on_rotated_cw(rotated_cw);
    void on_rotated_ccw(rotated_ccw);

protected:
    ////////////////////
    gpio::pin* pin1_ = nullptr;
    gpio::pin* pin2_ = nullptr;

    gpio::state state_ = gpio::off;

    static constexpr auto none = static_cast<encoder_direction>(0);
    encoder_direction step_ = none;

    call_chain<rotated> rotated_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
