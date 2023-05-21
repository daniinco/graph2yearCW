#pragma once

#include "graph_struct.h"
#include <set>


std::pair<std::set<int>, std::set<int>> fing_all_neighbours(Graph& graph, std::vector<int>& copy, int f_root, int s_root);
// Первый сет - все соседи копии и копия, второй - то же самое для копии без корней.

std::pair<bool, bool> is_roots_expandable(Graph& graph, std::vector<int>& copy, int second_root_number);
// second_root_number это номер вершины в копии. Параметр задаётся только если корни не совпадают
// Считается что нулевая вершина копии всегда корень
