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
    led(asio::io_service&, gpio::pin* pin) : pin_(pin) { }

    led(const led&) = delete;
    led& operator=(const led&) = delete;

    ////////////////////
    bool is_dimmable() const noexcept { return pin_->mode() == gpio::pwm; }

    void turn(gadget::state state) { pin_->set(state); }
    gadget::state state() { return pin_->state(); }

    void on() { turn(gadget::on); }
    void off() { turn(gadget::off); }

    void set(percent pc) { pin_->set(pc); }
    percent level() const noexcept { return pin_->duty_cycle(); }

protected:
    ////////////////////
    gpio::pin* pin_ = nullptr;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif