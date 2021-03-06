////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef GADGET_RGB_LED_HPP
#define GADGET_RGB_LED_HPP

////////////////////////////////////////////////////////////////////////////////
#include <gadget++/led.hpp>
#include <asio/io_service.hpp>

////////////////////////////////////////////////////////////////////////////////
namespace gadget
{

////////////////////////////////////////////////////////////////////////////////
class rgb_led
{
public:
    ////////////////////
    rgb_led(gpio::pin* red, gpio::pin* green, gpio::pin* blue, gpio::flag flags = { });
    rgb_led(gpio::pin* red, gpio::pin* green, gpio::pin* blue, adopt_pin_t);

    rgb_led(rgb_led&&) = default;
    rgb_led& operator=(rgb_led&&) = default;

    ////////////////////
    void color(const gadget::color&);
    const auto& color() const noexcept { return color_; }

    void turn(state);

    void on() { turn(gadget::on); }
    void off() { turn(gadget::off); }

    void dim(percent);
    auto level() const noexcept { return pc_; }

protected:
    ////////////////////
    led red_, green_, blue_;

    gadget::color color_ = white;
    percent pc_ = 100_pc;

    void sync_state();
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
