#ifndef H_FIELD
#define H_FIELD

#include <vector>

   
class Coordinates
{
    public:
        Coordinates(){}
        Coordinates(int _x, int _y){
            x=_x;
            y=_y;
        }
        int x;
        int y; 
};

class Size
{
    public:
        Size(){}
        Size(int _width, int _height){
            width=_width;
            height=_height;
        }
    int width;
    int height; 
};


class Circle{
    public:
        Circle(){}
        Circle(Coordinates _pos, int _diameter){
            pos = _pos;
            diameter = _diameter;
        }
    Coordinates pos;
    int diameter;
};

class Rectangle{
    public:
        Rectangle(){}
        Rectangle(Coordinates _pos, Size _dim) {
            pos = _pos;
            dim = _dim;
        }
        Coordinates pos;
        Size dim;
};



class Field
{
private:
    Size size;
    Circle robot;
    std::vector<Rectangle> list_obsctacles; // TODO une deuxieme liste pour les cercles?
    // on fait une classe obstacle ?
public:
    Field();
    Field(unsigned int robot_diameter_mm);
    Field(unsigned int width_mm, unsigned int height_mm, unsigned int robot_diameter_mm);
    ~Field();
    Size get_dimensions(void);
    void add_obsctacle(Rectangle obsctacle);
    bool delete_obstacle(Coordinates obstacle_position);
    bool is_possible(Coordinates robot_pos);
    bool intersect(Circle c, Rectangle r);
    // TODO load_chunk();
};



#endif