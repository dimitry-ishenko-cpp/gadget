////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef GADGET_CONTACT_HPP
#define GADGET_CONTACT_HPP

////////////////////////////////////////////////////////////////////////////////
#include <gadget++/gadget_base.hpp>
#include <gadget++/types.hpp>

#include <asio/io_service.hpp>
#include <asio/system_timer.hpp>
#include <chrono>
#include <functional>

////////////////////////////////////////////////////////////////////////////////
namespace gadget
{

////////////////////////////////////////////////////////////////////////////////
class contact : public gadget_base
{
public:
    ////////////////////
    contact(asio::io_service&, gpio::pin*);

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
    using state_changed = std::function<void(contact_state)>;
    using state_pressed = std::function<void()>;
    using state_released = std::function<void()>;

    cid on_state_changed(state_changed);
    cid on_pressed(state_pressed);
    cid on_released(state_released);

    bool remove_call(cid);

protected:
    ////////////////////
    contact_state state_;

    nsec time_ = 5ms;
    asio::system_timer timer_;

    call_chain<state_changed> state_changed_;

    ////////////////////
    static contact_state to_contact_state(gpio::state state)
    { return state == off ? pressed : released; }
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
