#include <stdio.h>
#include "castar.hpp"
#include "field.hpp"
#include <iostream>
#include "math.h"
using namespace std;

bool is_in_circle(Coordinates pos, Circle c){
    return sqrt(square(c.pos.x-pos.x)+square(c.pos.y-pos.y)) < c.diameter/2;
}

void board_visualize(Field board){
    Circle robot;
    robot.diameter =2;
    robot.pos.x = robot.pos.y = 10;
    Coordinates pos;
    for (size_t j = 0; j <= 200; j+=2)
    {
        for (size_t i = 0; i <= 300; i+= 2)
        {
            if (!j or j == 200)
            {
                std::cout << "-";
                continue;
            }
            
            else if (!i or i == 300)
            {
                std::cout << "|";
                continue;
            }
            
            pos.x = i;
            pos.y = j;
            if (is_in_circle(pos,robot))
            {
                std::cout << "R";
            }
            else if (!board.is_possible(pos))
            {
                std::cout << "X";
            }else
            {
                std::cout << " ";
            }

            
        }
        std::cout << "\n";
    }
}

int main(int argc, char const *argv[])
{
    std::cout << "Launching !" << endl;
    Field board = Field(200);
    Rectangle r;
    r.pos.x = 60;
    r.pos.y = 20;
    r.dim.height = 40;
    r.dim.width = 2;
    board.add_obsctacle(r);
    r.pos.x = 150;
    r.pos.y = 40;
    r.dim.height = 40;
    r.dim.width = 10;
    board.add_obsctacle(r);

    board_visualize(board);

    std::cout << "A* !" << endl;
    castar astar = castar();
    vector<Node> path;
    Node start, end;
    start.pos = {10,10};
    end.pos = {120,10};
    std::cout << astar.find_path(start,end, board , &path) << endl;
    return 0;
}
