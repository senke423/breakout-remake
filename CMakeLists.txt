cmake_minimum_required(VERSION 3.28)
project(44_breakout_cpp_attempt3)

set(CMAKE_CXX_STANDARD 17)

add_executable(44_breakout_cpp_attempt3 main.cpp
        Game.cpp
        Game.h
        Paddle.cpp
        Paddle.h
        Headers.h
        MainMenu.cpp
        MainMenu.h
        Level.cpp
        Level.h
        Block.cpp
        Block.h
        Ball.cpp
        Ball.h
)

target_link_libraries(44_breakout_cpp_attempt3 PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)