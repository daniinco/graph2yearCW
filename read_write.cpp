//
// Created by Daniil Klochkov on 11.01.2023.
//
#include "string"
#include "read_write.h"

#include "iostream"
#include "fstream"
#include "sstream"

Graph read_graph(std::string in_file) {
    Graph result = Graph();
    result.edges.clear();
    result.antiedges.clear();
    result.jumps.clear();
    result.copies_and_comp.clear();
    result.bad_copies = 0;
    result.matrix.clear();
    result.flag = false;
    std::ifstream file(in_file);
    std::string line;
    int num;
    int index = 0;
    while (getline(file, line)) {
        std::istringstream data(line);
        result.edges.emplace_back();
        bool flag = true;
        while (flag) {
            data >> num;
            if (num != -1) {
                result.edges[index].push_back(num);
            } else {
                index++;
                flag = false;
            }
        }
    }
    create_antiedges(result);
    create_matrix(result);
    create_jumps(result);
    return result;
}

void write_graph(Graph& graph, std::string out_file) {
    std::ofstream out;
    out.open(out_file);
    if (out.is_open()) {
        for (int i = 0; i < graph.edges.size(); ++i) {
            for (int j = 0; j < graph.edges[i].size(); ++j) {
                out << graph.edges[i][j] << ' ';
            }
            out << "-1" << std::endl;
        }
    }
}

void create_antiedges(Graph& graph) {
    graph.antiedges.resize(graph.edges.size());
    for (int i = 0; i < graph.edges.size(); ++i) {
        for (int j = 0; j < graph.edges.size(); ++j) {
            if (i != j && std::find(graph.edges[i].begin(), graph.edges[i].end(), j) == graph.edges[i].end()) {
                graph.antiedges[i].push_back(j);
            }
        }
    }
}

void create_matrix(Graph& graph) {
    std::vector<std::vector<bool>> matrix;
    std::vector<bool> raw;
    raw.resize(graph.edges.size());
    matrix.resize(graph.edges.size(), raw);
    for (int i = 0; i < graph.edges.size(); ++i) {
        for (int j = 0; j < graph.edges[i].size(); ++j) {
            matrix[i][graph.edges[i][j]] = true;
        }
    }
    graph.matrix = matrix;
}

void dfs_for_jumps(Graph& graph, std::vector<int>& res, std::vector<bool>& visited, int cur_vertex, int jump) {
    visited[cur_vertex] = true;
    jump = -1;
    for (int i: graph.edges[cur_vertex]) {
        if (!visited[i]) {
            res.push_back(jump);
            dfs_for_jumps(graph, res, visited, i, jump);
            jump = cur_vertex;
        }
    }
    graph.jumps.push_back(-1);
    graph.jumps = res;
}

void create_jumps(Graph& graph) {
    std::vector<int> result;
    std::vector<bool> visited;
    visited.resize(graph.edges.size());
    dfs_for_jumps(graph, result, visited, 0, -1);
}