#include "castar.hpp"
#include "math.h"
#include "stdio.h"
#include <queue>
#include <algorithm>

Castar::Castar()
{
}

Castar::~Castar()
{
}

void Castar::reconstruct_path(std::vector<Node> *valid_paths, std::vector<Coordinates> *real_path, Node end)
{
    Node tmp = end;
    while (tmp.pos.x != tmp.came_from.x or tmp.pos.y != tmp.came_from.y)
    {
        for (size_t i = 0; i < valid_paths->size(); i++)
        {
            if ((*valid_paths)[i].pos.x == tmp.came_from.x and (*valid_paths)[i].pos.y == tmp.came_from.y)
            {
                real_path->push_back(tmp.pos);
                tmp = (*valid_paths)[i];
                break;
            }
        }
    }
    real_path->push_back(tmp.pos);
    std::reverse(real_path->begin(), real_path->end());
}

int Castar::find_path(Coordinates start, Coordinates end, Field field, std::vector<Coordinates> *final_path)
{

    Size field_dim = field.get_dimensions();
    if (start.x > (int)field_dim.width - 1 or
        start.x < 0 or
        start.y > (int)field_dim.height - 1 or
        start.y < 0)
    {
        return ERR_ARGS_OUT_OF_RANGE;
    }

    if (!field.is_possible(end))
    {
        return ERR_GOAL_IS_BLOCKED;
    }

    std::vector<Node> open_list;
    std::vector<Node> close_list;

    int close_nodes[field_dim.width][field_dim.height];
    for (int i = 0; i < field_dim.width; i++)
    {
        for (int j = 0; j < field_dim.height; j++)
        {
            close_nodes[i][j] = 0;
        }
    }

    // Convert Coordinates to nodes
    Node start_node;
    start_node.pos = start;
    Node end_node;
    end_node.pos = end;

    start_node.g_cost = start_node.f_cost = start_node.h_cost = 0;
    start_node.came_from = start_node.pos;
    open_list.push_back(start_node);
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
        if (current.pos.x == end_node.pos.x and current.pos.y == end_node.pos.y)
        {
            reconstruct_path(&close_list, final_path, current);
            return NO_ERROR;
        }
        for (char x = -1; x <= 1; x++)
        {
            for (char y = -1; y <= 1; y++)
            {
                if (x == 0 and y == 0)
                {
                    continue;
                }
                Node new_node;
                new_node.pos.x = current.pos.x + x;
                new_node.pos.y = current.pos.y + y;
                if (new_node.pos.x > (int)field_dim.width - 1 or
                    new_node.pos.x < 0 or
                    new_node.pos.y > (int)field_dim.height - 1 or
                    new_node.pos.y < 0)
                {
                    continue;
                }

                if (!field.is_possible(new_node.pos))
                {
                    continue;
                }

                if (close_nodes[new_node.pos.x][new_node.pos.y])
                {
                    continue;
                }
                new_node.g_cost = current.g_cost + distance(new_node.pos, current.pos); // slower but the best path
                //new_node.g_cost = current.g_cost + 1; // faster but not the most efficient path
                new_node.h_cost = distance(new_node.pos, end_node.pos);
                new_node.f_cost = new_node.h_cost + new_node.g_cost;
                new_node.came_from = current.pos;

                int replaced = 0;
                int seen = 0;
                // Si node deja connue mais avec un poid plus fort alors on actualise le cout
                for (std::vector<Node>::iterator iter = open_list.begin(); iter != open_list.end(); ++iter)
                {
                    if (iter->pos.x == new_node.pos.x and iter->pos.y == new_node.pos.y)
                    {
                        seen = 1;
                        if (iter->f_cost > new_node.f_cost)
                        {
                            iter->f_cost = new_node.f_cost;
                            iter->g_cost = new_node.g_cost;
                            iter->came_from = new_node.came_from;
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
                }
            }
        }
        close_nodes[current.pos.x][current.pos.y] = 1;
        close_list.push_back(current);
    }

    // Open set is empty but goal was never reached
    return ERR_NO_PATH;
}

int Castar::simplify_path(std::vector<Coordinates> path, std::vector<Coordinates> *final_path)
{
    if (path.size() == 0)
    {
        return ERR_NO_PATH;
    }
    else if (path.size() == 1)
    {
        final_path->push_back(path.front());
        return NO_ERROR;
    }

    Coordinates previous_pos = path.front();
    Coordinates next = path[1];
    Coordinates previous_deriv = {next.x - previous_pos.x, next.y - previous_pos.y};

    Coordinates deriv;
    std::vector<Coordinates>::iterator iter = path.begin();
    std::advance(iter, 1);
    while (iter != path.end())
    {
        deriv.x = iter->x - previous_pos.x;
        deriv.y = iter->y - previous_pos.y;

        if (previous_deriv.x != deriv.x or previous_deriv.y != deriv.y)
        {
            final_path->push_back(*iter);
        }

        previous_deriv = deriv;
        previous_pos.x = iter->x;
        previous_pos.y = iter->y;
        iter++;
    }
    if (final_path->size() == 0)
    {
        final_path->push_back(path.back());
    }
    else if (path.back().x != final_path->back().x or path.back().y != final_path->back().y)
    {
        final_path->push_back(path.back());
    }

    return NO_ERROR;
}

int Castar::find_path_simplified(Coordinates start, Coordinates end, Field field, std::vector<Coordinates> *final_path){
    std::vector<Coordinates> path;
    int return_code = find_path(start, end, field, &path);
    simplify_path(path, final_path);
    return return_code;
}



float Castar::distance(Coordinates a, Coordinates b)
{
    return sqrt(square(a.x - b.x) + square(a.y - b.y));
}