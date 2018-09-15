////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef GADGET_LED_HPP
#define GADGET_LED_HPP

////////////////////////////////////////////////////////////////////////////////
#include <gadget++/types.hpp>
#include <gpio++/pin.hpp>

#include <asio/io_service.hpp>

////////////////////////////////////////////////////////////////////////////////
namespace gadget
{

////////////////////////////////////////////////////////////////////////////////
class led
{
public:
    ////////////////////
    led(gpio::pin* pin) : pin_(pin) { }

    led(led&& rhs) noexcept : pin_(rhs.pin_) { rhs.pin_ = nullptr; }
    led& operator=(led&& rhs) noexcept
    {
        pin_ = rhs.pin_; rhs.pin_ = nullptr;
        return *this;
    }

    ////////////////////
    void turn(state s) { pin_->set(s); }
    auto state() { return pin_->state(); }

    void on() { turn(gadget::on); }
    void off() { turn(gadget::off); }

    void dim(percent pc) { pin_->duty_cycle(pc); }
    auto level() const noexcept { return pin_->duty_cycle(); }

protected:
    ////////////////////
    gpio::pin* pin_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
