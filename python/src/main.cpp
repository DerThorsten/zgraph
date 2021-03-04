#include "pybind11/pybind11.h"

#include "xtensor/xmath.hpp"
#include "xtensor/xarray.hpp"

#define FORCE_IMPORT_ARRAY
#include "xtensor-python/pyarray.hpp"
#include "xtensor-python/pyvectorize.hpp"

#include <iostream>
#include <numeric>
#include <string>
#include <sstream>


// our headers
#include "zgraph/zgraph.hpp"
#include "zgraph/zgraph_config.hpp"

namespace py = pybind11;



namespace zgraph {


    void def_class(py::module & m);

    void def_build_config(py::module & m);

    void def_zadjacency_list_ugraph(py::module & m);

}


// Python Module and Docstrings
PYBIND11_MODULE(_zgraph , module)
{
    xt::import_numpy();

    module.doc() = R"pbdoc(
        _zgraph  python bindings

        .. currentmodule:: _zgraph 

        .. autosummary::
           :toctree: _generate

           BuildConfiguration
           MyClass
    )pbdoc";

    zgraph::def_build_config(module);
    zgraph::def_class(module);

    // graph classes
    zgraph::def_zadjacency_list_ugraph(module);

    // make version string
    std::stringstream ss;
    ss<<ZGRAPH_VERSION_MAJOR<<"."
      <<ZGRAPH_VERSION_MINOR<<"."
      <<ZGRAPH_VERSION_PATCH;
    module.attr("__version__") = ss.str().c_str();
}