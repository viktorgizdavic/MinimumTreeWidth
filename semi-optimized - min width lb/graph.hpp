#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <tuple>
#include <limits>

#define EXCLUDED_VERTEX (-1)

class Graph_semi {
public:
    std::vector<std::vector<int>> graph;
    int current_length = 0;

    int mw(Graph_semi graph) {
        int imax = std::numeric_limits<int>::max();
        Graph_semi initial_graph = Graph_semi(graph.graph);
        std::vector<int> min_width_ordering;

        while (graph.get_current_length() >= 1) {

            int min_neighbours = imax;
            int min_vertex = -1;

            for(int i = 0; i < graph.get_graph_length(); i++) {
                int sum = graph.count_neighbours(i);
                if(sum == EXCLUDED_VERTEX) continue;
                if (sum < min_neighbours) {
                    min_neighbours = sum;
                    min_vertex = i;
                }
            }

            min_width_ordering.push_back(min_vertex);
            graph.elim_vertex_without_simplicial(min_vertex);
        }

        int lb = 0;
        for(unsigned current_vertex = 0; current_vertex < min_width_ordering.size(); current_vertex++) {
            int neighbours_below = 0;
            for(unsigned j = 0; j < current_vertex; j++) {
                if(initial_graph.graph[current_vertex][j] > 0) {
                    neighbours_below++; 
                }
            }
            lb = std::max(lb, neighbours_below);
        }

        return lb;
    }

    void bnb_treewidth(Graph_semi &graph, std::vector<int> &perfect_elim_ord, int &ub, int g, int f) {
        if (graph.get_current_length() < 2) {
            ub = std::min(f, ub);
            return;
        }
        for (int i = 0; i < graph.get_graph_length(); i++) {
            if(graph.count_neighbours(i) == EXCLUDED_VERTEX) continue;
            int tmp_old = g;
            g = std::max(g, graph.count_neighbours(i));
            auto row_col = graph.elim_vertex(i);
            //graph.print_graph();
            perfect_elim_ord.push_back(i);
            int h = mw(graph);
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

    Graph_semi(int vertex_num) 
        : current_length(vertex_num) {
        graph.resize(vertex_num, std::vector<int>(vertex_num, 0));
    }

    Graph_semi(std::vector<std::vector<int>> graph) 
        : graph(graph), current_length(graph[0].size()) {
    }
    
    ~Graph_semi() {

    }
    
    void add_edge(int neighbour1, int neighbour2) {
        graph[neighbour1][neighbour2] = 1;
        graph[neighbour2][neighbour1] = 1;
    }

    void return_edges(std::vector<int> row_col, int vertex) {
        for(unsigned i = 0; i < row_col.size() - 1; i++) {
            if(row_col[i] > 0) {
                for(unsigned j = i + 1; j < row_col.size(); j++) {
                    if(row_col[j] > 0) {
                        graph[i][j]--;
                        graph[j][i]--;
                    }
                }
            }
        }

        graph[vertex] = row_col;
        for (unsigned i = 0; i < graph.size(); i++) {
            graph[i][vertex] = row_col[i];
        }
        current_length++;
    }

    void elim_vertex_without_simplicial(int vertex) {
        for(unsigned i = 0; i < graph.size(); i++) {
            graph[vertex][i] = graph[i][vertex] = -1;
        }

        current_length--;
    }
    
    std::vector<int> elim_vertex(int vertex) {
        std::vector<int> row_col = graph[vertex];

        for(unsigned i = 0; i < row_col.size() - 1; i++) {
            if(row_col[i] > 0) {
                for(unsigned j = i + 1; j < row_col.size(); j++) {
                    if(row_col[j] > 0) {
                        graph[i][j]++;
                        graph[j][i]++;
                    }
                }
            }
        }

        for(unsigned i = 0; i < graph.size(); i++) {
            graph[vertex][i] = graph[i][vertex] = -1;
        }

        current_length--;
        return row_col;
    }

    int count_neighbours(int vertex) {
        if(graph[vertex][vertex] == -1) {
            return EXCLUDED_VERTEX;
        }
        return std::accumulate(graph[vertex].cbegin(), graph[vertex].cend(), 0,
            [](int acc, int b) {return b <= 0 ? acc : acc + 1;});
    }

    std::vector<std::vector<int>> get_graph() {
        return graph;
    }

    void set_value_at(int x, int i, int j) {
        graph[i][j] = x;
    }



    int get_graph_length() {
        return graph.size();
    }
    int get_current_length() {
        return current_length;
    }
    void inc_current_length() {
        current_length++;
    }
    void dec_current_length() {
        current_length--;
    }


    void print_graph() {
        for(auto row: graph) {
            for(int a : row) {
                std::cout << a <<" ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};