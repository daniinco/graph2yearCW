//
// Created by Daniil Klochkov on 15.01.2023.
//
#include "search_occur.h"


void occur_starting_there(Graph& big, Graph& small, int& timer, int previous_timer, int starting_index_in_small, int current_vertex_in_big, std::vector<int>& available_vertexes, std::vector<std::vector<int>>& result, std::vector<int>& current_graph) {
    ++timer;
    int starting_timer = timer;
    current_graph.push_back(current_vertex_in_big);
    if (starting_index_in_small == small.edges.size() - 1) {
        result.push_back(current_graph);
        current_graph.pop_back();
        return;
    }
    for (int i: big.antiedges[current_vertex_in_big]) {
        if (available_vertexes[i] >= previous_timer) {
            available_vertexes[i] = starting_timer;
        }
    }
    if (starting_index_in_small == 0 || (small.jumps[starting_index_in_small] == -1 && small.jumps[starting_index_in_small - 1] == -1)) {
        for (int i: big.edges[current_vertex_in_big]) {
            if ((starting_index_in_small == 0 || i != current_graph[starting_index_in_small - 1]) && available_vertexes[i] >= previous_timer) {
                occur_starting_there(big, small, timer, starting_timer, starting_index_in_small + 1, i, available_vertexes, result, current_graph);
            }
        }
    } else if (small.jumps[starting_index_in_small] == -1) {
        for (int i: big.edges[current_vertex_in_big]) {
            if (i != current_graph[small.jumps[starting_index_in_small - 1]] && available_vertexes[i] >= previous_timer) {
                occur_starting_there(big, small, timer, starting_timer, starting_index_in_small + 1, i, available_vertexes, result, current_graph);
            }
        }
    } else {
        for (int i: big.edges[current_graph[small.jumps[starting_index_in_small]]]) {
            if (std::find(current_graph.begin(), current_graph.end(), i) == current_graph.end()) {
                big.flag = false;
                for (int j = 0; j < current_graph.size(); ++j) {
                    if (j != small.jumps[starting_index_in_small] && big.matrix[current_graph[j]][i]) {
                        big.flag = true;
                        break;
                    }
                }
                if (!big.flag) {
                    occur_starting_there(big, small, timer, starting_timer, starting_index_in_small + 1, i,
                                         available_vertexes, result, current_graph);
                }
            }
        }
    }
    current_graph.pop_back();
}

std::vector<std::vector<int>> search_occur(Graph& big, Graph& small) {
    std::vector<std::vector<int>> result;
    int timer = 0;
    std::vector<int> available_vertexes;
    std::vector<int> current_graph;
    available_vertexes.resize(big.edges.size());
    for (int i = 0; i < big.edges.size(); ++i) {
        occur_starting_there(big, small, timer, 0, 0, i, available_vertexes, result, current_graph);
    }
    return result;
}

std::vector<std::vector<int>> occur_with_vertex(Graph& big, Graph& small, int vertex) {
    std::vector<std::vector<int>> result;
    for (int i = 0; i < small.edges.size(); ++i) {
        std::vector<int> copy;
        copy.resize(small.edges.size(), -1);
        copy[i] = vertex;
        std::deque<std::pair<int, int>> queue;
        for (auto j: small.edges[i]) {
            queue.emplace_back(i, j);
        }
        occur_with_vertex_rec(big, small, queue, copy, result);
    }
    return result;
}

void occur_with_vertex_rec(Graph& big, Graph& small, std::deque<std::pair<int, int>>& queue, std::vector<int>& copy, std::vector<std::vector<int>>& result) {
    if (queue.empty()) {
        result.push_back(copy);
        return;
    }
    std::pair<int, int> added = queue.back();
    queue.pop_back();
    for (int i: big.edges[copy[added.first]]) {
        bool ok = true;
        for (int j: copy) {
            if (j == i || (j != -1 && j != copy[added.first] && big.matrix[i][j])) {
                ok = false;
                break;
            }
        }
        if (ok) {
            copy[added.second] = i;
            for (auto j: small.edges[added.second]) {
                if (j != added.first) {
                    queue.emplace_back(added.second, j);
                }
            }
            occur_with_vertex_rec(big, small, queue, copy, result);
            for (int _ = 0; _ < small.edges[added.second].size() - 1; ++_) {
                queue.pop_back();
            }
            copy[added.second] = -1;
        }
    }
    queue.push_back(added);
}

std::vector<std::vector<int>> occur_with_edge(Graph& big, Graph& small, int vertex1, int vertex2) {
    std::vector<std::vector<int>> result;
    for (int i = 0; i < small.edges.size(); ++i) {
        std::vector<int> copy;
        copy.resize(small.edges.size(), -1);
        copy[i] = vertex1;
        std::deque<std::pair<int, int>> queue;
        for (auto j: small.edges[i]) {
            copy[j] = vertex2;
            for (int k: small.edges[i]) {
                if (k != j) {
                    queue.emplace_back(i, k);
                }
            }
            for (int k: small.edges[j]) {
                if (k != i) {
                    queue.emplace_back(j, k);
                }
            }
            occur_with_vertex_rec(big, small, queue, copy, result);
            queue.clear();
        }
    }
    return result;
}