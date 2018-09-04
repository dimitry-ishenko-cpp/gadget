////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef GADGET_GADGET_BASE_HPP
#define GADGET_GADGET_BASE_HPP

////////////////////////////////////////////////////////////////////////////////
#include <gpio++/pin.hpp>
#include <gpio++/types.hpp>

////////////////////////////////////////////////////////////////////////////////
namespace gadget
{

////////////////////////////////////////////////////////////////////////////////
class gadget_base
{
public:
    ////////////////////
    explicit gadget_base(gpio::pin* pin) : pin_(pin) { }
    virtual ~gadget_base();

    gadget_base(const gadget_base&) = delete;
    gadget_base& operator=(const gadget_base&) = delete;

    gadget_base(gadget_base&&) noexcept = default;
    gadget_base& operator=(gadget_base&&) noexcept = default;

protected:
    ////////////////////
    gpio::pin* pin_ = nullptr;

    static constexpr gpio::cid none { -1, -1 };
    gpio::cid id_ = none;

    void reset_callback(gpio::cid = none);
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
