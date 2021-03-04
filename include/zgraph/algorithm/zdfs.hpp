#pragma once
#ifndef ZGRAPH_ALGORITHM_ZDFS_HXX
#define ZGRAPH_ALGORITHM_ZDFS_HXX


#include <range/v3/view/filter.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/view/transform.hpp>

#include "zgraph/graph/zgraph_base.hpp"

#include <stack>

namespace zgraph 
{

    
    template<
        class derived,
        class start_nodes_t,
        class pred_map_t,
        class discovery_callback_t,
        class adjacency_filter_t
    >
    void dfs(
        const zgraph_base<derived> & g,
        start_nodes_t && start_nodes,
        pred_map_t && pred_map, 
        discovery_callback_t && discovery_callback,
        adjacency_filter_t && filter
    ){
        using graph_t = derived;
        using node_index_t = typename zgraph_base<derived>::node_index_t;
        using stack_t = std::stack<node_index_t>;
        using node_set_t = typename graph_t::node_set;


        // cast from crtp base to actual graph
        const auto & graph = g.derived_cast();

        // iterate over the adjacency and yield a triple: <from_node, to_node, edge>
        auto adjacency_uve = [&](const node_index_t u){
            return graph.derived_cast().out_adjacency(u) | ranges::view::transform([u](auto && kv){
                return std::make_tuple(u,kv.first, kv.second);
            });
        };

        // exclude nodes from search via lambda 
        auto adjancey_filter = ranges::views::filter(filter);

        // node-set to store discovered nodes
        node_set_t discovered(graph);

        // create stack and put start node(s) on stack
        stack_t stack;
        for(auto && start_node : start_nodes){
            stack.push(start_node);
            pred_map[start_node] = start_node;
        }

        while(!stack.empty())
        {
            const auto node = stack.top();
            stack.pop();

            // if node not discovered
            if(discovered.find(node) == discovered.end()){

                discovered.insert(node);

                if(!discovery_callback(node)){
                    break;
                }

                for(auto [u, v, e] : adjacency_uve(node) |  adjancey_filter)  {
                    std::cout <<"   v "<<v<<" via e "<< e <<"\n";
                    
                    stack.push(v);
                    pred_map[v] = u;
                }
            }

        }
    }


    template<class derived>
    void dfs(
        const zgraph_base<derived> & g,
        const typename zgraph_base<derived>::node_index_t & start_node
    ){
        using node_index_t = typename zgraph_base<derived>::node_index_t;
        using pred_map_t = typename derived:: template node_map<node_index_t>;

        pred_map_t pred_map(g.derived_cast());

        dfs(g, std::array<node_index_t,1>{start_node},
            pred_map,
            [](auto && node ){return true;},
            [](auto && uve ){return true;}
        );   
    }

} // end namespace zgraph


#endif // ZGRAPH_ALGORITHM_ZDFS_HXX