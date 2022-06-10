//
// Created by YLW20 on 2022/6/10.
//

#ifdef ENV_WINDOWS

#include <conio.h>
int os_kbhit(){
    return _kbhit();
}

#else

int os_kbhit(){
    return 1;
}

#endif

