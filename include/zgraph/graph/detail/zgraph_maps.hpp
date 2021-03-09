#pragma once
#include "zgraph/graph/detail/zgraph_items.hpp"
#include "zgraph/detail/zvalue_map.hpp"


#include <range/v3/all.hpp> // get everything

namespace zgraph 
{
    template<class graph,  class item_tag, class associative_container>
    class zassociative_graph_item_map : public detail::zvalue_map<associative_container>{
    public:
        using graph_t = graph;
        using item_tag_t = item_tag;
        using associative_container_t = associative_container;
        using value_t = typename associative_container::mapped_type;

        zassociative_graph_item_map(const graph_t & g, const value_t & value = value_t()){
            ranges::for_each(g. template items<item_tag_t>(), [&](auto && item){
                //this->emplace(item, value); TODO impl me
                this->operator[](item) = value;

            });
        }
    };

}