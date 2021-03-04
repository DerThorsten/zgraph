
#include <iostream>
#include <random>
#include <range/v3/view/transform.hpp>


#include "zgraph/zgraph.hpp"
#include "zgraph/graph/zadjancency_list_digraph.hpp"
#include "zgraph/algorithm/zbfs.hpp"
#include "zgraph/algorithm/zdfs.hpp"
#include "zgraph/algorithm/zmst_kruskal.hpp"

int main(int argc, char *argv[]){

    using graph_t = zgraph::zadjacency_list_digraph<int>;
    using weights_map_t = typename graph_t:: template edge_map<float>;
    graph_t g;

    auto dx = 2;
    auto dy = 2;

    auto node_index = [dx](auto x,auto y){
        return x + y * dx;
    };

    for(auto x=0; x<dx; ++x){
        for(auto y=0; y<dy; ++y){
            const auto nu = node_index(x, y);
            g.add_node(nu);
            if(x+1 < dx){
                g.add_edge(nu, node_index(x+1,y));
                g.add_edge(node_index(x+1,y), nu);
            }
            if(y+1 < dy){
                g.add_edge(nu, node_index(x, y+1));
                g.add_edge(node_index(x, y+1), nu);
            }
        }
    }


    for( auto u : g.nodes()){
        std::cout <<"u "<<u<<"\n";

        for(auto [v, e] : g.adjacency(u)){
            std::cout <<"   v "<<v<<" via e "<< e <<"\n";
        }
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0, 1);

    weights_map_t weights_map(g);
    for( auto edge : g.edges()){
        std::cout <<"the edges "<<edge<<"\n";
        weights_map[edge] = dist(gen);
    }


    

    zgraph::zmst_kruskal<graph_t> mst(g);
    mst.run(weights_map, [](auto && edge, auto && weight, auto && num_clusters){
        std::cout<< " edge "<< edge << " weight "<<weight<<" num_clusters "<<num_clusters<<"\n";
        return true;
    });
    const auto ufd = mst.ufd();

    bfs(g, 0);
    dfs(g, 0);
}