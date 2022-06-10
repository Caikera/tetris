//
// Created by YLW20 on 2022/6/8.
//

#ifndef TETRIS_VIEW_H
#define TETRIS_VIEW_H

#include "content.h"
#include "curses.h"
#include "color.h"
#include "curses_string.h"
#include "playground.h"
#include "shape.h"


inline void quick_view(const _shape &sp, WINDOW* scr = stdscr){
    curses_string str;
    str += "+----------+\n";
    for(int i=0; i<=3; i++){
        str += "| ";
        for(int j=0; j<=3; j++){
            if(!sp.contents[i][j].filled){
                str += (chtype) ' ' | COLOR_PAIR(COLOR_BLACK);
                str += (chtype) ' ' | COLOR_PAIR(COLOR_BLACK);
            }
            else{
                str += (chtype) ' ' | COLOR_PAIR(RGB_map(sp.contents[i][j].color));
                str += (chtype) ' ' | COLOR_PAIR(RGB_map(sp.contents[i][j].color));
            }
        }
        str += " |\n";
    }
    str += "+----------+\n";
    for(chtype ch: str)
        waddch(scr, ch);
}

inline void quick_view(const _playground &pg, WINDOW* scr = stdscr){
    curses_string str;
    for(int r=0; r<=pg_height-1; r++){
        for(int c=0; c<=pg_width-1; c++){
            if(!pg.contents[r][c].filled){
                str += "  ";
            }
            else{
                str += (chtype) ' ' | COLOR_PAIR(RGB_map(pg.contents[r][c].color));
                str += (chtype) ' ' | COLOR_PAIR(RGB_map(pg.contents[r][c].color));
            }
        }
        str += '\n';
    }
    for(chtype ch: str)
        waddch(scr, ch);
}

inline void quick_view(chtype (&vbuffer)[pg_height][2*pg_width], WINDOW* scr = stdscr){
    for(int r=4; r<=pg_height-1; r++){
        for(int c=0; c<=2*pg_width-1; c++)
            addch(vbuffer[r][c]);
        waddstr(scr, "\n");
    }
}

#endif //TETRIS_VIEW_H
