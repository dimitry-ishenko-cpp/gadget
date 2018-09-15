////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef GADGET_CONTACT_HPP
#define GADGET_CONTACT_HPP

////////////////////////////////////////////////////////////////////////////////
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
class contact
{
public:
    ////////////////////
    contact(asio::io_service&, gpio::pin*);
   ~contact();

    contact(contact&&);
    contact& operator=(contact&&);

    ////////////////////
    auto state() { return to_contact_state(pin_->state()); }

    bool is_pressed() { return state() == pressed; }
    bool is_released() { return state() == released; }

    template<typename Rep, typename Period>
    void debounce_time(std::chrono::duration<Rep, Period> time)
    {
        time_ = std::chrono::duration_cast<nsec>(time);
    }
    auto debounce_time() const noexcept { return time_; }

    ////////////////////
    using fn_state_changed = std::function<void(contact_state)>;
    using fn_press = std::function<void()>;
    using fn_release = std::function<void()>;

    cid on_state_changed(fn_state_changed);
    cid on_press(fn_press);
    cid on_release(fn_release);

    bool remove_call(cid);

protected:
    ////////////////////
    gpio::pin* pin_;
    gpio::cid id_;

    static constexpr auto nos = static_cast<contact_state>(-1);
    contact_state state_ = nos;

    nsec time_ = 10ms;
    asio::system_timer timer_;

    call_chain<fn_state_changed> state_changed_;

    ////////////////////
    static contact_state to_contact_state(gpio::state state)
    { return state == off ? pressed : released; }

    void set_callback();
    void reset();
    void move_and_reset(contact&);
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
