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

#include <asio/io_service.hpp>
#include <asio/system_timer.hpp>
#include <chrono>
#include <functional>

////////////////////////////////////////////////////////////////////////////////
namespace gadget
{

////////////////////////////////////////////////////////////////////////////////
class button
{
public:
    ////////////////////
    button(asio::io_service&, gpio::pin*);

    ////////////////////
    void set(button_state state) { pin_->set(state_to_gpio(state)); }
    auto state() { return state_from_gpio(pin_->state()); }

    void press() { set(pressed); }
    bool is_pressed() { return state() == pressed; }

    void release() { set(released); }
    bool is_released() { return state() == released; }

    template<typename Rep, typename Period>
    void debounce_time(std::chrono::duration<Rep, Period> time)
    { time_ = std::chrono::duration_cast<nsec>(time); }
    auto debounce_time() const noexcept { return time_; }

    ////////////////////
    using state_changed = std::function<void(button_state)>;
    using state_pressed = std::function<void()>;
    using state_released = std::function<void()>;

    void on_state_changed(state_changed);
    void on_pressed(state_pressed);
    void on_released(state_released);

protected:
    ////////////////////
    gpio::pin* pin_ = nullptr;
    button_state state_;

    nsec time_ { 10'000'000 };
    asio::system_timer timer_;

    call_chain<state_changed> state_changed_;

    ////////////////////
    inline button_state state_from_gpio(gpio::state state)
    { return state == off ? pressed : released; }

    inline gpio::state state_to_gpio(button_state state)
    { return state == pressed ? off : on; }
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
