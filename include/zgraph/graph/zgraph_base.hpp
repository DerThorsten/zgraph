#pragma once
#ifndef ZGRAPH_GRAPH_ZGRAPH_BASE_HPP
#define ZGRAPH_GRAPH_ZGRAPH_BASE_HPP



#include <range/v3/all.hpp>


namespace zgraph 
{
    template<class derived> 
    class zcrtp_base{

    public:
        using derived_t = derived;

        derived_t & derived_cast() {
            return static_cast<derived_t &>(*this);
        }
        const derived_t & derived_cast() const {
            return static_cast<const derived_t &>(*this);
        }
    };


    template<class G>
    class zgraph_type_traits;


    template<class derived> 
    class zgraph_base : public zcrtp_base<derived>{

    public:
        using derived_t = derived;
        using graph_traits_t = zgraph_type_traits<derived>;
        using node_index_t = typename graph_traits_t::node_index_t;
        using edge_index_t = typename graph_traits_t::edge_index_t;


        template<class value_t>
        using node_map = typename graph_traits_t:: template node_map<value_t>;

        template<class value_t>
        using edge_map = typename graph_traits_t:: template edge_map<value_t>;

        using node_set = typename graph_traits_t::node_set;

        bool has_edge(const node_index_t u, const node_index_t v) const {
            const auto & adj = this->derived_cast().out_adjacency(u);
            return adj.find(v) != adj.end();
        }

        auto && neighbours(const node_index_t u)const{
            return this->derived_cast().adjacency(u) | ranges::view::keys;
        }
        auto && out_adjacency(const node_index_t node)const{
            return this->derived_cast().adjacency(node);
        }
    };


    template<class derived> 
    class zugraph_base : public zgraph_base<derived>{

    public:
        using derived_t = derived;
        using graph_traits_t = zgraph_type_traits<derived>;
        using node_index_t = typename graph_traits_t::node_index_t;
        using edge_index_t = typename graph_traits_t::edge_index_t;

        auto && out_adjacency(const node_index_t node)const{
            return this->derived_cast().adjacency(node);
        }
        auto && in_adjaceny(const node_index_t node)const{
            return this->derived_cast().adjacency(node);
        }

    };

    template<class derived> 
    class zdigraph_base : public zgraph_base<derived>{

    public:
      

    };




} // end namespace zgraph


#endif // ZGRAPH_GRAPH_ZGRAPH_BASE_HPP