#include<iostream>
#include <algorithm>
#include <limits>
#include "graph.hpp"
#include "../graph_generator.hpp"

int main() {
    Graph_generator generatedGraph{15, 20};
    Graph_semi graph(generatedGraph.get_matrix());
    graph.print_graph();

    int g = 0;
    int h = graph.mw(graph);
    int f = h;
    int ub = std::numeric_limits<int>::max();
    std::vector<int> perfect_elim_order;
    if (f < ub) {
        graph.bnb_treewidth(graph, perfect_elim_order, ub, g, f);
    }
    
    std::cout << "Min tree width (BnB): " << ub << std::endl;

    return 0;
}