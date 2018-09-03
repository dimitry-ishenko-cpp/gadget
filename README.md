# C++ Gadget Library for Linux

This library is built on top of the [gpio++](https://github.com/dimitry-ishenko-cpp/gpio) library. It abstracts various gadgets that can be controlled/emulated via GPIO pins.

Currently implemented gadgets are:
* gadget::contact - push button, switch, etc.
* gadget::encoder - rotary encoder
* gadget::led - single color LED

## Getting Started

### Prerequisites

* [asio C++ Library](https://think-async.com/) >= 1.10.10
* [C++ GPIO Library for Linux](https://github.com/dimitry-ishenko-cpp/gpio) >= 7.0
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

TODO

## Authors

* **Dimitry Ishenko** - dimitry (dot) ishenko (at) (gee) mail (dot) com

## License

This project is distributed under the GNU GPL license. See the
[LICENSE.md](LICENSE.md) file for details.
