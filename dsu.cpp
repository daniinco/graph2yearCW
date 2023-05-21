//
// Created by Daniil Klochkov on 30.03.2023.
//
#include "dsu.h"

void make_copies(Graph& big, Graph& small, int second_root_number) {
    auto copies = search_occur(big, small);
    int bad_copies = 0;
    big.copies_and_comp.clear();
    for (auto i: copies) {
        Copy tmp;
        tmp.vertexes = i;
        tmp.dsu = make_dsu(big, i, second_root_number);
        tmp.first_e = {};
        for (int j: big.edges[i[0]]) {
            bool good = true;
            for (int vert: tmp.vertexes) {
                if (vert != i[0] && (big.matrix[j][vert] || j == vert)) {
                    good = false;
                    break;
                }
            }
            if (good) {
                tmp.first_e.push_back(j);
            }
        }
        tmp.second_e = {};
        for (int j: big.edges[i[second_root_number]]) {
            bool good = true;
            for (int vert: tmp.vertexes) {
                if (vert != i[second_root_number] && (big.matrix[j][vert] || j == vert)) {
                    good = false;
                    break;
                }
            }
            if (good) {
                tmp.second_e.push_back(j);
            }
        }
        tmp.avoidable = !is_copy_unavoidable(big, i, second_root_number);
        if (tmp.avoidable) {
            bad_copies++;
        }
        big.copies_and_comp.emplace_back(tmp);
    }
    big.bad_copies = bad_copies;
}

std::vector<int> make_dsu(Graph& graph, std::vector<int>& copy, int second_root_number) {
    std::vector<int> result;
    result.resize(graph.edges.size(), -2);
    for (int i: copy) {
        result[i] = -1;
        for (int j: graph.edges[i]) {
            result[j] = -1;
        }
    }
    for (int i = 0; i < graph.edges.size(); ++i) {
        if (result[i] == -2) {
            std::vector<bool> visited;
            visited.resize(graph.edges.size());
            dfs(graph, visited, copy, i, i, result);
        }
    }
    return result;
}

void dfs(Graph& graph, std::vector<bool>& visited, std::vector<int>& copy, int curr_vertex, int group,
         std::vector<int>& result, int blocked_vertex) {
    visited[curr_vertex] = true;
    result[curr_vertex] = group;
    for (int i: graph.edges[curr_vertex]) {
        if (!visited[i] && result[i] != -1 && i != blocked_vertex) {
            dfs(graph, visited, copy, i, group, result, blocked_vertex);
        }
    }
}

int find(std::vector<int>& dsu, int cur_vert) {
    std::vector<int> upgrade;
    while (cur_vert != -1 && dsu[cur_vert] != cur_vert) {
        upgrade.push_back(cur_vert);
        cur_vert = dsu[cur_vert];
    }
    for (int i: upgrade) {
        dsu[i] = cur_vert;
    }
    return cur_vert;
}
