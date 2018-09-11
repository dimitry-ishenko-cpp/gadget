////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include <gadget++/temp.hpp>

#include <algorithm>
#include <cmath>
#include <initializer_list>

////////////////////////////////////////////////////////////////////////////////
namespace gadget
{

////////////////////////////////////////////////////////////////////////////////
namespace
{

using coef = std::initializer_list<double>;

coef red_2 =
{
    +1.42009689606e-14, -2.20908141363e-11, +1.39750287143e-08,
    -4.60735885134e-06, +0.000839423340225, -0.0814873134198,
    3.77521500948
};

coef green_1 =
{
    -9.92239350025e-11, +2.20158956851e-08, -1.87567079831e-06,
    +7.58616833292e-05, -0.00157440006442, +0.03711432675,
    -0.230587787182
};

coef green_2 =
{
    +7.16248383464e-15, -1.12207777143e-11, +7.16648450101e-09,
    -2.39473016612e-06, +0.000445223682939, -0.0446935952643,
    +2.5113795971
};

coef blue_1 =
{
    -1.85758346213e-11, +4.76277142815e-09, -4.24417528381e-07,
    +8.75163420451e-06, +0.000829400768745, -0.025105117995,
    +0.160337726094
};

double calc(coef c, double x)
{
    double y = 0;
    for(auto ci = c.begin(), ce = c.end(); ci != ce; ++ci)
        y += *ci * std::pow(x, ce - ci - 1);
    return y;
}

inline percent clamp(percent pc)
{
    return std::max(std::min(pc, 100_pc), 0_pc);
}

}

////////////////////////////////////////////////////////////////////////////////
color to_color(temp t)
{
    if(t < 6600_K)
    {
        if(t < 400_K) t = 400_K;
        return
        {
            100_pc,
            clamp( calc(green_1, t/100) * 100_pc ),
            t < 2000_K ? 0_pc : clamp( calc(blue_1, t/100) * 100_pc )
        };
    }
    else
    {
        if(t > 40000_K) t = 40000_K;
        return
        {
            clamp( calc(red_2, t/100) * 100_pc ),
            clamp( calc(green_2, t/100) * 100_pc ),
            100_pc
        };
    }
}

////////////////////////////////////////////////////////////////////////////////
}
