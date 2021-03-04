#include "pybind11/pybind11.h"
#include "pybind11/numpy.h"

#include <iostream>
#include <numeric>


// our headers
#include "zgraph/zgraph.hpp"
#include "zgraph/graph/zadjancency_list_ugraph.hpp"

namespace py = pybind11;



namespace zgraph {

    void def_zadjacency_list_ugraph(py::module & m)
    {
        using graph_t =  zadjacency_list_ugraph<int>;
        py::class_<graph_t>(m, "AdjacencyListUGraph")
            .def(py::init<>())
            .def("add_node", &graph_t::add_node, py::arg("node"), "add node to the graph")
            .def("add_edge", &graph_t::add_edge, py::arg("u"), py::arg("v"), "add edge to the graph")
            //.def(py::init<uint64_t>(),py::arg("size"))
            //.def("hello_world", &MyClass::hello_world)
        ;
    }

}
