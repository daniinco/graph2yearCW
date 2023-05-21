#pragma once

#include "graph_struct.h"

void classic_extension(Graph& graph, Graph& small, int second_root_number, int max_vert, int numerator=1, int denominator=1);
//second_root_number номер второго корня в small

void classic_extension_of_copy(Graph& graph, Graph& small, std::vector<int>& copy, int second_root, int type);
// second root номер второго корня в списке вершин копии
//type = 0 делаем висячие вершины из обоих корней, 1 - из первого, 2 - из второго