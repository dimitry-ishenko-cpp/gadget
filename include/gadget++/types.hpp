////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef GADGET_TYPES_HPP
#define GADGET_TYPES_HPP

////////////////////////////////////////////////////////////////////////////////
#include <gpio++/types.hpp>

////////////////////////////////////////////////////////////////////////////////
namespace gadget
{

////////////////////////////////////////////////////////////////////////////////
using gpio::state;
using gpio::on;
using gpio::off;

////////////////////////////////////////////////////////////////////////////////
using nsec = gpio::nsec;

////////////////////////////////////////////////////////////////////////////////
using gpio::percent;
namespace literals { using gpio::literals::operator""_pc; }

////////////////////////////////////////////////////////////////////////////////
enum button_state { pressed, released };

////////////////////////////////////////////////////////////////////////////////
enum encoder_step { ccw = -1, cw = 1 };

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
