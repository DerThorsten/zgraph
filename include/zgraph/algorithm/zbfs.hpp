#pragma once
#ifndef ZGRAPH_ALGORITHM_ZBFS_HPP
#define ZGRAPH_ALGORITHM_ZBFS_HPP


#include <range/v3/view/filter.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/view/transform.hpp>

#include "zgraph/graph/zgraph_base.hpp"

#include <queue>

namespace zgraph 
{

    
    template<
        class derived,
        class start_nodes_t,
        class pred_map_t,
        class node_discovery_callback_t,
        class adjacency_filter_t
    >
    void bfs(
        const zgraph_base<derived> & g,
        start_nodes_t && start_nodes,
        pred_map_t && pred_map, 
        node_discovery_callback_t && discovery_callback,
        adjacency_filter_t && filter
    ){
        using graph_t = derived;
        using node_index_t = typename zgraph_base<derived>::node_index_t;
        using queue_t = std::queue<node_index_t>;
        using node_set_t = typename graph_t::node_set;


        // cast from crtp base to actual graph
        const auto & graph = g.derived_cast();

        // iterate over the adjacency and yield a triple: <from_node, to_node, edge>
        auto adjacency_uve = [&](const node_index_t u){
            return graph.out_adjacency(u) | ranges::view::transform([u](auto && kv){
                return std::make_tuple(u,kv.first, kv.second);
            });
        };



        // exclude nodes from search via lambda 
        auto adjancey_filter = ranges::views::filter(filter);

     

        // node-set to store discovered nodes
        node_set_t discovered(graph);

        // filter function to to check if a node is *not* discovered
        auto not_discovered =  ranges::view::filter([&](const auto & uve_tripple){
            return discovered.find(std::get<1>(uve_tripple)) == discovered.end();
        });

        // create queue and put start node(s) on queue
        queue_t queue;
        for(auto && start_node : start_nodes){
            queue.push(start_node);
            discovered.insert(start_node);
            if(!discovery_callback(start_node)){
                return;
            }
            pred_map[start_node] = start_node;
        }

        while(!queue.empty())
        {
            const auto node = queue.front();
            queue.pop();

            if(!discovery_callback(node)){
                break;
            }

            for(auto [u, v, e] : adjacency_uve(node) | not_discovered | adjancey_filter)  {
                std::cout <<"   v "<<v<<" via e "<< e <<"\n";
                discovered.insert(v);
                queue.push(v);
                pred_map[v] = u;
            }
        }
    }


    template<class derived>
    void bfs(
        const zgraph_base<derived> & g,
        const typename zgraph_base<derived>::node_index_t & start_node
    ){
        using node_index_t = typename zgraph_base<derived>::node_index_t;
        using pred_map_t = typename derived:: template node_map<node_index_t>;

        pred_map_t pred_map(g.derived_cast());

        bfs(g, std::array<node_index_t,1>{start_node},
            pred_map,
            [](auto && node ){return true;},
            [](auto && uve ){return true;}
        );   
    }



    // // todo, consider sparse pred mapdd
    // template<class graph_t>
    // class zbfs{
    // public:

    //     using node_index_t = typename graph_t::node_index_t;
    //     using queue_t = std::queue<node_index_t>;
    //     using node_set_t = typename graph_t::node_set;

    //     using pred_map_t = typename derived:: template node_map<node_index_t>;



    //     zbfs(const graph_t & graph)
    //     : m_graph(graph){
    //     }
    //     const auto & graph()const{
    //         return m_graph;
    //     }

    //     template<class start_nodes_t>
    //     void operator()(
    //         start_nodes_t && start_nodes
    //     )const{

    //     };
    // private:
    //     const graph_t & m_graph;
    // };




    template<
        class derived,
        class start_nodes_t,
        class pred_map_t,
        class node_discovery_callback_t,
        class discovered_nodes_set_t,
        class node_discovery_callback_t && node_discovery_callback,
        class process_node_early_callback_t && process_node_early_callback,
    >
    void sparse_bfs_impl(
        const zgraph_base<derived> & base_g,
        const start_nodes_t & start_nodes
        pred_map_t & pred_map,
        discovered_nodes_set_t & discovered_nodes_set,
        node_discovery_callback_t && node_discovery_callback,

    ){

        using node_index_t = typename graph_t::node_index_t;
        using queue_t = std::queue<node_index_t>;
        using node_set_t = typename graph_t::node_set;

        // get instance from crtp - base
        const auto & g = base_g.derived_cast();


        // filter function to to check if a node is *not* discovered
        auto not_discovered =  ranges::view::filter([&](const auto & uve_tripple){
            return discovered_nodes_set.find(std::get<1>(uve_tripple)) == discovered_nodes_set.end();
        });


        // initialize from start nodes
        queue_t q;
        for(auto && start_node : start_nodes){
            queue.push(start_node);
            discovered_nodes_set.insert(start_node);
            pred_map[start_node] = start_node;
            if(!node_discovery_callback(start_node)){
                return;
            }
        }

        auto exit = false;
        while(!queue.empty() || exit)
        {
            const auto node_u = queue.front();
            queue.pop();

            if(!process_node_early_callback(node)){
                break;
            }

            for(auto [u, v] : adjacency(node) | not_discovered )  {
                discovered_set.insert(v);
                if(!node_discovery_callback(node)){
                    exit = true
                    break;
                }
                queue.push(v);
                pred_map[v] = u;
            }

            if(!process_node_late_callback(node) || exit){
                break;
            }
            
        }
    }


} // end namespace zgraph


#endif // ZGRAPH_GRAPH_ZUGRAPH_BASE_HPP