#ifndef H_CASTAR
#define h_CASTAR

#include <sys/types.h>
#include <vector>
#include "field.hpp"
#include "error.hpp"

#define square(_a) ((_a)*(_a))

struct Node
{
    Coordinates pos;
    Coordinates came_from;
    float g_cost;
    float h_cost;
    float f_cost;
};

class Castar
{
private:
    float distance(Coordinates a, Coordinates b);
    void reconstruct_path(std::vector<Node> *valid_paths , std::vector<Coordinates> *real_path, Node end);
public:
    Castar();
    ~Castar();
    int find_path(Coordinates start, Coordinates end, Field field, std::vector<Coordinates> *final_path);
    int simplify_path(std::vector<Coordinates> path, std::vector<Coordinates> *final_path);
    int find_path_simplified(Coordinates start, Coordinates end, Field field, std::vector<Coordinates> *final_path);

};

#endif