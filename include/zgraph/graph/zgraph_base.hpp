#pragma once
#ifndef ZGRAPH_GRAPH_ZGRAPH_BASE_HPP
#define ZGRAPH_GRAPH_ZGRAPH_BASE_HPP



#include <range/v3/all.hpp>

#include "zgraph/graph/detail/zgraph_items.hpp"

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
    class zgraph_base;



    


    template<class derived, bool is_directed, bool is_mutligraph, bool allow_self_loops>
    class zgraph_hyper_base;


    template<class derived, bool is_mutligraph, bool allow_self_loops>
    class zgraph_hyper_base<derived, true,  is_mutligraph, allow_self_loops> : public zcrtp_base<derived>{
    public:
        using derived_t = derived;
        using graph_traits_t = zgraph_type_traits<derived_t>;
        using node_index_t = typename graph_traits_t::node_index_t;
        using edge_index_t = typename graph_traits_t::edge_index_t;

    };

    template<class derived, bool is_mutligraph, bool allow_self_loops>
    class zgraph_hyper_base<derived, false,  is_mutligraph, allow_self_loops> : public zcrtp_base<derived>{
    public:
        using derived_t = derived;
        using graph_traits_t = zgraph_type_traits<derived_t>;
        using node_index_t = typename graph_traits_t::node_index_t;
        using edge_index_t = typename graph_traits_t::edge_index_t;

        decltype(auto) in_adjacency(const node_index_t node)const{
            return this->derived_cast().adjacency(node);
        }
    };


    template<class derived>
    class zgraph_base : public zgraph_hyper_base<derived,
        zgraph_type_traits<derived>::is_directed_t::value,
        zgraph_type_traits<derived>::is_mutligraph_t::value,
        zgraph_type_traits<derived>::allow_self_loops_t::value
    >{

    public:
        using derived_t = derived;
        using graph_traits_t = zgraph_type_traits<derived>;
        using node_index_t = typename graph_traits_t::node_index_t;
        using edge_index_t = typename graph_traits_t::edge_index_t;

        using is_directed_t = typename graph_traits_t::is_directed_t;
        using is_mutligraph_t = typename graph_traits_t::is_mutligraph_t;
        using allow_self_loops_t = typename graph_traits_t::allow_self_loops_t;

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

        // edges
        template<class item_tag, std::enable_if_t<std::is_same<item_tag, zedge_tag>::value, bool > = true  >
        decltype(auto) items() const{
            return this->derived_cast().edges();
        }

        // nodes
        template<class item_tag, std::enable_if_t<std::is_same<item_tag, znode_tag>::value, bool > = true  >
        decltype(auto)  items() const{
            return this->derived_cast().nodes();
        }


        decltype(auto) out_adjacency(const node_index_t node)const{
            return this->derived_cast().adjacency(node);
        }

        decltype(auto) endpoints()const{
            return this->derived_cast().edges() | ranges::views::transform([&](auto&& edge){
                return  this->derived_cast().endpoints(edge);
            });
        }

    };





} // end namespace zgraph


#endif // ZGRAPH_GRAPH_ZGRAPH_BASE_HPP