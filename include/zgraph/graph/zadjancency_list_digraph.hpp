#pragma once
#ifndef ZGRAPH_GRAPH_ZADJACENCY_LIST_DIGRAPH_HPP
#define ZGRAPH_GRAPH_ZADJACENCY_LIST_DIGRAPH_HPP

#include <cstdint>
#include <iostream>

#include <map>
#include <set>
#include <array>

#include "zgraph/graph/detail/zgraph_items.hpp"
#include "zgraph/graph/detail/zgraph_maps.hpp"
#include "zgraph/graph/zgraph_base.hpp"
#include "zgraph/detail/zvalue_map.hpp"


#include <range/v3/view/transform.hpp>

namespace zgraph 
{
 


    template<bool is_mutligraph, bool allow_self_loop>
    class zadjacency_list_digraph;

    template<bool is_mutligraph, bool allow_self_loop>
    class zgraph_type_traits<zadjacency_list_digraph<is_mutligraph, allow_self_loop>>{
    public:

        using is_directed_t = std::true_type;
        using is_mutligraph_t = std::integral_constant<bool, is_mutligraph>;
        using allow_self_loops_t = std::integral_constant<bool, allow_self_loop>;

        using graph_t = zadjacency_list_digraph<is_mutligraph, allow_self_loop>;
        using node_index_t = uint64_t;
        using edge_index_t = uint64_t;

        template<class value_t>
        class node_map : public zassociative_graph_item_map<graph_t,znode_tag,std::map<node_index_t, value_t>>{
            using base_t = zassociative_graph_item_map<graph_t,znode_tag,std::map<node_index_t, value_t>>;
            using base_t::base_t;
        };

        template<class value_t>
        class edge_map : public zassociative_graph_item_map<graph_t,zedge_tag,std::map<edge_index_t, value_t>>{
            using base_t = zassociative_graph_item_map<graph_t,zedge_tag,std::map<edge_index_t, value_t>>;
            using base_t::base_t;
        };

        class node_set : public std::set<node_index_t>{
        public:
            node_set(const graph_t & g){

            }
        };


    };



    template<bool is_mutligraph, bool allow_self_loop>
    class zadjacency_list_digraph : public zgraph_base<zadjacency_list_digraph<is_mutligraph, allow_self_loop>>
    {
    public:
        using node_index_t = uint64_t;
        using edge_index_t = uint64_t;
        using adjacency_t = std::map<node_index_t, edge_index_t>;

        using endpoint_array_t = std::array<node_index_t, 2>;


        zadjacency_list_digraph()
        : m_next_edge_index(0)
        {

        }
        
        // 
        bool add_node(const node_index_t node){
            return m_out_adjacencies.try_emplace(node, adjacency_t()).second;
        }

        edge_index_t add_edge(const node_index_t u, const node_index_t v){

            bool new_u = this->add_node(u);
            bool new_v = this->add_node(v);

            if(new_u || new_v){
                // edge cannot exist
                const auto edge_index = m_next_edge_index++;
                m_edges.emplace(edge_index, endpoint_array_t{u, v});
                this->get_out_adjacency(u).emplace(edge_index, v);
                this->get_in_adjacency(v).emplace(edge_index, u);
                return edge_index;
            } else{
                // edge might exist
                auto & adj_u =  this->get_out_adjacency(u);
                if(auto find_v = adj_u.find(v); find_v != adj_u.end()){
                    // edge does exist
                    return find_v->second;
                } else{
                    // edge does not exist
                    const auto edge_index = m_next_edge_index++;
                    m_edges.emplace(edge_index, endpoint_array_t{u, v});
                    adj_u.emplace(edge_index, v);
                    this->get_in_adjacency(v).emplace(edge_index, u);
                    return edge_index;
                }
            }
        }

        // api
        decltype(auto) adjacency(const node_index_t node)const{
            return get_out_adjacency(node);
        }
        decltype(auto) in_adjacency(const node_index_t node)const{
            return get_in_adjacency(node);
        }



        // node iter


        auto num_nodes()const{
            return m_out_adjacencies.size();
        }

        auto num_edges() const{
            return m_edges.size();
        }

        auto nodes() const{
            return m_out_adjacencies | ranges::views::keys;
        }

        auto edges() const{
            return m_edges | ranges::views::keys;
        }
        const auto & endpoints(const edge_index_t & edge) const{
            return m_edges.find(edge)->second;
        }
    private:
        auto & get_out_adjacency(const node_index_t node){
            return m_out_adjacencies.find(node)->second;
        }
        const auto & get_out_adjacency(const node_index_t node) const{
            return m_out_adjacencies.find(node)->second;
        }

        auto & get_in_adjacency(const node_index_t node){
            return m_in_adjacencies.find(node)->second;
        }
        const auto & get_in_adjacency(const node_index_t node) const{
            return m_in_adjacencies.find(node)->second;
        }

        edge_index_t m_next_edge_index;

        std::map<node_index_t, adjacency_t> m_out_adjacencies;
        std::map<node_index_t, adjacency_t> m_in_adjacencies;
        std::map<edge_index_t, endpoint_array_t> m_edges;
    };






} // end namespace zgraph


#endif // ZGRAPH_GRAPH_ZADJACENCY_LIST_DIGRAPH_HPP