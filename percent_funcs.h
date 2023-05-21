#pragma once

#include "utility"
#include "graph_struct.h"

std::pair<int, int> calc_copies_counts(Graph& graph, Graph& small, int second_root_number);
// second_root_number это номер вершины в копии. Параметр задаётся только если корни не совпадают
// Считается что нулевая вершина копии всегда корень