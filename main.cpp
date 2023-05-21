#pragma GCC optimize("o3")

#include <iostream>

#include "graph_struct.h"
#include "read_write.h"
#include "search_occur.h"
#include "copy_functions.h"
#include "vector"
#include "classic_extension.h"
#include "percent_funcs.h"
#include "list"
#include "dsu.h"
#include "percent_extension.h"
#include "compile_extensions.h"


int main() {
    std::vector<std::pair<std::string, int>> limited = {{"../graphs/T_3_3_3.txt", 9}, {"../graphs/T_1_1_1.txt", 3},
                                                        {"../graphs/T_2_2_2.txt", 6}, {"../graphs/ExC.txt", 5},
                                                        {"../graphs/Hedgehog_4.txt", 1}, {"../graphs/Hedgehog_4.txt", 2},
                                                        {"../graphs/Tr_2_2_2.txt", 4}, {"../graphs/Leaf_extended_tree.txt", 3},
                                                        {"../graphs/Leaf_extended_tree.txt", 7}, {"../graphs/P_7.txt", 5},
                                                        {"../graphs/P_6.txt", 4}, {"../graphs/P_4.txt", 2},
                                                        {"../graphs/P_3.txt", 0}, {"../graphs/P_4.txt", 0},
                                                        {"../graphs/T_1_1_1.txt", 0}, {"../graphs/T_1_1_2.txt", 0},
                                                        {"../graphs/T_1_2_1.txt", 0}, {"../graphs/T_1_4_1.txt", 0},
                                                        {"../graphs/P_4.txt", 1}, {"../graphs/T_2_1_1.txt", 1},
                                                        {"../graphs/T_2_4_1.txt", 1}, {"../graphs/P_6.txt", 2},
                                                        {"../graphs/T_3_1_1.txt", 2}, {"../graphs/T_2_1_2.txt", 4}};
    std::vector<std::pair<std::string, int>> unlimited = {{"../graphs/P_3.txt", 1}, {"../graphs/P_6.txt", 1},
                                                          {"../graphs/P_6.txt", 3}, {"../graphs/T_1_1_3.txt", 0},
                                                          {"../graphs/P_7.txt", 4}, {"../graphs/P_7.txt", 2},
                                                          {"../graphs/P_7.txt", 1}, {"../graphs/T_4_1_1.txt", 3},
                                                          {"../graphs/T_3_1_2.txt", 2}, {"../graphs/T_2_1_3.txt", 1},
                                                          {"../graphs/T_1_1_4.txt", 0}, {"../graphs/T_1_2_2.txt", 0},
                                                          {"../graphs/ExC.txt", 1}};
    std::vector<std::pair<std::string, int>> T_3 = {{"../graphs/T_1_1_2.txt", 3}, {"../graphs/T_1_2_2.txt", 5},
                                                    {"../graphs/T_1_4_2.txt", 9}, {"../graphs/T_2_1_3.txt", 4},
                                                    {"../graphs/P_3.txt", 1}, {"../graphs/P_7.txt", 3},
                                                    {"../graphs/T_3_3_4.txt", 9}};
    std::vector<std::pair<std::string, int>> T_1 = {{"../graphs/P_6.txt", 4}, {"../graphs/P_4.txt", 2},
                                                    {"../graphs/P_3.txt", 0}, {"../graphs/P_4.txt", 0},
                                                    {"../graphs/T_1_1_1.txt", 0}, {"../graphs/T_1_1_2.txt", 0},
                                                    {"../graphs/T_1_2_1.txt", 0}, {"../graphs/T_1_4_1.txt", 0},
                                                    {"../graphs/P_4.txt", 1}, {"../graphs/T_2_1_1.txt", 1},
                                                    {"../graphs/T_2_4_1.txt", 1}, {"../graphs/P_6.txt", 2}};

    std::vector<std::string> fine;
    std::vector<std::pair<std::string, int>> launching;
    int algoritm_type = 3;
    launching = unlimited;
    fine.resize(launching.size(), "no");
    int graph_ind = -1;
    for (auto graph_pair: launching) {
        std::cout << graph_pair.first << "------\n";
        graph_ind++;
        if (algoritm_type == 1) {
            std::string graph_file = graph_pair.first;
            Graph a;
            a = read_graph(graph_file);
            int second_root_number = graph_pair.second;
            Graph b;
            b = read_graph(graph_file);
            make_copies(a, b, second_root_number);
            if (percent_and_classic1(a, b, second_root_number, 50)) {
                fine[graph_ind] = graph_pair.first;
                write_graph(a, "../lol_nice.txt");
                a.antiedges.clear();
                create_antiedges(a);
                auto copies = search_occur(a, b);
                int count = 0;
                for (auto i: copies) {
                    if (!is_copy_unavoidable(a, i, second_root_number)) {
                        bool x = is_copy_unavoidable(a, i, second_root_number);
                        ++count;
                    }
                }
                std::cout << count << ' ' << copies.size() << "------\n";
            }
        } else {
            for (int x = 220; x < 237; ++x) {
                srand(x);
                std::string graph_file = graph_pair.first;
                Graph a;
                a = read_graph(graph_file);
                int second_root_number = graph_pair.second;
                Graph b;
                b = read_graph(graph_file);
                make_copies(a, b, second_root_number);
                if (algoritm_type == 2) {
                    if (percent_and_classic2(a, b, second_root_number, 38)) {
                        fine[graph_ind] = graph_pair.first;
                        write_graph(a, "../lol_nice.txt");
                        a.antiedges.clear();
                        create_antiedges(a);
                        auto copies = search_occur(a, b);
                        int count = 0;
                        for (auto i: copies) {
                            if (!is_copy_unavoidable(a, i, second_root_number)) {
                                ++count;
                            }
                        }
                        std::cout << count << ' ' << copies.size() << "------\n";
                        break;
                    }
                } else if (algoritm_type == 3) {
                    if (percent_and_classic3(a, b, second_root_number, 41)) {
                        fine[graph_ind] = graph_pair.first;
                        write_graph(a, "../lol_nice.txt");
                        a.antiedges.clear();
                        create_antiedges(a);
                        auto copies = search_occur(a, b);
                        int count = 0;
                        for (auto i: copies) {
                            if (!is_copy_unavoidable(a, i, second_root_number)) {
                                ++count;
                            }
                        }
                        std::cout << count << ' ' << copies.size() << "------\n";
                        break;
                    }
                } else {
                    if (percent_and_classic4(a, b, second_root_number, 39)) {
                        fine[graph_ind] = graph_pair.first;
                        write_graph(a, "../lol_nice.txt");
                        a.antiedges.clear();
                        create_antiedges(a);
                        auto copies = search_occur(a, b);
                        int count = 0;
                        for (auto i: copies) {
                            if (!is_copy_unavoidable(a, i, second_root_number)) {
                                ++count;
                            }
                        }
                        std::cout << count << ' ' << copies.size() << "------\n";
                        break;
                    }
                }
            }
        }
    }
    std::cout << '\n';
    for (auto i: fine) {
        std::cout << i << ' ';
    }

    return 0;
}

