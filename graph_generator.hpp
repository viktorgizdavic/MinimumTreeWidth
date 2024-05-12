#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <set>

class Graph_generator {
private:
    int num_vertex;
    int num_edges;
    std::vector<std::vector<int>> matrix;    

    void generate_marix() {
        std::srand(std::time(nullptr));
        std::set<int> connectable_vertices;
        
        matrix.push_back(std::vector<int>(1,0));
        connectable_vertices.insert(0);
        for (int i = 1; i < num_vertex; i++) {
            connectable_vertices.insert(i);
            matrix.push_back(std::vector<int>(i + 1, 0));
            int rand_value = std::rand() % i;
            matrix[i][rand_value]++;
            for (int j = 0; j < i; j++) {
                matrix[j].push_back(0);
            }
            matrix[rand_value][i]++;
        }
        std::set<std::pair<int,int>> set;
        for (int i = 0; i < num_vertex; i++) {
            for (int j = 0; j < num_vertex; j++) {
                if (matrix[i][j] != 1 && i != j && i > j) {
                    set.insert({i, j});
                }
            }
        }

        int remaining_edges = num_edges - num_vertex + 1;
        for (int i = 0; i < remaining_edges; i++) {
            auto it = set.begin();
            std::advance(it, std::rand() % set.size());
            std::pair<int, int> ij = *it;
            int n1 = ij.first;
            int n2 = ij.second;
            matrix[n1][n2]++;
            matrix[n2][n1]++;
            set.erase(ij);
        }
    }

public:
    Graph_generator(int n, int e) {
        num_vertex = n;
        num_edges = e;
        generate_marix();
    }
    
    ~Graph_generator() {

    }

    void print_matrix() {
        for (int i = 0; i < num_vertex; i++) {
            for (int j = 0; j < num_vertex; j++) {
                std::cout << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
        
    }
    
    std::vector<std::vector<int>> get_matrix() const {
        return matrix;
    }
};