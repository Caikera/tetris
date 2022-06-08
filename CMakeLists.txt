cmake_minimum_required(VERSION 3.20)
project(tetris)

set(CMAKE_CXX_STANDARD 14)


IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")
    add_executable(tetris main.cpp shape.h color.h view.h curses_string.h)
    include_directories("D:/lib/PDCurses-3.9")
    find_library(Curses pdcurses.a D:/lib/PDCurses-3.9/wincon)
    target_link_libraries(tetris ${Curses})
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")
    find_package(Curses REQUIRED)
    include_directories(${CURSES_INCLUDE_DIR})
    add_executable(tetris main.cpp)
    target_link_libraries(tetris curses)

#    add_executable(tetris main.cpp shape.h color.h view.h curses_string.h)
#    target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
#    add_executable(tetris main.cpp shape.h color.h view.h curses_string.h)
#    set(INCLUDE_DIR include)
#    include_directories (${INCLUDE_DIR})
#    find_package(Curses REQUIRED)
#    include_directories(${CURSES_INCLUDE_DIR})
#    MESSAGE(STATUS "curses_include_dir ${CURSES_INCLUDE_DIR}")
#    #target_link_libraries(${PROJECT_NAME} ncurses)
#    target_link_libraries(${PROJECT_NAME} ncurses)
ENDIF()


