//
// Created by YLW20 on 2022/6/8.
//

#include <array>
#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <cstdio>
#include <ctime>
extern "C"{
#include "curses.h"
}
#include "color.h"
#include "playground.h"


std::default_random_engine e(std::time(nullptr));
_playground pg;

chtype vbuffer[pg_height][2*pg_width];
bool permit_mv = true;

void handle_playground(std::function<void(_playground &pg)> handler){
    std::unique_lock<std::mutex> locker;
    locker.lock();
    handler(pg);
}

void task_vbuffer_display(_playground &pg){
    std::this_thread::sleep_for(std::chrono::milliseconds(25));
    char text[32];
    while(1){
        {
            std::unique_lock<std::mutex> locker;
            pg.display_buffer(vbuffer);
        }
        move(0, 0);
        for(int r=4; r<=pg_height-1; r++){
            std::sprintf(text, "%2d", r);
            addstr(text);
            for (int c=0; c<=2*pg_width-1; c++){
                add_wch(&(vbuffer[r][c]));
            }
            addch('\n');
        }
        std::sprintf(text, "  Toppest block is in %2dth row.", pg.get_toppest_r((4+pg_height-1)/2, 4, pg_height-1));
        addstr(text);
        refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
//        if(getch() == KEY_UP)
//            break;
    }
}

void task_shape_logic(_playground &pg){
    //std::unique_lock<std::mutex> locker;
    std::mutex locker;
    while(1){
        locker.lock();
        permit_mv = false;
        pg.shape_mv_down();
        permit_mv = true;
        locker.unlock();
        if(pg.shape_lower_surface_touch()){
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            locker.lock();
            permit_mv = false;
            if(pg.shape_lower_surface_touch()){
                pg.solidify();
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                pg.to_next_shape();
            }
            permit_mv = true;
            locker.unlock();
            pg.remove_full();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void task_controll(_playground &pg){
    std::mutex locker;
    while(1){
        int order = getch();

        switch(order){
            case KEY_UP:    if(permit_mv){ locker.lock(); pg.shape_rotate();   locker.unlock(); } break;
            case KEY_DOWN:  if(permit_mv){ locker.lock(); pg.shape_mv_down();  locker.unlock(); } break;
            case KEY_LEFT:  if(permit_mv){ locker.lock(); pg.shape_mv_left();  locker.unlock(); } break;
            case KEY_RIGHT: if(permit_mv){ locker.lock(); pg.shape_mv_right(); locker.unlock();} break;
            default: break;
        }

    }
}

int main(){
    initscr();
    start_color();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    color_pair_init();
    auto shapes = shapes_define();
    std::thread thread_display(task_vbuffer_display, std::ref(pg));
    std::thread thread_shape_logic(task_shape_logic, std::ref(pg));
    std::thread thread_controll(task_controll, std::ref(pg));
    while(1);
    return 0;
}