#include "castar.hpp"
#include "field.hpp"

#ifndef TEST
#include "pybind11/pybind11.h"
#include "pybind11/stl_bind.h"
#include "pybind11/stl.h"
namespace py = pybind11;
#else
#include "test.hpp"
#endif

using namespace std;

// DEFINES
#define DEFAULT_ROBOT_DIAMETER_MM 200

// GLOBALES


Field board = Field(DEFAULT_ROBOT_DIAMETER_MM);
Castar castar = Castar();

// FUNCTIONS

void add_obstacle(Rectangle rec){

}
/***
 *  Calculate the shortest path between two points
 *  Need tuples with size 2: (X, Y)
 */
vector<Coordinates> astar(Coordinates start_coordinates, Coordinates end_coordinates){
    
    vector<Coordinates> path;
    err_t error = castar.find_path(start_coordinates, end_coordinates, board, &path);
    if (error)
    {
        throw runtime_error("Astar error: no path ?");
    }
    return path;
    
}

vector<Coordinates> simplify_path(vector<Coordinates> path){
    vector<Coordinates> simplified_path;
    err_t error = castar.simplify_path(path, &simplified_path);
    if (error)
    {
        throw runtime_error("simplify_path error");
    }
    return simplified_path;
}

vector<Coordinates> astar_simplified(Coordinates start_coordinates, Coordinates end_coordinates){
    
    return simplify_path(astar(start_coordinates, end_coordinates));
    
}

// BINDINGS

#ifndef TEST

PYBIND11_MODULE(castar, m) {
    py::class_<Coordinates>(m, "Coordinates")
    .def_readwrite("x", &Coordinates::x)
    .def_readwrite("y", &Coordinates::y)
    .def(py::init<const int32_t, const int32_t &>())
    .def("__repr__",
        [](const Coordinates &crd) {
            return "x:" + to_string(crd.x) + "|y:" + to_string(crd.y);
        }
    );

    m.doc() = "Library implementing the A* pathfinding algorithme"; // optional module docstring
    m.def("add_obstacle", &add_obstacle, "Add a obtacle to the board", py::arg("rectangle_obstacle"));
    m.def("astar", &astar, "The A* function precise to the cm", py::arg("Start"), py::arg("End"));
    m.def("simplify_path", &simplify_path, "Simplify the path, return a path with only importants points when there is a direction change", py::arg("Path"));
    m.def("astar_simplified", &astar_simplified, "The astar method combined with the simplify_path method", py::arg("Start"), py::arg("End"));


    //m.def("test2", &test, "The A* function precise to the cm");
}
#else

int main(int argc, char const *argv[])
{
    test();
    return 0;
}

#endif


