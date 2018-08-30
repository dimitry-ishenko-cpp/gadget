cmake_minimum_required(VERSION 3.1)
project(gadget++ VERSION 1.0)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

set(HEADERS
    include/gadget++/call_chain.hpp
)
set(SOURCES
)

add_definitions(-DASIO_STANDALONE)
add_library(gadget++ SHARED ${HEADERS} ${SOURCES})

install(TARGETS gadget++ DESTINATION lib)
set_target_properties(gadget++ PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(gadget++ PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

########################
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})

set(CPACK_GENERATOR DEB)
set(CPACK_STRIP_FILES ON)
set(CPACK_PACKAGE_FILE_NAME
    ${PROJECT_NAME}_${PROJECT_VERSION}_${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_PROCESSOR}
)
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "C++ Gadget Library for Linux")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Dimitry Ishenko <dimitry (dot) ishenko (at) (gee) mail (dot) com>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "gpio++ (>= 5.0)")

set(CPACK_SOURCE_GENERATOR ZIP)
set(CPACK_SOURCE_IGNORE_FILES /\.git.*;/build/;\.user$)

include(CPack)
