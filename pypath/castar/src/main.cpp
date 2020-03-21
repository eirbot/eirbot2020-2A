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
vector<Coordinates> astar(Coordinates start, Coordinates end){
    
    Coordinates start_coordinates;
    Coordinates end_coordinates;
    vector<Coordinates> path;
    err_t error = castar.find_path(start_coordinates, end_coordinates, board, &path);
    if (error)
    {
        throw runtime_error("Astar error: no path ?");
    }
    return path;
    
}

// BINDINGS

#ifndef TEST


PYBIND11_MODULE(castar, m) {
    py::class_<Coordinates>(m, "Coordinates")
    .def_readwrite("x", &Coordinates::x)
    .def_readwrite("y", &Coordinates::y);

    m.doc() = "Library implementing the A* pathfinding algorithme"; // optional module docstring
    m.def("add_obstacle", &add_obstacle, "Add a obtacle to the board", py::arg("rectangle_obstacle"));
    m.def("astar", &astar, "The A* function precise to the cm", py::arg("Start"), py::arg("End"));
    //m.def("test2", &test, "The A* function precise to the cm");
}
#else

int main(int argc, char const *argv[])
{
    test();
    return 0;
}

#endif


