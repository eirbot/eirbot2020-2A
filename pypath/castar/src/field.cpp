#include "field.hpp"
#include "math.h"

#define square(_a) ((_a)*(_a))

Field::Field()
{
    size.height = 200;
    size.width = 300;
    robot.pos.x = robot.pos.y = 0;
    robot.diameter = 20;
}

Field::Field(u_int32_t robot_diameter_mm)
{
    size.height = 200;
    size.width = 300;
    robot.pos.x = robot.pos.y = 0;
    robot.diameter = robot_diameter_mm/10;

}

Field::Field(u_int32_t width_mm, u_int32_t height_mm, u_int32_t robot_diameter_mm){
    size.height = height_mm/10;
    size.width = width_mm/10;
    robot.pos.x = robot.pos.y = 0;
    robot.diameter = robot_diameter_mm/10;
}


Field::~Field()
{
}

Size Field::get_dimensions(void){
    return size;
}

bool Field::is_possible(Coordinates robot_pos){
    robot.pos = robot_pos;

    for (const Rectangle & obstacle : list_obsctacles)
    {
        if(!intersect(robot, obstacle)){
            return true;
        }
    }
    
    return false;
}

bool Field::intersect(Circle circle, Rectangle rect){
    Coordinates circle_distance;
    circle_distance.x = abs((int)circle.pos.x - (int)rect.pos.x);
    circle_distance.y = abs((int)circle.pos.y - (int)rect.pos.y);

    if (circle_distance.x > ((rect.dim.width + circle.diameter)/2)) { return false; }
    if (circle_distance.y > ((rect.dim.height + circle.diameter)/2)) { return false; }

    if (circle_distance.x <= (rect.dim.width/2)) { return true; } 
    if (circle_distance.y <= (rect.dim.height/2)) { return true; }

    u_int32_t cornerDistance_sq = square(circle_distance.x - rect.dim.width/2) +
                         square(circle_distance.y - rect.dim.height/2);

    return (cornerDistance_sq <= square(circle.diameter/2));
}


void Field::add_obsctacle(Rectangle obsctacle){
    list_obsctacles.push_back(obsctacle);
}
bool Field::delete_obstacle(Coordinates obstacle_position){
    for( std::vector<Rectangle>::iterator iter = list_obsctacles.begin(); iter != list_obsctacles.end(); ++iter )
    {
        if( iter->pos.x == obstacle_position.x and iter->pos.y == obstacle_position.y)
        {
            list_obsctacles.erase( iter );
            return true;
        }
    }
    return false;
}