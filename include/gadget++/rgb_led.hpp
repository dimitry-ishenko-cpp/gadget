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
#include <tuple>

////////////////////////////////////////////////////////////////////////////////
namespace gadget
{

////////////////////////////////////////////////////////////////////////////////
enum color { red, green, blue };

////////////////////////////////////////////////////////////////////////////////
class rgb_led
{
public:
    ////////////////////
    rgb_led(asio::io_service&, gpio::pin* red, gpio::pin* green, gpio::pin* blue);

    ////////////////////
    bool is_dimmable(color) const noexcept;

    void turn(gadget::state);
    void turn(color, gadget::state);
    void turn(gadget::state red, gadget::state green, gadget::state blue);
    gadget::state state(color);

    void on() { turn(gadget::on); }
    void on(color c) { turn(c, gadget::on); }

    void off() { turn(gadget::off); }
    void off(color c) { turn(c, gadget::off); }

    void dim(percent);
    void dim(color, percent);
    void dim(percent red, percent green, percent blue);
    percent level(color) const noexcept;

protected:
    ////////////////////
    led red_, green_, blue_;

    led& get_led(color) noexcept;
    const led& get_led(color) const noexcept;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
