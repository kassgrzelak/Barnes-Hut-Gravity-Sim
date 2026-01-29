cmake_minimum_required(VERSION 4.1)
project(Barnes_Hut_Gravity_Sim)

set(CMAKE_CXX_STANDARD 20)

find_package(raylib)

add_executable(Barnes_Hut_Gravity_Sim
        main.cpp
        src/Body.cpp
        src/common.cpp
        src/Sim.cpp
        src/Simtree.cpp
        src/logging.cpp
        src/Vec2.cpp
)

target_link_libraries(Barnes_Hut_Gravity_Sim PRIVATE raylib)
target_include_directories(Barnes_Hut_Gravity_Sim PRIVATE include)