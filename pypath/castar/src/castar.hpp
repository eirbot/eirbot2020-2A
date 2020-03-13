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
    u_int32_t d_cost;
    float h_cost;
};

class castar
{
private:
    Field field;
    template<typename T>
    T min(T val_1, T val_2);
    template<typename T>
    T max(T val_1, T val_2);
    template<typename T>
    T limit(T val, T v_min, T v_max);
    float distance(Coordinates a, Coordinates b);
public:
    castar();
    ~castar();
    err_t find_path(Node start, Node end, std::vector<Node> *final_path);
    err_t smooth_path(std::vector<Node> path, std::vector<Node> *final_path);
};

template<typename T>
T castar::min(T val_1, T val_2)
{
	return (val_1 < val_2) ? val_1 : val_2;
}

template<typename T>
T castar::max(T val_1, T val_2)
{
	return (val_1 > val_2) ? val_1 : val_2;
}

template<typename T>
T castar::limit(T val, T v_min, T v_max)
{
	return max(min(val, v_max), v_min);
}

#endif