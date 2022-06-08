//
// Created by YLW20 on 2022/6/8.
//

#ifndef TETRIS_VIEW_H
#define TETRIS_VIEW_H

#include "curses.h"
#include "shape.h"
#include "color.h"
#include "curses_string.h"

inline void quick_view(const _shape &shape){
    curses_string str;
    str += "+----------+\n";
    for(int i=0; i<=3; i++){
        str += "| ";
        for(int j=0; j<=3; j++){
            if(!shape.contents[i][j]){
                str += "  ";
            }
            else{
                str += (chtype) ' ' | COLOR_PAIR(RGB_map(shape.RGB.R, shape.RGB.G, shape.RGB.B));
                str += (chtype) ' ' | COLOR_PAIR(RGB_map(shape.RGB.R, shape.RGB.G, shape.RGB.B));
            }
        }
        str += " |\n";
    }
    str += "+----------+\n";
    for(chtype ch: str)
        addch(ch);
    refresh();
}

#endif //TETRIS_VIEW_H
