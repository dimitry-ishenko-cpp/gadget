////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef GADGET_BUTTON_HPP
#define GADGET_BUTTON_HPP

////////////////////////////////////////////////////////////////////////////////
#include <gadget++/call_chain.hpp>
#include <gadget++/types.hpp>
#include <gpio++/pin.hpp>

#include <asio/io_context.hpp>
#include <asio/system_timer.hpp>
#include <chrono>
#include <functional>
#include <utility>

////////////////////////////////////////////////////////////////////////////////
namespace gadget
{

////////////////////////////////////////////////////////////////////////////////
enum state { pressed, released };

////////////////////////////////////////////////////////////////////////////////
class button
{
public:
    ////////////////////
    button(asio::io_context&, gpio::pin*);

    button(const button&) = delete;
    button& operator=(const button&) = delete;

    ////////////////////
    void set(gadget::state state) { pin_->set(state_to_gpio(state)); }
    gadget::state state() { return state_from_gpio(pin_->state()); }

    void press() { set(gadget::pressed); }
    bool pressed() { return state() == gadget::pressed; }

    void release() { set(gadget::released); }
    bool released() { return state() == gadget::released; }

    template<typename Rep, typename Period>
    void debounce_time(std::chrono::duration<Rep, Period> time)
    { time_ = std::chrono::duration_cast<gadget::nsec>(time); }
    auto debounce_time() const noexcept { return time_; }

    ////////////////////
    using state_changed = std::function<void(gadget::state)>;
    using state_pressed = std::function<void()>;
    using state_released = std::function<void()>;

    void on_state_changed(state_changed fn);
    void on_pressed(state_pressed fn);
    void on_released(state_released fn);

protected:
    ////////////////////
    gpio::pin* pin_ = nullptr;
    gadget::state state_;

    gadget::nsec time_ { 10'000'000 };
    asio::system_timer timer_;

    call_chain<state_changed> state_changed_;

    ////////////////////
    inline gadget::state state_from_gpio(gpio::state state)
    { return state == gpio::off ? gadget::pressed : gadget::released; }

    inline gpio::state state_to_gpio(gadget::state state)
    { return state == gadget::pressed ? gpio::off : gpio::on; }
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
