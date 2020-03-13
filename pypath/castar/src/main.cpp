#include <stdio.h>
#include "castar.hpp"
#include "field.hpp"
#include <iostream>

using namespace std;



int main(int argc, char const *argv[])
{
    cout << "Launching !" << endl;
    Field board = Field(200);
    Rectangle r;
    r.pos.x = r.pos.y = 50;
    r.dim.height = 20;
    r.dim.width = 40;
    board.add_obsctacle(r);
    Coordinates pos;
    pos.x = pos.y = 0;
    cout << board.is_possible(pos) << endl;
    pos.x = 25;
    pos.y = 31;
    cout << board.is_possible(pos) << endl;
    return 0;
}
