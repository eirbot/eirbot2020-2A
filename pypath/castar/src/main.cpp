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


// BINDINGS

#ifndef TEST

PYBIND11_MODULE(castar, m) {
    py::class_<Coordinates>(m, "Coordinates")
    .def_readwrite("x", &Coordinates::x)
    .def_readwrite("y", &Coordinates::y)
    .def(py::init<const int, const int>())
    .def("__repr__",
        [](const Coordinates &crd) {
            return "x:" + to_string(crd.x) + "|y:" + to_string(crd.y);
        }
    );
    py::class_<Size>(m, "Size")
    .def_readwrite("width", &Size::width)
    .def_readwrite("height", &Size::height)
    .def(py::init<const int, const int>())
    .def("__repr__",
        [](const Size &i) {
            return "Width:" + to_string(i.width) + "|Height:" + to_string(i.height);
        }
    );
    py::class_<Rectangle>(m, "Rectangle")
    .def_readwrite("pos", &Rectangle::pos)
    .def_readwrite("dim", &Rectangle::dim)
    .def(py::init<const Coordinates, const Size>())
    .def("__repr__",
        [](const Rectangle &rec) {
            return "Pos:" + to_string(rec.pos.x) + "|" + to_string(rec.pos.y) + 
            " - Size:" + to_string(rec.dim.width) + "|" + to_string(rec.dim.height);
        }
    );
    py::class_<Field>(m, "Field")
    .def(py::init<>())
    .def(py::init<const unsigned int>())
    .def(py::init<const unsigned int, const unsigned int, const unsigned int>())
    .def("add_obstacle", &Field::add_obsctacle)
    .def("delete_obstacle", &Field::delete_obstacle)
    .def("__repr__",
        [](Field &fd) {
            return  "Size:" + to_string(fd.get_dimensions().width) + "|" + to_string(fd.get_dimensions().height)
            + " - Nb obstacle: " + to_string(fd.get_number_of_obstacle());
        }
    );

    py::class_<Castar>(m, "Castar")
    .def(py::init<>())
    .def("find_path", [](Castar &cas, Coordinates start, Coordinates end, Field field) {
        std::vector<Coordinates> final_path;
        int return_code = cas.find_path(start,end,field,&final_path); 
        return std::make_tuple(return_code, final_path); 
        }
    )
    .def("simplify_path", [](Castar &cas, std::vector<Coordinates> path) {
        std::vector<Coordinates> final_path;
        int return_code = cas.simplify_path(path,&final_path); 
        return std::make_tuple(return_code, final_path); 
        }
    )
    .def("find_path_simplified", [](Castar &cas, Coordinates start, Coordinates end, Field field) {
        std::vector<Coordinates> final_path;
        int return_code = cas.find_path_simplified(start,end,field,&final_path); 
        return std::make_tuple(return_code, final_path); 
        }
    );


    m.doc() = "Library implementing the A* pathfinding algorithme"; // optional module docstring
}
#else

int main(int argc, char const *argv[])
{
    test();
    return 0;
}

#endif


