//
// Created by YLW20 on 2022/6/8.
//

#ifndef TETRIS_SHAPE_H
#define TETRIS_SHAPE_H

#include <array>
#include <ctime>
#include <string>
#include <vector>
#include <random>
#include "content.h"

extern std::default_random_engine e;

typedef std::array<std::array<_content, 4>, 4>  _shape_contents;  // filled suggests whether a grid is filled and color suggests its color.
typedef std::array<std::array<char, 4>, 4>      _shape_filleds;   // in 4*4 grids, if [i][j] == 1, suggests the grid is filled

constexpr _shape_filleds filleds_I= {
        std::array<char, 4>{0, 1, 0, 0},
        std::array<char, 4>{0, 1, 0, 0},
        std::array<char, 4>{0, 1, 0, 0},
        std::array<char, 4>{0, 1, 0, 0}
};
constexpr _RGB color_I = {0, 255, 255};
constexpr _shape_filleds filleds_L= {
        std::array<char, 4>{1, 0, 0, 0},
        std::array<char, 4>{1, 0, 0, 0},
        std::array<char, 4>{1, 1, 0, 0},
        std::array<char, 4>{0, 0, 0, 0}
};
constexpr _RGB color_L = {255, 170, 1};
constexpr _shape_filleds filleds_RL= {
        std::array<char, 4>{0, 0, 1, 0},
        std::array<char, 4>{0, 0, 1, 0},
        std::array<char, 4>{0, 1, 1, 0},
        std::array<char, 4>{0, 0, 0, 0}
};
constexpr _RGB color_RL = {0, 0, 255};
constexpr _shape_filleds filleds_Z= {
        std::array<char, 4>{0, 1, 1, 0},
        std::array<char, 4>{1, 1, 0, 0},
        std::array<char, 4>{0, 0, 0, 0},
        std::array<char, 4>{0, 0, 0, 0}
};
constexpr _RGB color_Z =  {255, 0, 0};
constexpr _shape_filleds filleds_RZ= {
        std::array<char, 4>{1, 1, 0, 0},
        std::array<char, 4>{0, 1, 1, 0},
        std::array<char, 4>{0, 0, 0, 0},
        std::array<char, 4>{0, 0, 0, 0}
};
constexpr _RGB color_RZ =  {0, 255, 0};
constexpr _shape_filleds filleds_O= {
        std::array<char, 4>{0, 0, 0, 0},
        std::array<char, 4>{0, 1, 1, 0},
        std::array<char, 4>{0, 1, 1, 0},
        std::array<char, 4>{0, 0, 0, 0}
};
constexpr _RGB color_O =  {255, 255, 0};
constexpr _shape_filleds filleds_T= {
        std::array<char, 4>{0, 1, 0, 0},
        std::array<char, 4>{0, 1, 1, 0},
        std::array<char, 4>{0, 1, 0, 0},
        std::array<char, 4>{0, 0, 0, 0}
};
constexpr _RGB color_T =  {151, 0, 255};

constexpr std::array<_shape_filleds, 7> shape_filleds_sequence{
    filleds_I, filleds_L, filleds_RL, filleds_Z, filleds_RZ, filleds_O, filleds_T
};

constexpr std::array<_RGB, 7> shape_color_sequence{
        color_I, color_L, color_RL, color_Z, color_RZ, color_O, color_T
};

constexpr std::array<const char*, 7> shape_name_sequence{"I", "L", "RL", "Z", "RZ", "O", "T"};
constexpr std::array<double, 7> shape_center_c_sequence{2, 1.5, 1.5, 1.5, 1.5, 2, 1.5};
constexpr std::array<double, 7> shape_center_r_sequence{2, 1.5, 1.5, 1.5, 1.5, 2, 1.5};

struct _shape{
public:
    std::string       name;
    _shape_contents   contents{};
    double            center_c;
    double            center_r;

//    inline void operator= (_shape another){
//        name     = another.name;
//        contents = another.contents;
//        center_c = center_c;
//        center_r = center_r;
//    }

    _shape(){
        std::uniform_int_distribution<short>u(0,6);
        int shape_idx = u(e);
        name = std::string{shape_name_sequence[shape_idx]};
        center_c = shape_center_c_sequence[shape_idx];
        center_r = shape_center_r_sequence[shape_idx];
        for(int r=0; r<=3; r++)
            for(int c=0; c<=3; c++){
                contents[r][c].filled = shape_filleds_sequence[shape_idx][r][c];
                contents[r][c].color  = shape_color_sequence[shape_idx];
            }
    }

    _shape(const std::string &name, const _shape_filleds &filleds, double center_c, double center_r):
    name(name), center_c(center_c), center_r(center_r)
    {
        for(int i=0; i<=3; i++)
            for(int j=0; j<=3; j++)
                if(filleds[i][j] == 1){
                    contents[i][j].filled = 1;
                    contents[i][j].color = {255, 255, 255};
                }
    }
    _shape(const std::string &name, const _shape_filleds &filleds, double center_c, double center_r, const _RGB &RGB):
            name(name), center_c(center_c), center_r(center_r)
    {
        for(int i=0; i<=3; i++)
            for(int j=0; j<=3; j++)
                if(filleds[i][j] == 1){
                    contents[i][j].filled = 1;
                    contents[i][j].color = RGB;
                }
    }

    int get_toppest(){
        for(int r=0; r<=3; r++)
            for(int c=0; c<=3; c++)
                if(contents[r][c].filled == 1)
                    return r;
        return 0;
    }

    int get_bottlest(){
        for(int r=3; r>=0; r--)
            for(int c=0; c<=3; c++)
                if(contents[r][c].filled == 1)
                    return r;
        return 3;
    }

    int get_leftest(){
        for(int c=0; c<=3; c++)
            for(int r=0; r<=3; r++)
                if(contents[r][c].filled == 1)
                    return c;
        return 0;
    }

    int get_rightest(){
        for(int c=3; c>=0; c--)
            for(int r=0; r<=3; r++)
                if(contents[r][c].filled == 1)
                    return c;
        return 3;
    }

    void rotate(){
        int             contents_num{0};
        _shape_contents new_contents{};
        for(int i=0; i<=3; i++)
            for(int j=0; j<=3; j++){
                if(contents[i][j].filled == 1) {
                    contents_num++;
                    int new_c = (int) (center_c + center_r - (i + 0.5));
                    int new_r = (int) (center_r - center_c + (j + 0.5));
                    new_contents[new_r][new_c].filled = 1;
                    new_contents[new_r][new_c].color = contents[i][j].color;
                }
            }
        contents.swap(new_contents);
    }

    std::string quick_view(){
        std::string str;
        str += "------\n";
        for(int i=0; i<=3; i++) {
            str += '|';
            for (int j = 0; j <= 3; j++)
                str += contents[i][j].filled == 0 ? ' ' : 'o';
            str += "|\n";
        }
        str += "------\n";
        return str;
    }
};

inline std::vector<_shape> shapes_define(){
    std::vector<_shape> shapes;
    shapes.push_back(_shape{"I", filleds_I, 2, 2, color_I});
    shapes.push_back(_shape{"L", filleds_L, 1.5, 1.5, color_L});
    shapes.push_back(_shape{"RL", filleds_RL, 1.5, 1.5, color_RL});
    shapes.push_back(_shape{"Z", filleds_Z, 1.5, 1.5, color_Z});
    shapes.push_back(_shape{"RZ", filleds_RZ, 1.5, 1.5, color_RZ});
    shapes.push_back(_shape{"O", filleds_O, 2, 2, color_O});
    shapes.push_back(_shape{"T", filleds_T, 1.5, 1.5, color_T});
    return shapes;
}

#endif //TETRIS_SHAPE_H
