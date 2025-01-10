cmake_minimum_required(VERSION 3.30)
project(PROJ_PC)

set(CMAKE_CXX_STANDARD 17)

include_directories(inc)

add_executable(PROJ_PC
        inc/AccelerometerClass.hpp
        inc/CommunicationModuleBase.hpp
        inc/CommunicationModulePC.hpp
        inc/SerialPort.hpp
        src/AccelerometerClass.cpp
        src/CommunicationModulePC.cpp
        src/main.cpp
        src/SerialPort.cpp)
