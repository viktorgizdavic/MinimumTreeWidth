#include<iostream>
#include <algorithm>
#include <limits>
#include "graph.hpp"

int main() {
    std::vector<std::vector<int>> test_graph {
        {0, 1, 1, 0, 0, 0, 1, 0},
        {1, 0, 1, 0, 0, 0, 0, 0},
        {1, 1, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 0, 1, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 1, 0},
        {1, 0, 0, 0, 0, 1, 0, 1},
        {0, 0, 0, 0, 0, 0, 1, 0},
    };

    Graph graph(test_graph);
    int g = 0;

    int h = graph.mmw(graph);
    int f = h;
    int ub = std::numeric_limits<int>::max();
    std::vector<int> perfect_elim_order;
    if (f < ub) {
        graph.bnb_treewidth(graph, perfect_elim_order, ub, g, f);
    }
    std::cout << "Min tree width (BnB): " << ub << std::endl;

    return 0;
}