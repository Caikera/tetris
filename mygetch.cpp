//
// Created by YLW20 on 2022/6/10.
//
#include "OS.h"
#include "curses.h"
#ifdef ENV_WINDOWS

int getch_noblock(){
    if (os_kbhit()) {
        return getch();
    }
    else
        return -1;
}
#else
int getch_noblock(){
    if (os_kbhit())
        return _getch();
    else
        return -1;
}
#endif
