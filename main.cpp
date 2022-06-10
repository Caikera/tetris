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
#include "mygetch.h"
#include "playground.h"
#include "view.h"



std::default_random_engine e(std::time(nullptr));
_playground pg;
chtype vbuffer[pg_height][2*pg_width];
bool permit_mv = true;
bool gameover = false;
int  score = 0;

void handle_playground(std::function<void(_playground &pg)> handler){
    std::unique_lock<std::mutex> locker;
    locker.lock();
    handler(pg);
}

WINDOW* main_win;
WINDOW* sub_win;

void task_vbuffer_display(_playground &pg){
    std::this_thread::sleep_for(std::chrono::milliseconds(25));
    char text[32];
    while(1){
        {
            std::unique_lock<std::mutex> locker;
            pg.display_buffer(vbuffer);
        }
        wmove(main_win, 0, 0);
        for(int r=4; r<=pg_height-1; r++){
            std::sprintf(text, "%2d", r);
            waddstr(main_win, text);
            for (int c=0; c<=2*pg_width-1; c++){
                waddch(main_win, vbuffer[r][c]);
            }
            waddch(main_win, '\n');
        }
        std::sprintf(text, "  Toppest block is in %2dth row.", pg.get_toppest_r((4+pg_height-1)/2, 4, pg_height-1));
        waddstr(main_win, text);

        wrefresh(main_win);
//        std::sprintf(text, "Your score is %d.\n  Next shape is:\n", score);
//        wmove(sub_win, 0, 0);
//        waddstr(sub_win, text);
//        //quick_view(pg.next_shape);
//        wrefresh(sub_win);
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
        if(gameover) break;
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
        if(pg.exceed()){
            gameover = true;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void task_controll(_playground &pg){
    std::mutex locker;
    while(1){
        int order = mygetch();
        switch(order){
            case KEY_UP:    if(permit_mv){ locker.lock(); pg.shape_rotate();   locker.unlock(); } break;
            case KEY_DOWN:  if(permit_mv){ locker.lock(); pg.shape_mv_down();  locker.unlock(); } break;
            case KEY_LEFT:  if(permit_mv){ locker.lock(); pg.shape_mv_left();  locker.unlock(); } break;
            case KEY_RIGHT: if(permit_mv){ locker.lock(); pg.shape_mv_right(); locker.unlock();} break;
            default: break;
        }
        if(gameover) break;
    }
}

void task_count_score(){
    std::this_thread::sleep_for(std::chrono::milliseconds(900));
    score++;
}


int main(){
    initscr();
    start_color();
    keypad(stdscr, TRUE);
    main_win = newwin(pg_height+2, 2*(pg_width+1), 0, 0);
    sub_win  = newwin(20, 20, 0, 2*(pg_width+1)+2);
    //nodelay(stdscr, TRUE);
    color_pair_init();
    auto shapes = shapes_define();
    std::thread thread_display(task_vbuffer_display, std::ref(pg));
    std::thread thread_shape_logic(task_shape_logic, std::ref(pg));
    std::thread thread_controll(task_controll, std::ref(pg));
    while(1);
    return 0;
}