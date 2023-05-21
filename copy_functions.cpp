//
// Created by Daniil Klochkov on 07.03.2023.
//
#include "copy_functions.h"
#include "queue"

bool is_copy_unavoidable(Graph& graph, std::vector<int>& copy, int second_root_number=0) {
    bool found_unclosable = false;
    std::pair<std::set<int>, std::set<int>> neighbours = fing_all_neighbours(graph, copy, copy[0],
                                                                             copy[second_root_number]);
    for (int i: graph.edges[copy[0]]) {
        if (neighbours.second.find(i) == neighbours.second.end() && (!graph.matrix[i][copy[second_root_number]]
        || second_root_number == 0)) {
            for (int j: graph.edges[copy[second_root_number]]) {
                if (neighbours.second.find(j) == neighbours.second.end() && (!graph.matrix[j][copy[0]]
                || 0 == second_root_number) && !graph.matrix[j][i] && j != i) {
                    found_unclosable = true;
                    //Поиск в ширину между i и j на графе без копии и соседей
                    std::queue<int> que;
                    std::set<int> processed;
                    for (int k: graph.edges[i]) {
                        if (neighbours.first.find(k) == neighbours.first.end()) {
                            que.push(k);
                            processed.insert(k);
                        }
                    }
                    int in_process;
                    while (!que.empty()) {
                        in_process = que.front();
                        if (in_process == j) {
                            found_unclosable = false;
                            break;
                        }
                        for (int k: graph.edges[in_process]) {
                            if (k == j) {
                                std::queue<int> empty;
                                std::swap(empty, que);
                                que.push(0);
                                found_unclosable = false;
                                break;
                            }
                            if (neighbours.first.find(k) == neighbours.first.end() && processed.find(k)
                            == processed.end()) {
                                que.push(k);
                                processed.insert(k);
                            }
                        }
                        que.pop();
                    }
                    if (found_unclosable) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

std::pair<std::set<int>, std::set<int>> fing_all_neighbours(Graph& graph, std::vector<int>& copy, int f_root,
                                                            int s_root) {
    std::set<int> result_1;
    std::set<int> result_2;
    for (int i: copy) {
        result_1.insert(i);
        result_2.insert(i);
        if (i == f_root || i == s_root) {
            for (int j: graph.edges[i]) {
                result_1.insert(j);
                if (graph.matrix[j][s_root] && graph.matrix[j][f_root] && f_root != s_root) {
                    result_2.insert(j);
                }
            }
        } else {
            for (int j: graph.edges[i]) {
                result_1.insert(j);
                result_2.insert(j);
            }
        }
    }
    return {result_1, result_2};
}

std::pair<bool, bool> is_roots_expandable(Graph& graph, std::vector<int>& copy, int second_root_number) {
    bool first = false;
    bool second = false;
    for (int i: graph.edges[copy[0]]) {
        if (std::find(copy.begin(), copy.end(), i) == copy.end()) {
            first = true;
            for (int j: copy) {
                if (j != copy[0] && graph.matrix[j][i]) {
                    first = false;
                    break;
                }
            }
            if (first) {
                break;
            }
        }
    }
    for (int i: graph.edges[copy[second_root_number]]) {
        if (std::find(copy.begin(), copy.end(), i) == copy.end()) {
            second = true;
            for (int j: copy) {
                if (j != copy[second_root_number] && graph.matrix[j][i]) {
                    second = false;
                    break;
                }
            }
            if (second) {
                break;
            }
        }
    }
    return {first, second};
}