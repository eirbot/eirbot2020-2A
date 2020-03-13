#include "castar.hpp"
#include "math.h"

castar::castar(): field()
{
}

castar::~castar()
{
}

err_t castar::find_path(Node start, Node end, std::vector<Node> *final_path){
    

    Size field_dim = field.get_dimensions();
    if (start.pos.x > (int32_t)field_dim.width or 
        start.pos.x < 0 or 
        start.pos.y > (int32_t)field_dim.height or 
        start.pos.y < 0)
    {
        return ERR_ARGS_OUT_OF_RANGE;
    }

    if(!field.is_possible(end.pos)){
        return ERR_GOAL_IS_BLOCKED;
    }

    std::vector<Node> open_list;
    std::vector<Node> close_list;
    open_list.push_back(start);
    
    Node current;
    while (open_list.size())
    {
        current = open_list.back();
        open_list.pop_back();
        if(current.pos.x == end.pos.x and current.pos.y == end.pos.y){
            //Find algo 
        }
        for(int8_t x=-1; x<1; x++){
            for(int8_t y=-1; y<1; y++){
                Node new_node;
                new_node.pos.x = limit(current.pos.x+x,0,(int32_t)field_dim.width-1);
                new_node.pos.y = limit(current.pos.y+y,0,(int32_t)field_dim.height-1);
                new_node.d_cost = current.d_cost +1;
                for( std::vector<Node>::iterator iter = close_list.begin(); iter != close_list.end(); ++iter ){
                    if(iter->pos.x == new_node.pos.x and iter->pos.y == new_node.pos.y){
                        continue;
                    }
                }

                // Si node deja connue mais avec un poid plus lourd alors on traite
                for( std::vector<Node>::iterator iter = open_list.begin(); iter != open_list.end(); ++iter ){
                    if(iter->pos.x == new_node.pos.x and iter->pos.y == new_node.pos.y){
                        if (iter->d_cost < new_node.d_cost) // FIXME is that right ?
                        {
                            continue;
                        }
                        /*else
                        {
                            break; // TODO 2 obstacles au méme endroit?
                        }*/
                    }
                }
                
                new_node.h_cost = new_node.d_cost + distance(new_node.pos, end.pos);
                
                

            }
        }

        close_list.push_back(current);
    }
    

    // Open set is empty but goal was never reached
    return ERR_NO_PATH;
}

err_t castar::smooth_path(std::vector<Node> path, std::vector<Node> *final_path){
    return NO_ERROR;
}

float castar::distance(Coordinates a, Coordinates b){
    return sqrt(square(a.x-b.x)+square(a.y-b.y));
}