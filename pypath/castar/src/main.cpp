#include <stdio.h>
#include "castar.hpp"
#include "field.hpp"
#include <iostream>

using namespace std;



int main(int argc, char const *argv[])
{
    cout << "Launching !" << endl;
    Field board = Field(20);
    Rectangle r;
    r.pos.x = 20;
    r.pos.y = 15;
    r.dim.height = 60;
    r.dim.width = 2;
    board.add_obsctacle(r);
    r.pos.x = 50;
    r.pos.y = 20;
    r.dim.height = 2;
    r.dim.width = 20;
    board.add_obsctacle(r);
    
    cout << "A* !" << endl;
    castar astar = castar();
    vector<Node> path;
    Node start, end;
    start.pos = {10,10};
    end.pos = {60,10};
    cout << astar.find_path(start,end, board , &path) << endl;
    return 0;
}
