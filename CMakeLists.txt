cmake_minimum_required(VERSION 3.20)
project(tetris)

set(CMAKE_CXX_STANDARD 14)

add_definitions(-D PDC_WIDE)

set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc")
MESSAGE(STATUS ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(
        tetris main.cpp
        playground.h
        content.h
        ${APP_ICON_RESOURCE_WINDOWS}
)


IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")
    add_definitions(-D ENV_WINDOWS)
    include_directories("D:/lib/PDCurses-3.9_mingw")
    find_file(Curses
            NAMES pdcurses.a
            PATHS "D:/lib/PDCurses-3.9_mingw/wincon" NO_DEFAULT_PATH
            REQUIRED
            )
    MESSAGE(STATUS "${Curses}")
    target_link_libraries(tetris ${Curses})
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")
    add_definitions(-D ENV_DARWIN)
    find_package(Curses REQUIRED)
    include_directories(${CURSES_INCLUDE_DIR})
    target_link_libraries(tetris curses)
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "CYGWIN")
    MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")
    add_definitions(-D CYGWIN)
    find_package(Curses REQUIRED)
    include_directories(${CURSES_INCLUDE_DIR})
    MESSAGE(STATUS "CURSES_INCLUDE_DIR is ${CURSES_INCLUDE_DIR}")
    target_link_libraries(tetris Curses)
ENDIF()


