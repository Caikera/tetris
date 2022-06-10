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
#include "view.h"

std::default_random_engine e(std::time(nullptr));
_playground pg;
chtype vbuffer[pg_height][2*pg_width];
bool permit_mv = true;
bool gameover = false;
int  score = 0;

void handle_playground(std::function<void(_playground &pg)> handler){
    std::mutex locker;
    locker.try_lock();
    handler(pg);
    locker.unlock();
}

//WINDOW* main_win;
WINDOW* sub_win;

void task_vbuffer_display(_playground &pg){
    std::this_thread::sleep_for(std::chrono::milliseconds(25));
    char text[32];
    while(1){
        handle_playground([&](_playground &pg)->void{pg.display_buffer(vbuffer);});
        pg.display_buffer(vbuffer);
        //screen: display the playground
        wmove(stdscr, 0, 0);
        for(int r=4; r<=pg_height-1; r++){
            std::sprintf(text, "%2d", r);
            waddstr(stdscr, text);
            for (int c=0; c<=2*pg_width-1; c++){
                waddch(stdscr, vbuffer[r][c]);
            }
            waddch(stdscr, '\n');
        }
        std::sprintf(text, "  Toppest block is in %2dth row.", pg.get_toppest_r((4+pg_height-1)/2, 4, pg_height-1));
        waddstr(stdscr, text);
        wrefresh(stdscr);
        //sub_scr display the score and next shape
        std::sprintf(text, "Your score is %d.\nNext shape is:\n", score);
        wmove(sub_win, 0, 0);
        waddstr(sub_win, text);
        handle_playground([&](_playground pg)->void{quick_view(pg.next_shape, sub_win);});
        wrefresh(sub_win);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        if(gameover) break;
    }
}

void task_shape_logic(_playground &pg){
    //std::unique_lock<std::mutex> locker;
    while(1){
        handle_playground([&](_playground &pg)->void{pg.shape_mv_down();});
        if(pg.shape_lower_surface_touch()){
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            if(pg.shape_lower_surface_touch()){
                handle_playground([&](_playground &pg)->void{pg.solidify();});
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                handle_playground([&](_playground &pg)->void{pg.to_next_shape();});
            }
            handle_playground([&](_playground &pg)->void{pg.remove_full();});
        }
        if(pg.exceed()){
            gameover = true;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void task_controll(_playground &pg){
    while(1){
        int order = getch();
        switch(order){
            case KEY_UP:    handle_playground([&](_playground &pg)->void{pg.shape_rotate();   });  break;
            case KEY_DOWN:  handle_playground([&](_playground &pg)->void{pg.shape_mv_down();  }); break;
            case KEY_LEFT:  handle_playground([&](_playground &pg)->void{pg.shape_mv_left();  }); break;
            case KEY_RIGHT: handle_playground([&](_playground &pg)->void{pg.shape_mv_right(); }); break;
            default: break;
        }
        if(gameover) break;
    }
}

void task_count_score(){
    while(1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(900));
        score++;
        if(gameover) break;
    }
}


int main(){
    initscr();
    start_color();
    keypad(stdscr, TRUE);
    curs_set(0);
//    main_win = newwin(pg_height+2, 2*(pg_width+4), 0, 0);
    sub_win  = subwin(stdscr, 20, 20, 0, 2*(pg_width+4));
    //nodelay(stdscr, TRUE);
    color_pair_init();
    auto shapes = shapes_define();
    std::thread thread_display(task_vbuffer_display, std::ref(pg));
    std::thread thread_shape_logic(task_shape_logic, std::ref(pg));
    std::thread thread_controll(task_controll, std::ref(pg));
    std::thread thread_count_score(task_count_score);
    while(1);
    return 0;
}