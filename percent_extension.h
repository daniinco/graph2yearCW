//
// Created by Daniil Klochkov on 31.03.2023.
//
#pragma once

#include "utility"
#include "graph_struct.h"
#include "search_occur.h"
#include "copy_functions.h"
#include "dsu.h"
#include "unordered_set"

std::pair<int, int> delta_by_edge(Graph& big, Graph& small, int vertex1, int vertex2, int second_root_number);
