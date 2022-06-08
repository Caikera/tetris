//
// Created by YLW20 on 2022/6/8.
//

#include <array>
#include <string>
#include <vector>
#include <iostream>

#include <cstdio>

#include "curses.h"
#include "color.h"
#include "shape.h"
#include "view.h"
#include "curses_string.h"

int main(){
    initscr();
    start_color();
    keypad(stdscr, TRUE);
    color_pair_init();
    auto shapes = shapes_define();

    int  idx = 0;
    chtype ch;
    int order;

    while(1) {
        //std::cout << shapes[idx].quick_view() << std::endl;
        quick_view(shapes[idx]);
        order = getch();
        switch(order){
            case(KEY_UP):   idx = idx == 0 ? shapes.size()-1 : idx - 1; break;
            case(KEY_DOWN): idx = idx == shapes.size()-1 ? 0 : idx + 1; break;
            case(KEY_LEFT): shapes[idx].rotate(); break;
            case(KEY_RIGHT): shapes[idx].rotate(); break;
            default: break;
        }
        if(order == KEY_ENTER)
            break;
        clear();
    }


//    while(1){
//        if(getch()==KEY_RIGHT){
//            ch = ' ' | COLOR_PAIR(i++);
//            addch(ch);
//        }
//        else if(getch()==KEY_LEFT){
//            ch = ' ' | COLOR_PAIR(--i);
//            addch('\b');
//        }
//        refresh();
//    }
    return 0;
}