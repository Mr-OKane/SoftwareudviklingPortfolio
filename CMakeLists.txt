cmake_minimum_required(VERSION 3.10)
project(MonsterGame)

set(CMAKE_CXX_STANDARD 17)

find_package(SQLite3 REQUIRED)

add_executable(MonsterGame
    source/main.cpp
    source/Monster.cpp
    source/Player.cpp
    source/Battle.cpp
    source/Game.cpp
    source/Item.cpp
    source/Status.cpp
    source/Dungeon.cpp
    source/Database.cpp
)

target_link_libraries(MonsterGame PRIVATE SQLite::SQLite3)
