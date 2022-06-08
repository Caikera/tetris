//
// Created by YLW20 on 2022/6/8.
//

#ifndef TETRIS_CURSES_STRING_H
#define TETRIS_CURSES_STRING_H

#include <vector>
#include <string>
#include "curses.h"

typedef std::vector<chtype> curses_string;

inline void operator += (std::vector<chtype> &raw, std::vector<chtype> &append){
    raw.insert(raw.end(), append.begin(), append.end());
}

inline void operator += (std::vector<chtype> &raw, chtype append){
    raw.push_back(append);
}

inline void operator += (std::vector<chtype> &raw, std::string &append){
    for(int i=0; i<=append.length()-1; i++)
        raw += (chtype) (append[i] | COLOR_PAIR(COLOR_BLACK));
}

inline void operator += (std::vector<chtype> &raw, const char *append){
    if(append != nullptr){
        int i = 0;
        while(1){
            printf("ascii: %u\n", append[i]);
            if(append[i] == 0)
                break;
            else
                raw += (chtype) (append[i] | COLOR_PAIR(COLOR_BLACK));
            i++;
        }
    }
}

inline void operator += (std::vector<chtype> &raw, char append){
    raw += (chtype) (append | COLOR_PAIR(COLOR_BLACK));
}

#endif //TETRIS_CURSES_STRING_H
