cmake_minimum_required(VERSION 3.1.0)

project(directconnectserver)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()


#! [qt5_generate_repc]
find_package(Qt5 COMPONENTS RemoteObjects REQUIRED)

set(SOURCES
    main.cpp
    simpleswitch.cpp
)

qt5_generate_repc(SOURCES simpleswitch.rep SOURCE)

add_executable(directconnectserver ${SOURCES})
target_link_libraries(directconnectserver Qt5::RemoteObjects)
#! [qt5_generate_repc]
