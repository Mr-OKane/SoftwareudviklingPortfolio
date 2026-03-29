cmake_minimum_required(VERSION 3.10)
project(MonsterGame)

set(CMAKE_CXX_STANDARD 17)

add_executable(MonsterGame
    source/main.cpp
    source/Monster.cpp
    source/Player.cpp
)
