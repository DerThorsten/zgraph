#include "pybind11/pybind11.h"
#pragma once
#include "pybind11/numpy.h"
#include "pybind11/stl.h"

#include <iostream>
#include <numeric>


// our headers
#include "zgraph/zgraph.hpp"
#include "zgraph/graph/zadjancency_list_ugraph.hpp"


// algorithms
#include "zgraph/algorithm/zbfs.hpp"


#include "xtensor-python/pyarray.hpp"     // Numpy bindings
#include "xtensor-python/pytensor.hpp"     // Numpy bindings

namespace py = pybind11;



namespace zgraph {






    // todo add check st. only integral types are considered
    template<class item_t>
    struct item_size : std::integral_constant<std::size_t, 1>{
    };

    template<class T, std::size_t N >
    struct item_size<std::array< T, N>> : public std::tuple_size<std::array<T, N>>{
    };

    template<class item_t>
    struct item_value_type {
        using type = item_t;
    };
    template<class T, std::size_t N >
    struct item_value_type<std::array< T, N>> {
        using type = T;
    };


    template<class item_t>
    using item_value_type_t = typename item_value_type<item_t>::type;

    



    template<class T, std::size_t N >
    inline auto item_as_array(const std::array<T, N>  & edge){
        return edge;
    }

    template<class T>
    inline auto item_as_array(T edge){
        return std::array<T,1>{edge};
    }












    template<class graph_t, class py_class_t>
    void def_bfs(
        py::module & py_mod,
        py_class_t & py_class, 
        const std::string & graph_cls_name)
    {


        using node_index_t = typename graph_t::node_index_t;
        using node_index_dim_t = item_size< node_index_t>;
        using node_index_value_type = item_value_type_t<node_index_t>;
        using nodes_tensor_type = xt::pytensor<node_index_value_type, node_index_dim_t::value == 1 ? 1: 2>;



        // using bfs_t = zbfs<graph_t>;
        // py::class_<bfs_t>(py_mod, (std::string("Bfs") + graph_cls_name).c_str())
        //     .def("__call__", [](
        //         const bfs_t & bfs,
        //         const nodes_tensor_type & nodes
        //     ){
        //         bfs(nodes);
        //     }, py::arg("start_nodes"))
        // ;
        // // factor
        // py_mod.def("bfs_factory", [](const graph_t & graph){
        //         return new bfs_t(graph);
        //     },
        //     py::return_value_policy::take_ownership,
        //     py::keep_alive<0, 1>(),
        //     py::arg("graph")
        // );

    }









    template<class graph_t, class py_class_t>
    void def_graph_algoritms(
        py::module & py_mod,
        py_class_t & py_class, 
        const std::string & graph_cls_name
    ){
        def_bfs<graph_t>(py_mod, py_class, graph_cls_name);       
    }


    template<class graph_t, class py_class_t>
    void def_graph_batch_api(py::module & py_mod,py_class_t & py_class, const std::string & cls_name){
     // edge "indices" can be plain scalars or a array of scalars
        // this array of scalars could be the endpoints themself
        // or for a gridgraph, the "coordinate" for the edge
        using edge_index_t = typename graph_t::edge_index_t;
        using edge_index_dim_t = item_size< edge_index_t>;
        using edge_index_value_type = item_value_type_t<edge_index_t>;


        // node "indices" can be plain scalars or a array of scalars
        //for gridgraph, the "coordinate" for the node can be the index
        using node_index_t = typename graph_t::node_index_t;
        using node_index_dim_t = item_size< node_index_t>;
        using node_index_value_type = item_value_type_t<node_index_t>;



        using edges_tensor_type = xt::pytensor<edge_index_value_type, edge_index_dim_t::value == 1 ? 1 : 2>;
        using endpoints_tensor_type = xt::pytensor<node_index_value_type, node_index_dim_t::value == 1 ? 2 : 3>;
        using nodes_tensor_type = xt::pytensor<node_index_value_type, node_index_dim_t::value == 1 ? 1: 2>;



        // numpy batch interface
        py_class.def("endpoints_array", [&](const graph_t & graph){

            std::array<int, node_index_dim_t::value == 1 ? 2 : 3> shape;
            shape[0] = graph.num_edges();
            shape[1] = 2;
            if(node_index_dim_t::value != 1){
                shape.back() = int(node_index_dim_t::value);
            }

            auto endpoints_tensor = endpoints_tensor_type::from_shape(shape);
            auto endpoints_tensor_iter = endpoints_tensor.begin();
            for(auto && uv : graph.endpoints()){
                for(auto && p : uv){
                    for(auto pp : item_as_array(p)){
                        *endpoints_tensor_iter = pp;
                        ++endpoints_tensor_iter;
                    }
                }
            }
            return endpoints_tensor;
        });


        // get all the nodes as "native" node ids
        py_class.def("nodes_array", [&](const graph_t & graph){

            std::array<int, node_index_dim_t::value == 1 ? 1 : 2> shape;
            shape[0] = graph.num_nodes();
            if(node_index_dim_t::value != 1){
                shape.back() = int(node_index_dim_t::value);
            }

            auto nodes_tensor = nodes_tensor_type::from_shape(shape);
            auto nodes_tensor_iter = nodes_tensor.begin();
            for(auto node : graph.nodes()){
                for(auto e : item_as_array(node)){
                    *nodes_tensor_iter = e;
                    ++nodes_tensor_iter;
                }
            }
            return nodes_tensor;
        });


        // get all the edges as "native" edge ids
        py_class.def("edges_array", [&](const graph_t & graph){

            std::array<int, edge_index_dim_t::value == 1 ? 1 : 2> shape;
            shape[0] = graph.num_edges();
            if(edge_index_dim_t::value != 1){
                shape.back() = int(edge_index_dim_t::value);
            }

            auto edges_tensor = edges_tensor_type::from_shape(shape);
            auto edges_tensor_iter = edges_tensor.begin();
            for(auto edge : graph.edges()){
                for(auto e : item_as_array(edge)){
                    *edges_tensor_iter = e;
                    ++edges_tensor_iter;
                }
            }
            return edges_tensor;
        });
    }


    template<class graph_t, class py_class_t>
    void def_graph_api_core(py::module & py_mod, py_class_t & py_class, const std::string & cls_name){

        using edge_index_t = typename graph_t::edge_index_t;
        using node_index_t = typename graph_t::node_index_t;
    

        py_class.def_property_readonly_static("is_directed", [](py::object /* self */) { return graph_t::is_directed_t::value; });
        py_class.def_property_readonly_static("allow_self_loops", [](py::object /* self */) { return graph_t::allow_self_loops_t::value; });
        py_class.def_property_readonly_static("is_mutligraph", [](py::object /* self */) { return graph_t::is_mutligraph_t::value; });


        py_class.def("num_edges", &graph_t::num_edges);
        py_class.def("num_nodes", &graph_t::num_nodes);
        py_class.def("endpoints", [](const graph_t & graph, const edge_index_t & edge){
            return graph.endpoints(edge);
        });


        // adjacency iter
        py_class.def("adjacency_iter",[](const graph_t & g, const node_index_t & node ){
            auto && rng = g.adjacency(node);
            return py::make_iterator<py::return_value_policy::copy>(rng.begin(), rng.end());
        },py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */);



        // loop over nodes itself
        py_class.def("node_iter",[](const graph_t & g){
            auto rng = g.nodes();
            return py::make_iterator<py::return_value_policy::automatic>(rng.begin(), rng.end());
        },py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */);



        // loop over edges themself
        py_class.def("edge_iter",[](const graph_t & g){
            auto rng = g.edges();
            return py::make_iterator<py::return_value_policy::automatic>(rng.begin(), rng.end());
        },py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */);

        py_class.def("endpoints_iter",[](const graph_t & g){
            auto rng = g.endpoints();
            return py::make_iterator<py::return_value_policy::automatic>(rng.begin(), rng.end());
        },py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */);
    }

    template<class graph_t, class py_class_t>
    void def_graph_api(py::module & py_mod, py_class_t & py_class, const std::string & cls_name){
        def_graph_api_core<graph_t, py_class_t >(py_mod, py_class, cls_name);
        def_graph_batch_api<graph_t, py_class_t>(py_mod, py_class, cls_name);
        def_graph_algoritms<graph_t, py_class_t>(py_mod, py_class, cls_name);
    }

}
