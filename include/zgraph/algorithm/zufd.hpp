#pragma once
#ifndef ZGRAPH_ALGORITHM_ZUFD_HPP
#define ZGRAPH_ALGORITHM_ZUFD_HPP


#include <range/v3/view/filter.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/view/transform.hpp>

#include "zgraph/graph/zgraph_base.hpp"

#include <queue>

namespace zgraph 
{

    template<class graph_t>
    class zufd{
    public:
        using node_index_t = typename graph_t::node_index_t;
        using parents_map_t = typename graph_t::  template node_map<node_index_t>;
        using ranks_map_t = typename graph_t::  template node_map<std::size_t>;


        zufd(const graph_t & graph)
        :   m_graph(graph),
            m_parents_map(graph),
            m_ranks_map(graph, 0),
            m_num_sets(0)
        {
            for(auto && node : m_graph.nodes())
            {
                m_parents_map[node] = node;
                ++m_num_sets;
            }
        }

        node_index_t merge(node_index_t node_a, node_index_t node_b){
            // merge by rank
            node_a = find(node_a);
            node_b = find(node_b);
            if(m_ranks_map[node_a] < m_ranks_map[node_b]) {
                m_parents_map[node_a] = node_b;
                --m_num_sets;
                return node_b;
            }
            else if(m_ranks_map[node_a] > m_ranks_map[node_b]) {
                m_parents_map[node_b] = node_a;
                --m_num_sets;
                return node_a;
            }
            else if(node_a != node_b) {
                m_parents_map[node_b] = node_a;
                ++m_ranks_map[node_a];
                --m_num_sets;
                return node_a;
            } 
            else {
                return node_a;
            }
        }


        node_index_t find(const node_index_t node_index)const{

            // find the root
            auto root = node_index;
            while(m_parents_map[root] != root) {
                root = m_parents_map[root];
            }

            // path compression
            auto element = node_index;
            while(element != root) {
                const auto tmp = m_parents_map[element];
                m_parents_map[element] = root;
                element = tmp;
            }
            return root;
        }

        std::size_t num_sets()const{
            return m_num_sets;
        }
    private:

        const graph_t & m_graph;
        mutable parents_map_t m_parents_map;
        ranks_map_t m_ranks_map;
        std::size_t m_num_sets;
    };
   
}


#endif // ZGRAPH_ALGORITHM_ZUFD_HPP