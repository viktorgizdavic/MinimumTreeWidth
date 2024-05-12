#include<iostream>
#include <algorithm>
#include <limits>
#include "graph_brute_force.hpp"

int main() {
    /*
    junction tree algorithm yt video
    a b c d e f g h 
    0 1 2 3 4 5 6 7
    */
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

    graph.print_initial_graph();

    long unsigned int min_width = graph.decompose();
    while (std::next_permutation(graph.current_permutation.begin(), graph.current_permutation.end())) {
        graph.reset_graph(test_graph);
        long unsigned int current_width = graph.decompose();
        if (current_width < min_width) {
            min_width = current_width;
        }
    }

    std::cout << "Min tree width (brute force): " << min_width << std::endl;

    return 0;
}