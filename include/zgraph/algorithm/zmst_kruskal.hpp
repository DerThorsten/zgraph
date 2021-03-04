#pragma once
#ifndef ZGRAPH_ALGORITHM_ZMST_KRUSKAL_HPP
#define ZGRAPH_ALGORITHM_ZMST_KRUSKAL_HPP

#include<iostream>

#include <range/v3/view/filter.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/view/transform.hpp>


#include "xtl/xoptional.hpp"

#include "zgraph/graph/zgraph_base.hpp"
#include "zgraph/algorithm/zufd.hpp"



#include <queue>

namespace zgraph 
{


    template<class graph>
    class zmst_kruskal{
    public:
        using graph_t = graph;
        using ufd_t = zufd<graph_t>;




        zmst_kruskal(const zgraph_base<graph> & g)
        :   m_graph(g.derived_cast()),
            m_ufd(m_graph){

        }

        template<class weights_map_t, class callback_t>
        void run(const weights_map_t & weights, callback_t && callback){

            // zip edges and weights together and sort according to weight
            auto vec = ranges::views::zip(m_graph.edges(), weights) | ranges::to_vector;
            ranges::sort(vec, std::less{}, [](auto && ew){
                return std::get<1>(ew);
            });

            // run kruskal
            for(const auto & [edge, weight] :vec){

                auto && [u,v] = m_graph.endpoints(edge);

                // is edge is part of spanning tree?
                if(m_ufd.find(u) != m_ufd.find(v)){

                    if(callback(edge, weight, m_ufd.num_sets())){
                        m_ufd.merge(u, v);
                    } 
                    else{
                        break;
                    }
                }
            }    
        }

        const ufd_t & ufd()const{
            return m_ufd;
        }

    private:
        const graph_t & m_graph;
        ufd_t m_ufd;
    };
    

} // end namespace zgraph


#endif // ZGRAPH_ALGORITHM_ZMST_KRUSKAL_HPP