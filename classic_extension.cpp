#include "classic_extension.h"
#include "search_occur.h"
#include "copy_functions.h"
#include "iostream"

void classic_extension(Graph& graph, Graph& small, int second_root_number, int max_vert, int numerator, int denominator) {
    size_t copy_count = graph.copies_and_comp.size();
    auto iter = graph.copies_and_comp.begin();
    while (copy_count != 0) {
        if (iter->avoidable) {
            if (rand() % denominator <= numerator - 1) {
                auto root_expandable = is_roots_expandable(graph, iter->vertexes, second_root_number);
                if (!root_expandable.first && !root_expandable.second) {
                    classic_extension_of_copy(graph, small, iter->vertexes, second_root_number, 0);
                } else if (!root_expandable.first && root_expandable.second) {
                    classic_extension_of_copy(graph, small, iter->vertexes, second_root_number, 1);
                } else if (root_expandable.first && !root_expandable.second) {
                    classic_extension_of_copy(graph, small, iter->vertexes, second_root_number, 2);
                } else {
                    classic_extension_of_copy(graph, small, iter->vertexes, second_root_number, 1);
                }
            }
        }
        ++iter;
        --copy_count;
        if (graph.edges.size() > max_vert) {
            break;
        }
    }
}

void classic_extension_of_copy(Graph& graph, Graph& small, std::vector<int>& copy, int second_root, int type) {
    if (type == 0) {
        graph.add_vertex();
        graph.add_vertex();
        graph.add_edge(small, copy[0], graph.edges.size() - 2, second_root);
        graph.add_edge(small, copy[second_root], graph.edges.size() - 1, second_root);
    } else if (type == 1) {
        graph.add_vertex();
        graph.add_edge(small, copy[0], graph.edges.size() - 1, second_root);
    } else {
        graph.add_vertex();
        graph.add_edge(small, copy[second_root], graph.edges.size() - 1, second_root);
    }
}
