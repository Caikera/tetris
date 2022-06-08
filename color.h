//
// Created by YLW20 on 2022/6/8.
//

#ifndef TETRIS_COLOR_H
#define TETRIS_COLOR_H

#include "curses.h"

inline int color_pair_init(){
    for (int i = 16; i < 256; i++)
        if(init_pair(i, COLOR_BLACK, i) == ERR)
            return ERR;
    return OK;
}

inline short RGB_map(unsigned short R, unsigned short G, unsigned short B){
    /**
     map RGB value to short number in range 16-215 for PDCurses color pair
     **/
    R = R * 6.0 / 256.0;
    G = G * 6.0 / 256.0;
    B = B * 6.0 / 256.0;
    return ( R*36 + G*6 + B*1 + 16);
}

inline chtype RGB_blank(unsigned short R, unsigned short G, unsigned short B){
    return (' ' | COLOR_PAIR(RGB_map(R, G, B)));
}

#endif //TETRIS_COLOR_H
