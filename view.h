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


inline void quick_view(const _shape &sp, WINDOW* scr = stdscr, int beg_y = 0, int beg_x = 0){
    wmove(scr, beg_y, beg_x);
    addstr("+----------+");
    for(int i=0; i<=3; i++){
        wmove(scr, beg_y+1+i, beg_x);
        addstr("| ");
        for(int j=0; j<=3; j++){
            if(!sp.contents[i][j].filled){
                addch( (chtype) ' ' | COLOR_PAIR(COLOR_BLACK) );
                addch( (chtype) ' ' | COLOR_PAIR(COLOR_BLACK) );
            }
            else{
                addch( (chtype) ' ' | COLOR_PAIR(RGB_map(sp.contents[i][j].color)) );
                addch( (chtype) ' ' | COLOR_PAIR(RGB_map(sp.contents[i][j].color)) );
            }
        }
        addstr(" |");
    }
    wmove(scr, beg_y+1+4, beg_x);
    addstr("+----------+");
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
