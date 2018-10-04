# C++ Gadget Library for Linux

This library is built on top of the [gpio++](https://github.com/dimitry-ishenko-cpp/gpio) library. It abstracts various gadgets that can be controlled/emulated via GPIO pins.

Currently implemented gadgets are:
* gadget::contact - push button, switch, etc.
* gadget::encoder - rotary encoder
* gadget::led - single color LED

## Getting Started

### Prerequisites

* [asio C++ Library](https://think-async.com/) >= 1.10.10
* [C++ GPIO Library for Linux](https://github.com/dimitry-ishenko-cpp/gpio) >= 2.0
* CMake >= 3.1

### Installation

Binary (Debian/Ubuntu/etc):
```console
$ version=1.0
$ arch=$(uname -p)
$ wget https://github.com/dimitry-ishenko-cpp/gadget/releases/download/v${version}/gadget++_${version}_Linux_${arch}.deb
$ sudo apt install ./gadget++_${version}_Linux_${arch}.deb
```

Compile from source:
```console
$ version=1.0
$ wget https://github.com/dimitry-ishenko-cpp/gadget/releases/download/v${version}/gadget++-${version}-Source.tar.bz2
$ tar xjf gadget++-${version}-Source.tar.bz2
$ mkdir gadget++-${version}-Source/build
$ cd gadget++-${version}-Source/build
$ cmake .. -DCMAKE_INSTALL_PREFIX=/usr
$ make
$ sudo make install
```

Clone and compile from repository:
```console
$ git clone https://github.com/dimitry-ishenko-cpp/gadget.git gadget++
$ mkdir gadget++/build
$ cd gadget++/build
$ cmake .. -DCMAKE_INSTALL_PREFIX=/usr
$ make
$ sudo make install
```

### Usage

Example:
```cpp
#include <gadget++/contact.hpp>
#include <gadget++/encoder.hpp>
#include <gadget++/rgb_led.hpp>
#include <gadget++/temp.hpp>
#include <gpio++/gpio.hpp>

#include <asio.hpp>

#include <algorithm>
#include <iostream>

using namespace gpio::literals;
using namespace gadget::literals;

using std::cout; using std::endl;

int main()
{
    asio::io_service io;
    auto chip = gpio::get_chip(io, "chip:0");

    // optical encoder
    gadget::encoder encoder(
        chip->pin(5)->as(in),
        chip->pin(6)->as(in)
    );

    // push button
    gadget::contact button(io, chip->pin(7)->as(in));

    // rgb led
    gadget::rgb_led led(
        chip->pin(14)->as(out), // red
        chip->pin(15)->as(out), // green
        chip->pin(16)->as(out)  // blue
    );

    // initial color temp and level
    gadget::temp temp = 3200_K;
    led.color(gadget::to_color(temp));
    led.dim(10_pc);

    // control mode
    enum { set_temp, set_level } mode = set_level;

    // switch between color temp and level
    button.on_tap([&]()
    {
        switch(mode)
        {
        case set_temp:
            cout << "Switching to level mode" << endl;
            mode = set_level;
            break;

        case set_level:
            cout << "Switching to color temp mode" << endl;
            mode = set_temp;
            break;
        }
    });

    // adjust color temp or level
    encoder.on_rotate([&](gadget::encoder_step step)
    {
        switch(mode)
        {
        case set_temp:
            // adjust color temp
            temp = temp + step * 100_K;

            // clamp color temp between 400K and 40,000K
            temp = std::max(std::min(temp, 40000_K), 400_K);

            led.color(gadget::to_color(temp));
            cout << "Setting color temp to " << temp << "K" << endl;
            break;

        case set_level:
            // adjust level
            led.dim(led.level() + step * 2_pc);
            cout << "Setting level to " << led.level() << "%" << endl;
            break;
        }
    });

    asio::system_timer timer(io);
    std::function<void()> dim_led = [&]()
    {
        led.dim(led.level() - 2_pc);

        if(led.level() > 0_pc)
        {
            timer.expires_from_now(100ms);
            timer.async_wait([&](const asio::error_code& ec) { if(!ec) dim_led(); });
        }
    };
    button.on_tap_twice(dim_led);

    io.run();
    return 0;
}
```

Compile and run:
```console
$ g++ example.cpp -o example -DASIO_STANDALONE -lgadget++ -lgpio++ -ldl
$ ./example
```

## Authors

* **Dimitry Ishenko** - dimitry (dot) ishenko (at) (gee) mail (dot) com

## License

This project is distributed under the GNU GPL license. See the
[LICENSE.md](LICENSE.md) file for details.
