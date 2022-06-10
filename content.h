//
// Created by YLW20 on 2022/6/9.
//

#ifndef TETRIS_CONTENT_H
#define TETRIS_CONTENT_H

#include "color.h"

struct _content{
    char  filled;
    _RGB  color;
    _content(){
        filled = 0;
        color  = {0, 0, 0};
    }
};

#endif //TETRIS_CONTENT_H
