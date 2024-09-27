#include "graph_generator.hpp"
#include "brute_force/graph_brute_force.hpp"
#include "optimized/graph.hpp"

#include <chrono>
#include <utility>

#include <iostream>
#include <fstream>
#include <sstream>

#define TEST_SIZE (3)

#define EXCLUDE_BRUTE_FORCE (0)

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

int min_width_test(const Graph_generator& generatedGraph) {
    Graph graph(generatedGraph.get_matrix());
    int g = 0;
    int ub = std::numeric_limits<int>::max();
    int h = graph.mw(graph);
    int f = h;
    std::vector<int> perfect_elim_order;
    if (f < ub) {
        graph.bnb_treewidth(MW_HEURISTIC, graph, perfect_elim_order, ub, g, f);
    }
    return ub;
}

int minor_min_width_test(const Graph_generator& generatedGraph) {
    Graph graph(generatedGraph.get_matrix());
    int g = 0;
    int h = graph.mmw(graph);
    int f = h;
    int ub = std::numeric_limits<int>::max();
    std::vector<int> perfect_elim_order;
    if (f < ub) {
        graph.bnb_treewidth(MMW_HEURISTIC, graph, perfect_elim_order, ub, g, f);
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
    experiment_results << "(n, e): brute force, mw, mmw, tw brute, tw mw, tw mmw" << std::endl;

    std::string line;
    while (std::getline(graph_specs, line)) {
        std::istringstream iss(line);
        int n, e;
        if (!(iss >> n >> e)) {
            std::cerr << "Error parsing line: " << line << std::endl;
            continue;
        }

        std::cout << "Executing testing for (n, e) = (" << n << ", " << e << ") " << std::flush;

        double brute_elapsed_time_avg = 0;
        double mmw_elapsed_time_avg = 0;
        double mw_elapsed_time_avg = 0;
        
        double brute_tw_avg = 0;
        double mmw_tw_avg = 0;
        double mw_tw_avg = 0;

        int test_size = TEST_SIZE;
        if(n >= 20) {
            test_size = 1;
        }
        for(int i=0; i<test_size; i++) {
            Graph_generator generatedGraph{n, e};
            auto mmwResult = executeAndMeasureTime(minor_min_width_test, generatedGraph);
            mmw_elapsed_time_avg += mmwResult.second;
            mmw_tw_avg += mmwResult.first;
           
            if(n <= 20) {
                auto mwResult = executeAndMeasureTime(min_width_test, generatedGraph);
                mw_elapsed_time_avg += mwResult.second;
                mw_tw_avg += mwResult.first;
            }

            if(!EXCLUDE_BRUTE_FORCE && n < 10) {
                auto bruteResult =  executeAndMeasureTime(brute_test, generatedGraph);
                brute_elapsed_time_avg += bruteResult.second;
                brute_tw_avg += bruteResult.first;
            }
        }
        mmw_elapsed_time_avg /= TEST_SIZE;
        mmw_tw_avg /= TEST_SIZE;

        if(n <= 20) {
            mw_elapsed_time_avg /= TEST_SIZE;
            mw_tw_avg /= TEST_SIZE;
        }

        if(!EXCLUDE_BRUTE_FORCE && n < 10) {
            brute_elapsed_time_avg /= TEST_SIZE;
            brute_tw_avg /= TEST_SIZE;
        }

        experiment_results << "(" << n << ", " << e << "): " 
            << brute_elapsed_time_avg << ", " << mw_elapsed_time_avg << ", " << mmw_elapsed_time_avg << ", "
            << brute_tw_avg << ", " << mw_tw_avg << ", " << mmw_tw_avg << std::endl;

        std::cout << "Done." << std::endl;
    }
    graph_specs.close();
    experiment_results.close();

    return 0;
}