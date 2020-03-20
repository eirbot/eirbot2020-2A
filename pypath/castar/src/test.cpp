#include "test.hpp"

using namespace std;

bool is_in_circle(Coordinates pos, Circle c){
    return sqrt(square(c.pos.x-pos.x)+square(c.pos.y-pos.y)) < c.diameter/2;
}

void board_visualize(Field board, std::vector<Coordinates> path){
    Circle robot;
    robot.diameter =2;
    robot.pos.x = robot.pos.y = 10;
    Coordinates pos;
    bool is_path;
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

            is_path =0;
            for (size_t i = 0; i < path.size(); i++)
            {
                if (path[i].x == pos.x and path[i].y == pos.y)
                {
                    is_path = 1;
                    break;
                }
            }

            if (is_in_circle(pos,robot))
            {
                std::cout << "R";
            }else if(is_path){
                std::cout << "+";
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

void test(void){
    Field board = Field(200);
    Castar astar = Castar();
    std::cout << "Launching !" << endl;
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
    r.pos.x = 80;
    r.pos.y = 80;
    r.dim.height = 2;
    r.dim.width = 200;
    board.add_obsctacle(r);


    vector<Coordinates> path;
    Coordinates start, end;
    start = {10,10};
    end = {290,160};
    auto t_start = chrono::high_resolution_clock::now(); 
    ios_base::sync_with_stdio(false); 
    astar.find_path(start,end, board , &path);
    auto t_end = chrono::high_resolution_clock::now(); 
  
    // Calculating total time taken by the program. 
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(t_end - t_start).count(); 
  
    time_taken *= 1e-9; 
  
    cout << "Time taken by program is : " << fixed  
         << time_taken << setprecision(9); 
    cout << " sec" << endl; 
    board_visualize(board, path);

}