#ifndef H_FIELD
#define H_FIELD

#include <sys/types.h>
#include <vector>

   
class Coordinates
{
    public:
        Coordinates(){}
        Coordinates(int32_t _x, int32_t _y){
            x=_x;
            y=_y;
        }
        int32_t x;
        int32_t y; 
};

struct Size
{
    int32_t width;
    int32_t height; 
};


struct Circle{
    Coordinates pos;
    int32_t diameter;
};

struct Rectangle{
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
    Field(u_int32_t robot_diameter_mm);
    Field(u_int32_t width_mm, u_int32_t height_mm, u_int32_t robot_diameter_mm);
    ~Field();
    Size get_dimensions(void);
    void add_obsctacle(Rectangle obsctacle);
    bool delete_obstacle(Coordinates obstacle_position);
    bool is_possible(Coordinates robot_pos);
    bool intersect(Circle c, Rectangle r);
    // TODO load_chunk();
};



#endif