#include "castar.hpp"
#include "math.h"
#include "stdio.h"
#include <queue>

castar::castar()
{
}

castar::~castar()
{
}

err_t castar::find_path(Node start, Node end, Field field, std::vector<Node> *final_path)
{

    Size field_dim = field.get_dimensions();
    if (start.pos.x > (int32_t)field_dim.width - 1 or
        start.pos.x < 0 or
        start.pos.y > (int32_t)field_dim.height - 1 or
        start.pos.y < 0)
    {
        return ERR_ARGS_OUT_OF_RANGE;
    }

    if (!field.is_possible(end.pos))
    {
        return ERR_GOAL_IS_BLOCKED;
    }

    std::vector<Node> open_list;
    std::vector<Node> close_list;
    start.g_cost = start.f_cost = start.h_cost = 0;
    open_list.push_back(start);
    Node current;
    Node tmp;
    while (!open_list.empty())
    {
        tmp = open_list.back();
        size_t x = 0;
        for (size_t i = 0; i < open_list.size(); i++)
        {
            if (open_list[i].f_cost < tmp.f_cost)
            {
                tmp = open_list[i];
                x = i;
            }
        }

        current = tmp;
        open_list.erase(open_list.begin() + x);
        if (current.pos.x == end.pos.x and current.pos.y == end.pos.y)
        {
            printf("Done! \n");
            return NO_ERROR;
        }
        for (int8_t x = -1; x <= 1; x++)
        {
            for (int8_t y = -1; y <= 1; y++)
            {
                if (x == 0 and y == 0)
                {
                    continue;
                }
                Node new_node;
                new_node.pos.x = current.pos.x + x;
                new_node.pos.y = current.pos.y + y;
                if (new_node.pos.x > (int32_t)field_dim.width - 1 or
                    new_node.pos.x < 0 or
                    new_node.pos.y > (int32_t)field_dim.height - 1 or
                    new_node.pos.y < 0)
                {
                    continue;
                }

                if (!field.is_possible(new_node.pos))
                {
                    continue;
                }

                for (std::vector<Node>::iterator iter = close_list.begin(); iter != close_list.end(); ++iter)
                {
                    if (iter->pos.x == new_node.pos.x and iter->pos.y == new_node.pos.y)
                    {
                        continue;
                    }
                }

                new_node.g_cost = current.g_cost + distance(new_node.pos, current.pos);
                new_node.h_cost = distance(new_node.pos, end.pos);
                new_node.f_cost = new_node.h_cost + new_node.g_cost;

                int replaced = 0;
                int seen = 0;
                // Si node deja connue mais avec un poid plus faible alors on actualise le cout
                for (std::vector<Node>::iterator iter = open_list.begin(); iter != open_list.end(); ++iter)
                {
                    if (iter->pos.x == new_node.pos.x and iter->pos.y == new_node.pos.y)
                    {
                        seen = 1;
                        if (iter->f_cost < new_node.f_cost)
                        {
                            iter->f_cost = new_node.f_cost;
                            iter->g_cost = new_node.g_cost;
                            replaced = 1;
                            break;
                        }
                    }
                }
                if (seen)
                {
                    continue;
                }
                if (!replaced)
                {
                    open_list.push_back(new_node);
                    replaced = 0;
                }
            }
        }
        close_list.push_back(current);
    }

    // Open set is empty but goal was never reached
    return ERR_NO_PATH;
}

err_t castar::smooth_path(std::vector<Node> path, std::vector<Node> *final_path)
{
    return NO_ERROR;
}

float castar::distance(Coordinates a, Coordinates b)
{
    return sqrt(square(a.x - b.x) + square(a.y - b.y));
}