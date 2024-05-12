#include<iostream>
#include <algorithm>
#include <limits>
#include "graph.hpp"

int mmw(Graph graph) {
    int lb = 0;
    int imax = std::numeric_limits<int>::max();
    while (graph.get_current_length() > 1) {

        int min_neighbours = imax;
        int min_vertex = -1;

        for(unsigned i = 0; i < graph.get_graph_length(); i++) {
            if(graph.count_neighbours(i) == EXCLUDED_VERTEX) continue;
            int sum = graph.count_neighbours(i);
            if (sum < min_neighbours) {
                min_neighbours = sum;
                min_vertex = i;
            }
        }

        int min2_neighbours = imax;
        int min2_vertex = -1;
        for (unsigned i = 0; i < graph.get_graph_length(); i++) {
            if(graph.get_graph()[min_vertex][i] > 0) {
                int sum = graph.count_neighbours(i);
                if (sum < min2_neighbours) {
                    min2_neighbours = sum;
                    min2_vertex = i;
                }
            }
        }


        auto row_col = graph.get_graph()[min2_vertex];
        for(unsigned i = 0; i < row_col.size(); i++) {
            graph.set_value_at(-1, min2_vertex, i);
            graph.set_value_at(-1, i, min2_vertex);
        }
        graph.dec_current_length();

        for (unsigned i = 0; i < graph.get_graph_length(); i++) { 
            if(row_col[i] > 0 && i != min_vertex) {
                graph.add_edge(min_vertex, i);
            }
        }

        

        /*std::cout << "contracted: " << min_vertex << " i " << min2_vertex << std::endl;
        graph.print_graph();*/

        lb = std::max(lb, min_neighbours);
        /*std::cout << "current lb: " << lb << std::endl;*/
    }
    return lb;
}

void bnb_treewidth(Graph &graph, std::vector<int> &perfect_elim_ord, int &ub, int g, int f) {
    if (graph.get_current_length() < 2) {
        ub = std::min(f, ub);
        return;
    }
    for (unsigned i = 0; i < graph.get_graph_length(); i++) {
        if(graph.count_neighbours(i) == EXCLUDED_VERTEX) continue;
        int tmp_old = g;
        g = std::max(g, graph.count_neighbours(i));
        auto row_col = graph.elim_vertex(i);
        //graph.print_graph();
        perfect_elim_ord.push_back(i);
        int h = mmw(graph);
        f = std::max(g, h);
        if (f < ub) {
            bnb_treewidth(graph, perfect_elim_ord, ub, g, f);
        }
        graph.return_edges(row_col, i);
        perfect_elim_ord.pop_back();
        //graph.print_graph();
        g = tmp_old;
    } 
}

int main() {
    /*Graph graph({
        {0, 1, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 1, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 1, 1, 1},
        {0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0}
    });*/

    //wiki
    /*Graph graph({
        {0, 1, 1, 1, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 1, 1, 0},
        {1, 0, 0, 0, 0, 1, 1},
        {0, 0, 1, 0, 0, 1, 0},
        {0, 0, 1, 1, 1, 0, 1},
        {0, 0, 0, 1, 0, 1, 0},
    });*/
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

    /*Graph graph({
        {0, 1},
        {1, 0}
    });*/

    /*Graph graph({
        {0, 1, 0, 1, 0, 0, 0},
        {1, 0, 1, 1, 0, 0, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {1, 1, 1, 0, 0, 1, 1},
        {0, 0, 1, 0, 0, 1, 1},
        {0, 0, 1, 1, 1, 0, 1},
        {0, 0, 0, 1, 1, 1, 0},
    });*/

    int g = 0;

    int h = mmw(graph);
    int f = h;
    int ub = std::numeric_limits<int>::max();
    std::vector<int> perfect_elim_order;
    if (f < ub) {
        bnb_treewidth(graph, perfect_elim_order, ub, g, f);
    }
    std::cout << "Min tree width (BnB): " << ub << std::endl;
    

    //graph.print_graph();

    /*auto vec1 = graph.elim_vertex(4);
    //graph.print_graph();

    auto vec2 = graph.elim_vertex(3);
    //graph.print_graph();

    graph.return_edges(vec2, 3);

    //graph.print_graph();

    graph.return_edges(vec1, 4);

    graph.print_graph();*/



    /*graph.elim_vertex(3);
    graph.print_graph();
    mmw(graph);*/

    return 0;
}