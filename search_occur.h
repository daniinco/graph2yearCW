//
// Created by Daniil Klochkov on 15.01.2023.
//
#pragma once

#include <vector>
#include "graph_struct.h"
#include "deque"


std::vector<std::vector<int>> search_occur(Graph& big, Graph& small);
void occur_starting_there(Graph& big, Graph& small, int& timer, int previous_timer, int starting_index_in_small, int current_vertex_in_big, std::vector<int>& available_vertexes, std::vector<std::vector<int>>& result, std::vector<int>& current_graph);
std::vector<std::vector<int>> occur_with_vertex(Graph& big, Graph& small, int vertex);
void occur_with_vertex_rec(Graph& big, Graph& small, std::deque<std::pair<int, int>>& queue, std::vector<int>& copy, std::vector<std::vector<int>>& result);
