//
// Created by Daniil Klochkov on 11.01.2023.
//
#pragma once

#include "vector"
#include "list"

struct Copy {
    std::vector<int> vertexes;
    std::vector<int> dsu;
    std::vector<int> first_e;
    std::vector<int> second_e;
    bool avoidable;
};

struct Graph{
    std::vector<std::vector<int>> edges; //[[1], [0, 2], [1, 3, 5], [2, 4], [3], [2, 6], [5]]
    std::vector<std::vector<int>> antiedges; // анти-ребра
    std::vector<int> jumps; // [-1, -1, -1, -1, 2, -1, -1] нужно задавать для субкубического дерева при его поиске
    std::vector<std::vector<bool>> matrix;
    bool flag; // кажется нужен для какого то костыля. Не трогать если не уверен. Используется где то как глобальная переменная
    std::list<Copy> copies_and_comp;
    int bad_copies;
    int add_vertex();
    void add_edge(Graph& small, int vertex1, int vertex2, int second_root_number);
};

int max_edge(const Graph& graph);
bool is_copy_unavoidable(Graph& graph, std::vector<int>& copy, int second_root_number);
// second_root_number это номер вершины в копии. Параметр задаётся только если корни не совпадают
// Считается что нулевая вершина копии всегда корень
std::vector<int> make_dsu(Graph& graph, std::vector<int>& copy, int second_root_number);
void dfs(Graph& graph, std::vector<bool>& visited, std::vector<int>& copy, int curr_vertex, int group, std::vector<int>& result, int blocked_vertex=-1);
int find(std::vector<int>& dsu, int cur_vert);
std::vector<std::vector<int>> occur_with_edge(Graph& big, Graph& small, int vertex1, int vertex2);
