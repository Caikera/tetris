//
// Created by YLW20 on 2022/6/9.
//

#ifndef TETRIS_PLAYGROUND_H
#define TETRIS_PLAYGROUND_H

#include <array>
#include <chrono>
#include <thread>
#include "content.h"
#include "curses.h"
#include "shape.h"

constexpr int pg_width  = 10 + 2;
constexpr int pg_height = 2*(pg_width-2) + 4 + 1;


typedef std::array<std::array<_content, pg_width>, pg_height> _pg_contents;

struct _playground{
public:
    _pg_contents contents;
    _shape       shape;
    _shape       next_shape;
    int          shape_r;     // the pos of most left-top grid of shape in the pg
    int          shape_c;
    char         row_full[pg_height]; //line_full[i] = 0 indicate i-th line is full
    _playground(){
        shape_r = 0;
        shape_c = (pg_width-1)/2 - 1;
        for(int r=0; r<=pg_height-1; r++){
            for(int c=0; c<=pg_width-1; c++){
                if(c == 0 || c == pg_width-1 || r == pg_height - 1){
                    contents[r][c].filled = 1;
                    contents[r][c].color = {255, 255, 255};
                }
                else
                    contents[r][c].filled = 0;
            }
        }
        for(int i=0; i<=pg_height-2; i++)
            row_full[i] = 0;
        row_full[pg_height-1] = 1;
    }

    int shape_mv_left(){
        for(int r=0; r<=3; r++)
            for(int c=0; c<=3; c++)
                if(shape.contents[r][c].filled){
                    if( r+shape_r<0 || r+shape_r>pg_height || (c+shape_c-1)<0 || (c+shape_c-1)>pg_width-1 ) return -1;
                    if(contents[r+shape_r][c+shape_c-1].filled) return -1;
                }
        shape_c -= 1;
        return 0;
    }

    int shape_mv_right(){
        for(int r=0; r<=3; r++)
            for(int c=0; c<=3; c++)
                if(shape.contents[r][c].filled){
                    if( r+shape_r<0 || r+shape_r>pg_height || (c+shape_c+1)<0 || (c+shape_c+1)>pg_width-1 ) return -1;
                    if(contents[r+shape_r][c+shape_c+1].filled) return -1;
                }
        shape_c += 1;
        return 0;
    }

    void shape_mv_up(){
        if(shape_r > 1) shape_r -= 1;
    }

    int shape_mv_down(){
        for(int r=0; r<=3; r++)
            for(int c=0; c<=3; c++)
                if(shape.contents[r][c].filled){
                    if( r+shape_r+1<0 || r+shape_r+1>pg_height || c+shape_c<0 || c+shape_c>pg_width-1 ) return -1;
                    if(contents[r+shape_r+1][c+shape_c].filled) return -1;
                }
        shape_r += 1;
        return 0;
    }

    bool permit_shape_hard_rotate() {
        _shape test_shape = shape;
        test_shape.rotate();
        for(int r=0; r<=3; r++){
            for(int c=0; c<=3; c++){
                if(test_shape.contents[r][c].filled){
                    if(r+shape_r<0 || r+shape_r>pg_height-1 || c+shape_c<0 || c+shape_c>pg_width-1)
                        return false;
                    if(contents[r+shape_r][c+shape_c].filled){
                        return false;
                    }
                }
            }
        }
        return true;
    }

    bool permit_shape_hard_rotate(int new_shape_r, int new_shape_c){
        _shape test_shape = shape;
        test_shape.rotate();
        for(int r=0; r<=3; r++){
            for(int c=0; c<=3; c++){
                if(test_shape.contents[r][c].filled){
                    if(r+new_shape_r<0 || r+new_shape_r>pg_height-1 || c+new_shape_c<0 || c+new_shape_c>pg_width-1)
                        return false;
                    if(contents[r+new_shape_r][c+new_shape_c].filled){
                        return false;
                    }
                }
            }
        }
        return true;
    }

    void shape_hard_rotate(){
        shape.rotate();
    }

    int shape_rotate(){
        if(permit_shape_hard_rotate()) {
            shape_hard_rotate();
            return 0;
        }
        if (permit_shape_hard_rotate(shape_r, shape_c-1)){
            //beep();
            shape_mv_left();
            shape_hard_rotate();
            return 0;
        }
        if (permit_shape_hard_rotate(shape_r, shape_c-2)){
            //beep();
            shape_mv_left();
            shape_mv_left();
            shape_hard_rotate();
            return 0;
        }
        if (permit_shape_hard_rotate(shape_r, shape_c+1)){
            shape_mv_right();
            shape_hard_rotate();
            return 0;
        }
        if (permit_shape_hard_rotate(shape_r, shape_c+2)){

            shape_mv_right();
            shape_mv_right();
            shape_hard_rotate();
            return 0;
        }
        if (permit_shape_hard_rotate(shape_r-1, shape_c)){
            shape_mv_up();
            shape_hard_rotate();
            return 0;
        }
        if (permit_shape_hard_rotate(shape_r-2, shape_c)){
            shape_mv_up();
            shape_mv_up();
            shape_hard_rotate();
            return 0;
        }
        return -1;
    }

    bool shape_lower_surface_touch(){
        //whether the lower surface of shape has touched the content of playground
        for(int r=0; r<=3; r++){
            for(int c=0; c<=3; c++){
                if(shape.contents[r][c].filled){
                    if((r+shape_r+1) <= pg_height-1 && contents[r+shape_r+1][c+shape_c].filled) {
                        //shape.contents[r][c].color = {255, 255, 255};
                        return true;
                    }
                }
            }
        }
        return false;
    }

    void solidify(){
        //make current shape contents be a part of pg's contents
        for(int r=0; r<=3; r++){
            for(int c=0; c<=3; c++){
                if(shape.contents[r][c].filled){
                    contents[shape_r+r][shape_c+c] = shape.contents[r][c];
                }
            }
        }
    }

    void to_next_shape(){
        shape = next_shape;
        next_shape = _shape{};
        shape_c = (pg_width-1)/2 - 1;
        shape_r = 0;
    }

    void display_buffer(chtype (&vbuffer)[pg_height][2*pg_width]){
        static wchar_t symbol = ' ';
        for(int r=0; r<=pg_height-1; r++){
            for(int c=0; c<=pg_width-1; c++){
                if(contents[r][c].filled) {
                    vbuffer[r][2*c]   = symbol | COLOR_PAIR(RGB_map(contents[r][c].color));
                    vbuffer[r][2*c+1] = symbol | COLOR_PAIR(RGB_map(contents[r][c].color));
                }
                else {
                    vbuffer[r][2*c]   = ' ' | COLOR_PAIR(COLOR_BLACK);
                    vbuffer[r][2*c+1] = ' '| COLOR_PAIR(COLOR_BLACK);
                }
            }
        }
        for(int r=0; r<=3; r++){
            for(int c=0; c<=3; c++){
                if(shape.contents[r][c].filled){
                    vbuffer[r+shape_r][2*(c+shape_c)]   = symbol | COLOR_PAIR(RGB_map(shape.contents[r][c].color));
                    vbuffer[r+shape_r][2*(c+shape_c)+1] = symbol | COLOR_PAIR(RGB_map(shape.contents[r][c].color));
                }
            }
        }
    }

    bool is_row_empty(int r){
        for(int c=1; c<=pg_width-2; c++)
            if(contents[r][c].filled == 1) return false;
        return true;
    }

    bool is_row_full(int r){
        for(int c=1; c<=pg_width-2; c++)
            if(contents[r][c].filled == 0) return false;
        return true;
    }

    //  (4+pg_height-1)/2 4 pg_height-1

    int get_toppest_r(int start_r = (4+pg_height-1)/2, int high_r=3 , int low_r=pg_height-1){
        if(start_r==high_r && start_r==low_r)
            return start_r;
        else{
            if (!is_row_empty(start_r))
                return get_toppest_r((high_r + start_r) / 2, high_r, start_r);
            else
                return get_toppest_r((low_r + start_r + 1) / 2, start_r + 1, low_r);
        }
    }

    void scan_for_full(){
        for(int r = 4; r <= pg_height-2; r++)
            row_full[r] = is_row_full(r);
    }

    void remove_full() {
        scan_for_full();
        for (int r = 4; r <= pg_height - 2; r++)
            if (row_full[r]) {
                for (int c = 1; c <= pg_width - 2; c++)
                    contents[r][c].color = {255, 255, 255};
                std::this_thread::sleep_for(std::chrono::milliseconds(25));
                for (int c = 1; c <= pg_width - 2; c++)
                    contents[r][c].color = {191, 191, 191};
                std::this_thread::sleep_for(std::chrono::milliseconds(25));
                for (int c = 1; c <= pg_width - 2; c++)
                    contents[r][c].color = {127, 127, 127};
                std::this_thread::sleep_for(std::chrono::milliseconds(25));
                for (int c = 1; c <= pg_width - 2; c++)
                    contents[r][c].color = {63, 63, 63};
                std::this_thread::sleep_for(std::chrono::milliseconds(25));
                for (int c = 1; c <= pg_width - 2; c++)
                    contents[r][c].color = {0, 0, 0};
                for (int rr = r; rr >= get_toppest_r(); rr--)
                    for (int c = 1; c <= pg_width - 2; c++) {
                        contents[rr][c].filled = contents[rr - 1][c].filled;
                        contents[rr][c].color = contents[rr - 1][c].color;
                    }

            }
    }

    bool exceed(){
        return get_toppest_r() < 4;
    }








};



#endif //TETRIS_PLAYGROUND_H
