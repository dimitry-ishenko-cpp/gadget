////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef GADGET_TYPES_HPP
#define GADGET_TYPES_HPP

////////////////////////////////////////////////////////////////////////////////
#include <gpio++/call_chain.hpp>
#include <gpio++/types.hpp>

////////////////////////////////////////////////////////////////////////////////
namespace gadget
{

////////////////////////////////////////////////////////////////////////////////
using namespace gpio;

namespace literals { using namespace gpio::literals; }
using namespace literals;

////////////////////////////////////////////////////////////////////////////////
namespace literals { enum contact_state { pressed, released }; }

////////////////////////////////////////////////////////////////////////////////
namespace literals { enum encoder_step { ccw = -1, cw = 1 }; }

////////////////////////////////////////////////////////////////////////////////
struct color
{
    percent red;
    percent green;
    percent blue;
};

namespace literals
{

constexpr color white { 100_pc, 100_pc, 100_pc };
constexpr color black {   0_pc,   0_pc,   0_pc };
constexpr color red   { 100_pc,   0_pc,   0_pc };
constexpr color green {   0_pc, 100_pc,   0_pc };
constexpr color blue  {   0_pc,   0_pc, 100_pc };

}

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
