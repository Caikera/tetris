//
// Created by YLW20 on 2022/6/8.
//


#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

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

bool    gameover           = false;
int     score              = 0;


double get_difficulty(double set){
    static double current_difficulty = 0;
    std::unique_lock<std::mutex> locker;
    if(set <= 4.9 && set >= 0){
        //beep();
        current_difficulty = set <= 4.9 && set >= 0 ? set : current_difficulty;
    }
    return current_difficulty;
}


void handle_playground(std::function<void(_playground &pg)> handler){
    std::mutex locker;
    locker.try_lock();
    handler(pg);
    locker.unlock();
}

void task_vbuffer_display(){
    std::this_thread::sleep_for(std::chrono::milliseconds(25));
    char text[64];
    while(1){
        handle_playground([&](_playground &pg)->void{pg.display_buffer(vbuffer);});
        pg.display_buffer(vbuffer);
        //display the playground
        wmove(stdscr, 0, 0);
        for(int r=4; r<=pg_height-1; r++){
            std::sprintf(text, "%2d", r);
            waddstr(stdscr, text);
            for (int c=0; c<=2*pg_width-1; c++){
                waddch(stdscr, vbuffer[r][c]);
            }
            waddch(stdscr, '\n');
        }
        std::sprintf(text, "  Toppest block is in %2dth row.\n", pg.get_toppest_r((4+pg_height-1)/2, 4, pg_height-1));
        waddstr(stdscr, text);
        //display the score and next shape
        std::sprintf(text, "  Your score is %004d.\n", score);
        waddstr(stdscr, text);
        std::sprintf(text, "  Current difficulty level is %1.2lf.\n", get_difficulty(-1));
        waddstr(stdscr, text);
        wmove(stdscr, 4, 2*(pg_width+2));
        std::sprintf(text, "Next shape is:\n");
        waddstr(stdscr, text);
        handle_playground([&](_playground &pg)->void{quick_view(pg.next_shape, stdscr, 5, 2*(pg_width+2));});
        wrefresh(stdscr);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        if(gameover){
            wmove(stdscr, pg_height-4+3, 0);
            addstr("  GAME OVER.\n  To restart the game, please relunch the program.");
            wrefresh(stdscr);
            break;
        }
    }
}

void task_shape_logic(){
    //std::unique_lock<std::mutex> locker;
    while(1){
        if(pg.shape_lower_surface_touch()){
            std::this_thread::sleep_for(std::chrono::milliseconds((int) (5-get_difficulty(-1))*100));
            if(pg.shape_lower_surface_touch()){
                handle_playground([&](_playground &pg)->void{pg.solidify();});
                std::this_thread::sleep_for(std::chrono::milliseconds((int) (5-get_difficulty(-1))*20));
                handle_playground([&](_playground &pg)->void{pg.to_next_shape();});
            }
            handle_playground([&](_playground &pg)->void{score += pg.remove_full();});
            get_difficulty(( (double)score / 100 * 4.9));
        }else{
            handle_playground([&](_playground &pg)->void{pg.shape_mv_down();});
        }
        if(pg.exceed()){
            gameover = true;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds((int) (5-get_difficulty(-1))*100));
    }
}

void task_controll(){
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

int main(){
    initscr();
    start_color();
    keypad(stdscr, TRUE);
    curs_set(0);
    //nodelay(stdscr, TRUE);
    color_pair_init();
    auto shapes = shapes_define();
    std::thread thread_display(task_vbuffer_display);
    std::thread thread_shape_logic(task_shape_logic);
    std::thread thread_controll(task_controll);
    while(1){
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        if(gameover) break;
    }
    while(1);
    thread_controll.join();
    thread_shape_logic.join();
    thread_display.join();
    return 9;
}