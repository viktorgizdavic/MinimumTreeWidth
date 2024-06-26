#include "graph_generator.hpp"
#include "brute_force/graph_brute_force.hpp"
#include "optimized/graph.hpp"

#include <chrono>
#include <utility>

#include <iostream>
#include <fstream>
#include <sstream>

#define TEST_SIZE (3)

template<typename Func, typename Arg>
std::pair<typename std::result_of<Func(Arg)>::type, double> executeAndMeasureTime(Func&& func, Arg arg) {
    auto start = std::chrono::steady_clock::now();
    int result = func(arg);
    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    double elapsed_time = elapsed_seconds.count();

    return std::make_pair(result, elapsed_time);
}


int brute_test(const Graph_generator& generatedGraph) {
    Graph_brute graph_brute(generatedGraph.get_matrix());
    long unsigned int min_width = graph_brute.decompose();
    while (std::next_permutation(graph_brute.current_permutation.begin(), graph_brute.current_permutation.end())) {
        graph_brute.reset_graph(generatedGraph.get_matrix());
        long unsigned int current_width = graph_brute.decompose();
        if (current_width < min_width) {
            min_width = current_width;
        }
    }
    return min_width;
}

int optimized_test(const Graph_generator& generatedGraph) {
    Graph graph(generatedGraph.get_matrix());
    int g = 0;
    int h = graph.mmw(graph);
    int f = h;
    int ub = std::numeric_limits<int>::max();
    std::vector<int> perfect_elim_order;
    if (f < ub) {
        graph.bnb_treewidth(graph, perfect_elim_order, ub, g, f);
    }
    return ub;
}

int main() {
    std::ifstream graph_specs("test_graphs.txt");
    if (!graph_specs.is_open()) {
        std::cerr << "Error opening the test_graphs.txt file." << std::endl;
        return 1;
    
    }

    std::ofstream experiment_results("experiment_results.txt");
    if (!experiment_results.is_open()) {
        std::cerr << "Error opening the experiment_results.txt file." << std::endl;
        graph_specs.close();
        return 1;
    }
    experiment_results << "Average values for test size " << TEST_SIZE << std::endl;
    experiment_results << "(n, e): time optimized, time brute, tree width optimized, tree width brute" << std::endl;

    std::string line;
    while (std::getline(graph_specs, line)) {
        std::istringstream iss(line);
        int n, e;
        if (!(iss >> n >> e)) {
            std::cerr << "Error parsing line: " << line << std::endl;
            continue;
        }

        std::cout << "Executing testing for (n, e) = (" << n << ", " << e << ") " << std::flush;

        double optimized_elapsed_time_avg = 0;
        double brute_elapsed_time_avg = 0;
        double optimized_tw_avg = 0;
        double brute_tw_avg = 0;
        for(int i=0; i<TEST_SIZE; i++) {
            Graph_generator generatedGraph{n, e};
            auto optimizedResult = executeAndMeasureTime(optimized_test, generatedGraph);
            optimized_elapsed_time_avg += optimizedResult.second;
            optimized_tw_avg += optimizedResult.first;

            auto bruteResult =  executeAndMeasureTime(brute_test, generatedGraph);
            brute_elapsed_time_avg += bruteResult.second;
            brute_tw_avg += bruteResult.first;
        }
        optimized_elapsed_time_avg /= TEST_SIZE;
        brute_elapsed_time_avg /= TEST_SIZE;
        optimized_tw_avg /= TEST_SIZE;
        brute_tw_avg /= TEST_SIZE;

        experiment_results << "(" << n << ", " << e << "): " 
            << optimized_elapsed_time_avg << ", " << brute_elapsed_time_avg << ", "
            << optimized_tw_avg << ", " << brute_tw_avg << std::endl;

        std::cout << "Done." << std::endl;
    }
    graph_specs.close();
    experiment_results.close();

    return 0;
}