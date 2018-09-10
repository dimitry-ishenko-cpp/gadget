////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include <gadget++/temp.hpp>

#include <algorithm>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////
namespace gadget
{

////////////////////////////////////////////////////////////////////////////////
namespace
{

template<typename T>
inline T clamp(T value, T min, T max)
{
    return std::max(std::min(value, max), min);
}

}

////////////////////////////////////////////////////////////////////////////////
gadget::color to_color(gadget::temp temp)
{
    // algorithm based on:
    // https://github.com/neilbartlett/color-temperature

    temp = clamp(temp, 100_K, 40000_K);

    gadget::color color;
    if(temp < 6600_K)
    {
        color.red = 100_pc;

        auto green = temp / 100 - 2;
        green = -60.88425710866345 - 0.17489000184149 * green + 40.97731842899564 * std::log(green);
        color.green = clamp(std::round(green), 0_pc, 100_pc);

        if(temp > 2000_K)
        {
            auto blue = temp / 100 - 10;
            blue = -99.9095497416506 + 0.32447435545127 * blue + 45.3646839257496 * std::log(blue);
            color.blue = clamp(std::round(blue), 0_pc, 100_pc);
        }
        else color.blue = 0_pc;
    }
    else
    {
        auto red = temp / 100 - 55;
        red = 138.03015908551252 + 0.04478684462124 * red - 15.78575023267501 * std::log(red);
        color.red = clamp(std::round(red), 0_pc, 100_pc);

        auto green = temp / 100 - 50;
        green = 127.62722061615584 + 0.0311508099477 * green - 11.01384170619439 * std::log(green);
        color.green = clamp(std::round(green), 0_pc, 100_pc);

        color.blue = 100_pc;
    }
    return color;
}

////////////////////////////////////////////////////////////////////////////////
}
