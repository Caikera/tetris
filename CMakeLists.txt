cmake_minimum_required(VERSION 3.20)
project(tetris)

set(CMAKE_CXX_STANDARD 14)

include_directories("D:/lib/PDCurses-3.9")
find_library(Curses pdcurses.a D:/lib/PDCurses-3.9/wincon)
add_executable(tetris main.cpp shape.h color.h view.h curses_string.h)
target_link_libraries(tetris ${Curses})