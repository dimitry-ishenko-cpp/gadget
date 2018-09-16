cmake_minimum_required(VERSION 3.1)
project(gadget++ VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions(-DASIO_STANDALONE)

include_directories(include)

set(HEADERS
    include/gadget++/contact.hpp
    include/gadget++/encoder.hpp
    include/gadget++/led.hpp
    include/gadget++/multi_tap.hpp
    include/gadget++/rgb_led.hpp
    include/gadget++/temp.hpp
    include/gadget++/types.hpp
    include/gadget++/unique_function.hpp
)
set(SOURCES
    lib/contact.cpp
    lib/encoder.cpp
    lib/multi_tap.cpp
    lib/rgb_led.cpp
    lib/temp.cpp
)
set(FILES LICENSE.md README.md)

########################
# dynamic library
add_library(gadget++ SHARED ${HEADERS} ${SOURCES})

# install
install(TARGETS gadget++ DESTINATION lib)
set_target_properties(gadget++ PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(gadget++ PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
install(DIRECTORY include/ DESTINATION include)
install(FILES ${FILES} DESTINATION share/doc/${PROJECT_NAME})

########################
# package
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_STRIP_FILES ON)
set(CPACK_PACKAGE_FILE_NAME
    ${PROJECT_NAME}_${PROJECT_VERSION}_${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_PROCESSOR}
)

# source
set(CPACK_SOURCE_GENERATOR TBZ2)
set(CPACK_SOURCE_IGNORE_FILES /\.git.*;/build/;\.user$)

# binary
set(CPACK_GENERATOR DEB)
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "C++ Gadget Library for Linux")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Dimitry Ishenko <dimitry (dot) ishenko (at) (gee) mail (dot) com>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libasio-dev (>= 1.10.10), gpio++ (>= 1.0)")

include(CPack)
