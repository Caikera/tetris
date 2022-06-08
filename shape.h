//
// Created by YLW20 on 2022/6/8.
//

#ifndef TETRIS_SHAPE_H
#define TETRIS_SHAPE_H

#include <array>
#include <string>
#include <vector>

typedef std::array<std::array<char, 4>, 4>  _contents;

struct _pos{
    double c;
    double r;
};

struct _RGB{
    short R;
    short G;
    short B;
};

struct _shape{
public:
    std::string name;
    _contents   contents;
    double      center_c;
    double      center_r;
    _RGB        RGB;

    _shape(std::string name, _contents contents, double center_c, double center_r):
    name(name), contents{contents}, center_c(center_c), center_r(center_r), RGB{255, 255, 255}
    {;}
    _shape(std::string name, _contents contents, double center_c, double center_r, _RGB RGB):
            name(name), contents{contents}, center_c(center_c), center_r(center_r), RGB{RGB}
    {;}

    void rotate(){
        int       contents_num{0};
        _contents new_contents{};
        for(int i=0; i<=3; i++)
            for(int j=0; j<=3; j++){
                if(contents[i][j] == 1) {
                    contents_num++;
                    int new_c = (int) (center_c + center_r - (i + 0.5));
                    int new_r = (int) (center_r - center_c + (j + 0.5));
                    new_contents[new_r][new_c] = 1;
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
                str += contents[i][j] == 0 ? ' ' : 'o';
            str += "|\n";
        }
        str += "------\n";
        return str;
    }
};

inline std::vector<_shape> shapes_define(){
    std::vector<_shape> shapes;
    _contents contents_I= {
            std::array<char, 4>{0, 1, 0, 0},
            std::array<char, 4>{0, 1, 0, 0},
            std::array<char, 4>{0, 1, 0, 0},
            std::array<char, 4>{0, 1, 0, 0}
    };
    shapes.push_back(_shape{"I", contents_I, 2, 2, {0, 255, 255}});

    _contents contents_L= {
            std::array<char, 4>{1, 0, 0, 0},
            std::array<char, 4>{1, 0, 0, 0},
            std::array<char, 4>{1, 1, 0, 0},
            std::array<char, 4>{0, 0, 0, 0}
    };
    shapes.push_back(_shape{"L", contents_L, 1.5, 1.5, {255, 170, 1}});

    _contents contents_RL= {
            std::array<char, 4>{0, 0, 1, 0},
            std::array<char, 4>{0, 0, 1, 0},
            std::array<char, 4>{0, 1, 1, 0},
            std::array<char, 4>{0, 0, 0, 0}
    };
    shapes.push_back(_shape{"RL", contents_RL, 1.5, 1.5, {0, 0, 255}});

    _contents contents_Z= {
            std::array<char, 4>{0, 1, 1, 0},
            std::array<char, 4>{1, 1, 0, 0},
            std::array<char, 4>{0, 0, 0, 0},
            std::array<char, 4>{0, 0, 0, 0}
    };
    shapes.push_back(_shape{"Z", contents_Z, 1.5, 1.5, {255, 0, 0}});

    _contents contents_RZ= {
            std::array<char, 4>{1, 1, 0, 0},
            std::array<char, 4>{0, 1, 1, 0},
            std::array<char, 4>{0, 0, 0, 0},
            std::array<char, 4>{0, 0, 0, 0}
    };
    shapes.push_back(_shape{"RZ", contents_RZ, 1.5, 1.5, {0, 255, 0}});

    _contents contents_O= {
            std::array<char, 4>{0, 0, 0, 0},
            std::array<char, 4>{0, 1, 1, 0},
            std::array<char, 4>{0, 1, 1, 0},
            std::array<char, 4>{0, 0, 0, 0}
    };
    shapes.push_back(_shape{"O", contents_O, 2, 2, {255, 255, 0}});

    _contents contents_T= {
            std::array<char, 4>{0, 1, 0, 0},
            std::array<char, 4>{0, 1, 1, 0},
            std::array<char, 4>{0, 1, 0, 0},
            std::array<char, 4>{0, 0, 0, 0}
    };
    shapes.push_back(_shape{"T", contents_T, 1.5, 1.5, {151, 0, 255}});

    return shapes;
}

#endif //TETRIS_SHAPE_H
