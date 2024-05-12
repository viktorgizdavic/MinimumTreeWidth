#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <set>
#include <tuple>

#define EXCLUDED_VERTEX (-1)

class Graph_brute {
private:
    std::vector<std::vector<int>> graph;

    std::vector<std::vector<int>> conjunction_graph_edges;
    std::vector<std::set<int>> conjunction_graph_nodes;

    int current_length = 0;
public:
    Graph_brute(std::vector<std::vector<int>> graph) 
        : graph(graph), current_length(graph[0].size()) {
        current_permutation.resize(current_length);
        for (int i = 0; i < current_length; i++) {
            current_permutation[i] = i;
        }
    }
    ~Graph_brute() {

    }

    std::vector<int> current_permutation;
    long unsigned int decompose() {
        // prvi deo, dobija se graph a ne stablo
        for(int i = current_permutation.size() - 1; i >= 0; i--) {
            int current_vertex = current_permutation[i];
            add_to_conjunction_graph(current_vertex);
            elim_vertex(current_vertex);
        }
        long unsigned int max = 0;
        for(auto s: conjunction_graph_nodes) {
            if(max < s.size()) {
                max = s.size();
            }
        }

        //vracanje width-a jedne permutacije
        return max - 1;
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

    void reset_graph(std::vector<std::vector<int>> g) {
        graph = g;
        current_length = graph[0].size();
        conjunction_graph_nodes.resize(0);
        conjunction_graph_edges.resize(0);
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

    void print_conjuction_edges() {
        print_graph_helper(conjunction_graph_edges);
    }
    void print_initial_graph() {
        print_graph_helper(graph);
    }

    void print_conjuction_nodes() {
        for(auto s: conjunction_graph_nodes) {
            for(int a : s) {
                std::cout << a;
            }
            std::cout << "  ";
        }
        std::cout << std::endl;
    }

private:
    void add_to_conjunction_graph(int vertex) {
        std::vector<int> row_col = graph[vertex];
        std::set<int> new_node;
        new_node.insert(vertex);
        for(unsigned i = 0; i < row_col.size(); i++) {
            if(row_col[i] > 0) {
                new_node.insert(i);
            }
        }
        // proveri da li je new_node pravi podskup postojeceg
        for(unsigned i = 0; i < conjunction_graph_nodes.size(); i++) {
            
            if(intersection_size(new_node, conjunction_graph_nodes[i]) == new_node.size()) {
                return;
            }
        }

        // znamo da moramo da ga dodamo, i jos moramo da dodamo granu sa weight-om od velicine preseka
        conjunction_graph_nodes.push_back(new_node);
        conjunction_graph_edges.push_back(std::vector<int>(conjunction_graph_nodes.size(), 0));
        for(unsigned i = 0; i < conjunction_graph_nodes.size() - 1; i++) {
            int weight = intersection_size(new_node, conjunction_graph_nodes[i]);
            conjunction_graph_edges[i].push_back(weight);
            conjunction_graph_edges.back()[i] = weight;
        }
        
    }

    void elim_vertex(int vertex) {
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
    }

    long unsigned int intersection_size(const std::set<int>& first_set, const std::set<int>& second_set) {
        std::vector<int> v_intersection;
        std::set_intersection(first_set.begin(), first_set.end(), second_set.begin(), second_set.end(),
               std::back_inserter(v_intersection));
        return v_intersection.size();
    }

    void print_graph_helper(const std::vector<std::vector<int>>& g) {
        for(auto row: g) {
            for(int a : row) {
                std::cout << a <<" ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }    
};