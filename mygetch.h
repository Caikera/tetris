//
// Created by YLW20 on 2022/6/10.
//

#ifndef TETRIS_MYGETCH_H
#define TETRIS_MYGETCH_H

int getch_noblock();

inline int mygetch(){
    return getch_noblock();
}

#endif //TETRIS_MYGETCH_H
