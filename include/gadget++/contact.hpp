////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef GADGET_CONTACT_HPP
#define GADGET_CONTACT_HPP

////////////////////////////////////////////////////////////////////////////////
#include <gadget++/multi_tap.hpp>
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
class contact : public multi_tap
{
public:
    ////////////////////
    contact(asio::io_service&, gpio::pin*, gpio::flag flags = { });
    contact(asio::io_service&, gpio::pin*, adopt_pin_t);
   ~contact();

    contact(contact&&);
    contact& operator=(contact&&);

    ////////////////////
    contact_state state();

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

    ////////////////////
    using multi_tap::tap_time;
    using multi_tap::hold_time;

protected:
    ////////////////////
    gpio::pin* pin_;
    gpio::cid id_ = ncid;

    gpio::state state_ = off;

    nsec time_ = 10ms;
    asio::system_timer timer_;

    call_chain<fn_state_changed> state_changed_;

    ////////////////////
    void set_call();
    void reset();
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
