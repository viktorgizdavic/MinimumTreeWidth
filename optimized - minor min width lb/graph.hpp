#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <tuple>
#include <limits>

#define EXCLUDED_VERTEX (-1)

class Graph {
public:
    std::vector<std::vector<int>> graph;
    int current_length = 0;

    int mmw(Graph graph) {
        int lb = 0;
        int imax = std::numeric_limits<int>::max();
        while (graph.get_current_length() > 1) {

            int min_neighbours = imax;
            int min_vertex = -1;

            for(int i = 0; i < graph.get_graph_length(); i++) {
                int neighbours_num = graph.count_neighbours(i);
                if(neighbours_num == EXCLUDED_VERTEX) continue;
                if (neighbours_num < min_neighbours) {
                    min_neighbours = neighbours_num;
                    min_vertex = i;
                }
            }

            int min2_neighbours = imax;
            int min2_vertex = -1;
            for (int i = 0; i < graph.get_graph_length(); i++) {
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

            for (int i = 0; i < graph.get_graph_length(); i++) { 
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
        for (int i = 0; i < graph.get_graph_length(); i++) {
            int neighbours_num = graph.count_neighbours(i);
            if(neighbours_num == EXCLUDED_VERTEX) continue;
            int tmp_old = g;
            g = std::max(g, neighbours_num);
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

    Graph(int vertex_num) 
        : current_length(vertex_num) {
        graph.resize(vertex_num, std::vector<int>(vertex_num, 0));
    }

    Graph(std::vector<std::vector<int>> graph) 
        : graph(graph), current_length(graph[0].size()) {
    }
    
    ~Graph() {

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