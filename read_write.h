//
// Created by Daniil Klochkov on 11.01.2023.
//
#pragma once

#include "graph_struct.h"

Graph read_graph(std::string file);
void write_graph(Graph& graph, std::string out_file);
void create_antiedges(Graph& graph);
void create_matrix(Graph& graph);
void create_jumps(Graph& graph);
