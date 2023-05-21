//
// Created by Daniil Klochkov on 04.04.2023.
//
#include "compile_extensions.h"

bool percent_and_classic1(Graph& big, Graph& small, int second_root_number, int max_vert_count) {
    std::pair<int, int> delta;
    if (big.bad_copies == 0) {
        return true;
    }
    while (big.edges.size() < max_vert_count) {
        bool ok = false;
        for (int i = big.edges.size() - 1; i > -1; --i) {
            for (int j = i % big.edges.size() - 1; j > -1; --j) {
                if (!big.matrix[i % big.edges.size()][j]) {
                    delta = delta_by_edge(big, small, i % big.edges.size(), j, second_root_number);
                    if (big.copies_and_comp.size() + delta.second != 0 && big.bad_copies + delta.first == 0) {
                        big.add_edge(small, i % big.edges.size(), j, second_root_number);
                        std::cout << big.bad_copies << ' ' << big.copies_and_comp.size() << ' ' << big.edges.size() << '\n';
                        return true;
                    } else if (big.copies_and_comp.size() + delta.second != 0 && (((float)(big.bad_copies + delta.first)
                    / (float)(big.copies_and_comp.size() + delta.second)) < (float)big.bad_copies
                    / (float)big.copies_and_comp.size())) {
                        big.add_edge(small, i % big.edges.size(), j, second_root_number);
                        ok = true;
                    }
                }
            }
        }
        if (!ok) {
            classic_extension(big, small, second_root_number, 2 * max_vert_count);
            if (big.bad_copies == 0) {
                std::cout << big.bad_copies << ' ' << big.copies_and_comp.size() << ' ' << big.edges.size() << "\n";
                return true;
            }
        }
        std::cout << big.bad_copies << ' ' << big.copies_and_comp.size() << ' ' << big.edges.size() << '\n';
        fflush(stdout);
    }
    std::cout << big.bad_copies << ' ' << big.copies_and_comp.size() << ' ' << big.edges.size() << '\n';
    std::cout << ":( ";
    fflush(stdout);
    return false;
}

bool percent_and_classic2(Graph& big, Graph& small, int second_root_number, int max_vert_count) {
    std::pair<int, int> delta;
    if (big.bad_copies == 0) {
        return true;
    }
    while (big.edges.size() < max_vert_count) {
        bool ok = false;
        for (int i = big.edges.size() - 1; i > -1; --i) {
            for (int j = i - 1; j > -1; --j) {
                if (!big.matrix[i][j]) {
                    delta = delta_by_edge(big, small, i, j, second_root_number);
                    if (big.copies_and_comp.size() + delta.second != 0 && big.bad_copies + delta.first == 0) {
                        big.add_edge(small, i, j, second_root_number);
                        std::cout << "|" << big.bad_copies << ' ' << big.copies_and_comp.size() << ' ' << big.edges.size() << "|";
                        return true;
                    } else if (delta.first <= 10 && delta.second <= 10 && big.copies_and_comp.size() + delta.second
                    != 0 && (((float)(big.bad_copies + delta.first) / (float)(big.copies_and_comp.size() + delta.second))
                    < (float)big.bad_copies / (float)big.copies_and_comp.size())) {
                        big.add_edge(small, i, j, second_root_number);
                        std::cout << "|" << big.bad_copies << ' ' << big.copies_and_comp.size() << ' ' << big.edges.size() << "|";
                        ok = true;
                    }
                }
            }
        }
        if (!ok) {
            classic_extension(big, small, second_root_number, 2 * max_vert_count, 1, 6);
            if (big.bad_copies == 0) {
                std::cout << "|" << big.bad_copies << ' ' << big.copies_and_comp.size() << ' ' << big.edges.size() << "|";
                return true;
            }
        }
        std::cout << "||" << big.bad_copies << ' ' << big.copies_and_comp.size() << ' ' << big.edges.size() << "|";
        fflush(stdout);
    }
    std::cout << big.bad_copies << ' ' << big.copies_and_comp.size() << ' ' << big.edges.size() << ":(\n";
    return false;
}

bool percent_and_classic3(Graph& big, Graph& small, int second_root_number, int max_vert_count) {
    std::pair<int, int> delta;
    if (big.bad_copies == 0) {
        return true;
    }
    while (big.edges.size() < max_vert_count) {
        bool ok = false;
        for (int i = 0; i < big.edges.size(); ++i) {
            for (int j = 0; j < i; ++j) {
                if (!big.matrix[i][j]) {
                    delta = delta_by_edge(big, small, i, j, second_root_number);
                    if (big.copies_and_comp.size() + delta.second != 0 && big.bad_copies + delta.first == 0) {
                        big.add_edge(small, i, j, second_root_number);
                        std::cout << big.bad_copies << ' ' << big.copies_and_comp.size() << ' ' << big.edges.size() << "***";
                        return true;
                    } else if (delta.first <= 10 && delta.second <= 10 && big.copies_and_comp.size() + delta.second != 0
                    && (((float)(big.bad_copies + delta.first) / (float)(big.copies_and_comp.size() + delta.second))
                    < (float)big.bad_copies / (float)big.copies_and_comp.size())) {
                        big.add_edge(small, i, j, second_root_number);
                        ok = true;
                    }
                }
            }
        }
        if (!ok) {
            classic_extension(big, small, second_root_number, 2 * max_vert_count, 1, 6);
            if (big.bad_copies == 0) {
                return true;
            }
        }
        std::cout << big.bad_copies << ' ' << big.copies_and_comp.size() << ' ' << big.edges.size() << "***";
    }
    std::cout << big.bad_copies << ' ' << big.copies_and_comp.size() << ' ' << big.edges.size() << ":(\n";
    fflush(stdout);
    return false;
}

bool percent_and_classic4(Graph& big, Graph& small, int second_root_number, int max_vert_count) {
    std::pair<int, int> delta;
    if (big.bad_copies == 0) {
        return true;
    }
    while (big.edges.size() < max_vert_count) {
        bool ok = false;
        int random_move = rand();
        for (int i = 0; i < big.edges.size(); ++i) {
            for (int j = 0; j < (i + random_move) % big.edges.size(); ++j) {
                if (!big.matrix[(i + random_move) % big.edges.size()][j]) {
                    delta = delta_by_edge(big, small, (i + random_move) % big.edges.size(), j, second_root_number);
                    if (big.copies_and_comp.size() + delta.second != 0 && big.bad_copies + delta.first == 0) {
                        big.add_edge(small, (i + random_move) % big.edges.size(), j, second_root_number);
                        std::cout << big.bad_copies << ' ' << big.copies_and_comp.size() << ' ' << big.edges.size() << "***";
                        return true;
                    } else if (delta.first <= 10 && delta.second <= 10 && big.copies_and_comp.size() + delta.second != 0
                               && (((float)(big.bad_copies + delta.first) / (float)(big.copies_and_comp.size() + delta.second))
                                   < (float)big.bad_copies / (float)big.copies_and_comp.size())) {
                        big.add_edge(small, (i + random_move) % big.edges.size(), j, second_root_number);
                        ok = true;
                    }
                }
            }
        }
        if (!ok) {
            classic_extension(big, small, second_root_number, 2 * max_vert_count, 1, 6);
            if (big.bad_copies == 0) {
                return true;
            }
        }
        std::cout << big.bad_copies << ' ' << big.copies_and_comp.size() << ' ' << big.edges.size() << "***";
    }
    std::cout << big.bad_copies << ' ' << big.copies_and_comp.size() << ' ' << big.edges.size() << ":(\n";
    fflush(stdout);
    return false;
}