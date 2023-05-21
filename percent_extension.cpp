//
// Created by Daniil Klochkov on 31.03.2023.
//
#include "percent_extension.h"

std::pair<int, int> delta_by_edge(Graph& big, Graph& small, int vertex1, int vertex2, int second_root_number) {
    int delta_avoid = 0;
    int delta_copy = 0;

    //обрабатываем появляющиеся копии
    auto new_copies = occur_with_edge(big, small, vertex1, vertex2);
    delta_copy += new_copies.size();
    for (auto i: new_copies) {
        delta_avoid += !is_copy_unavoidable(big, i, second_root_number);
    }

    //обрабатываем исчезающие копии
    auto iter = big.copies_and_comp.begin();
    while (iter != big.copies_and_comp.end()) {
        if (std::find(iter->vertexes.begin(), iter->vertexes.end(), vertex1) != iter->vertexes.end() &&
                std::find(iter->vertexes.begin(), iter->vertexes.end(), vertex2) != iter->vertexes.end()) {
            delta_copy -= 1;
            if (iter->avoidable) {
                delta_avoid -= 1;
            }
        }
        iter++;
    }

    //обрабатываем меняющие статус копии(самое сложное)
    for (Copy& copy: big.copies_and_comp) {
        if (std::find(copy.vertexes.begin(), copy.vertexes.end(), vertex1) != copy.vertexes.end()
        && std::find(copy.vertexes.begin(), copy.vertexes.end(), vertex2) != copy.vertexes.end()) {
            continue;
        } else if (find(copy.dsu, vertex1) != -1 && find(copy.dsu, vertex2) != -1 &&
            find(copy.dsu, vertex1) != find(copy.dsu, vertex2)) {
            bool found_uncloseable = false;
            int ver1_col = find(copy.dsu, vertex1);
            int ver2_col = find(copy.dsu, vertex2);
            for (int first_n: copy.first_e) {
                std::unordered_set<int> comps;
                for (int i: big.edges[first_n]) {
                    if (copy.dsu[i] != -1) {
                        if (find(copy.dsu, i) == ver1_col || find(copy.dsu, i) == ver2_col) {
                            comps.insert(ver1_col);
                            comps.insert(ver2_col);
                        } else {
                            comps.insert(find(copy.dsu, i));
                        }
                    }
                }
                for (int second_n: copy.second_e) {
                    if (second_n != first_n && !big.matrix[second_n][first_n]) {
                        bool good = true;
                        for (int i: big.edges[second_n]) {
                            if (comps.find(find(copy.dsu, i)) != comps.end()) {
                                good = false;
                                break;
                            }
                        }
                        if (good) {
                            found_uncloseable = true;
                            break;
                        }
                    }
                }
                if (found_uncloseable) {
                    break;
                }
            }
            if (found_uncloseable && copy.avoidable) {
                delta_avoid -= 1;
            } else if (!found_uncloseable && !copy.avoidable) {
                delta_avoid += 1;
            }
        } else if (find(copy.dsu, vertex1) != -1 && find(copy.dsu, vertex1)
                                                    != find(copy.dsu, vertex2)) {
            if (std::find(copy.vertexes.begin(), copy.vertexes.end(), vertex2) != copy.vertexes.end()) {
                std::vector<int> dsu = copy.dsu;
                int bad_comp = find(copy.dsu, vertex1);
                for (int i = 0; i < dsu.size(); ++i) {
                    if (find(copy.dsu, i) == bad_comp) {
                        dsu[i] = -2;
                    }
                }
                for (int i = 0; i < dsu.size(); ++i) {
                    if (dsu[i] == -2 && i != vertex1) {
                        std::vector<bool> visited;
                        visited.resize(dsu.size());
                        dfs(big, visited, copy.vertexes, i, i, dsu, vertex1);
                    }
                }
                dsu[vertex1] = -1;
                bool found_uncloseable = false;
                for (int first_n: copy.first_e) {
                    std::unordered_set<int> comps;
                    for (int i: big.edges[first_n]) {
                        if (dsu[i] != -1) {
                            comps.insert(find(dsu, i));
                        }
                    }
                    for (int second_n: copy.second_e) {
                        if (second_n != first_n && !big.matrix[second_n][first_n]) {
                            bool good = true;
                            for (int i: big.edges[second_n]) {
                                if (comps.find(find(dsu, i)) != comps.end()) {
                                    good = false;
                                    break;
                                }
                            }
                            if (good) {
                                found_uncloseable = true;
                                break;
                            }
                        }
                    }
                    if (found_uncloseable) {
                        break;
                    }
                    if (vertex2 == copy.vertexes[second_root_number] && !big.matrix[first_n][vertex1]) {
                        bool good = true;
                        for (int i: big.edges[vertex1]) {
                            if (comps.find(find(dsu, i)) != comps.end()) {
                                good = false;
                                break;
                            }
                        }
                        if (good) {
                            found_uncloseable = true;
                        }
                    }
                    if (found_uncloseable) {
                        break;
                    }
                }
                if (vertex2 == copy.vertexes[0]) {
                    std::unordered_set<int> comps;
                    for (int i: big.edges[vertex1]) {
                        if (dsu[i] != -1) {
                            comps.insert(find(dsu, i));
                        }
                    }
                    for (int second_n: copy.second_e) {
                        if (second_n != vertex1 && !big.matrix[second_n][vertex1]) {
                            bool good = true;
                            for (int i: big.edges[second_n]) {
                                if (comps.find(find(dsu, i)) != comps.end()) {
                                    good = false;
                                    break;
                                }
                            }
                            if (good) {
                                found_uncloseable = true;
                                break;
                            }
                        }
                    }
                }
                if (found_uncloseable && copy.avoidable) {
                    delta_avoid -= 1;
                } else if (!found_uncloseable && !copy.avoidable) {
                    delta_avoid += 1;
                }
            } else if (std::find(copy.first_e.begin(), copy.first_e.end(), vertex2) != copy.first_e.end()) {
                bool found_uncloseable = false;
                for (int first_n: copy.first_e) {
                    std::unordered_set<int> comps;
                    for (int i: big.edges[first_n]) {
                        if (copy.dsu[i] != -1) {
                            comps.insert(find(copy.dsu, i));
                        }
                    }
                    if (vertex2 == first_n) {
                        comps.insert(find(copy.dsu, vertex1));
                    }
                    for (int second_n: copy.second_e) {
                        if (second_n != first_n && !big.matrix[second_n][first_n]) {
                            bool good = true;
                            for (int i: big.edges[second_n]) {
                                if (comps.find(find(copy.dsu, i)) != comps.end()) {
                                    good = false;
                                    break;
                                }
                            }
                            if (vertex2 == second_n) {
                                if (comps.find(find(copy.dsu, vertex1)) != comps.end()) {
                                    good = false;
                                }
                            }
                            if (good) {
                                found_uncloseable = true;
                                break;
                            }
                        }
                    }
                    if (found_uncloseable) {
                        break;
                    }
                }
                if (found_uncloseable && copy.avoidable) {
                    delta_avoid -= 1;
                } else if (!found_uncloseable && !copy.avoidable) {
                    delta_avoid += 1;
                }
            } else if (std::find(copy.second_e.begin(), copy.second_e.end(), vertex2) != copy.second_e.end()) {
                bool found_uncloseable = false;
                for (int first_n: copy.first_e) {
                    std::unordered_set<int> comps;
                    for (int i: big.edges[first_n]) {
                        if (copy.dsu[i] != -1) {
                            comps.insert(find(copy.dsu, i));
                        }
                    }
                    if (vertex2 == first_n) {
                        comps.insert(find(copy.dsu, vertex1));
                    }
                    for (int second_n: copy.second_e) {
                        if (second_n != first_n && !big.matrix[second_n][first_n]) {
                            bool good = true;
                            for (int i: big.edges[second_n]) {
                                if (comps.find(find(copy.dsu, i)) != comps.end()) {
                                    good = false;
                                    break;
                                }
                            }
                            if (vertex2 == second_n) {
                                if (comps.find(find(copy.dsu, vertex1)) != comps.end()) {
                                    good = false;
                                }
                            }
                            if (good) {
                                found_uncloseable = true;
                                break;
                            }
                        }
                    }
                    if (found_uncloseable) {
                        break;
                    }
                }
                if (found_uncloseable && copy.avoidable) {
                    delta_avoid -= 1;
                } else if (!found_uncloseable && !copy.avoidable) {
                    delta_avoid += 1;
                }
            }
        } else if (find(copy.dsu, vertex2) != -1 && find(copy.dsu, vertex2)
                                                    != find(copy.dsu, vertex1)) {
            std::swap(vertex1, vertex2);
            if (std::find(copy.vertexes.begin(), copy.vertexes.end(), vertex2) != copy.vertexes.end()) {
                std::vector<int> dsu = copy.dsu;
                int bad_comp = find(copy.dsu, vertex1);
                for (int i = 0; i < dsu.size(); ++i) {
                    if (find(copy.dsu, i) == bad_comp) {
                        dsu[i] = -2;
                    }
                }
                for (int i = 0; i < dsu.size(); ++i) {
                    if (dsu[i] == -2 && i != vertex1) {
                        std::vector<bool> visited;
                        visited.resize(dsu.size());
                        dfs(big, visited, copy.vertexes, i, i, dsu, vertex1);
                    }
                }
                dsu[vertex1] = -1;
                bool found_uncloseable = false;
                for (int first_n: copy.first_e) {
                    std::unordered_set<int> comps;
                    for (int i: big.edges[first_n]) {
                        if (dsu[i] != -1) {
                            comps.insert(find(dsu, i));
                        }
                    }
                    for (int second_n: copy.second_e) {
                        if (second_n != first_n && !big.matrix[second_n][first_n]) {
                            bool good = true;
                            for (int i: big.edges[second_n]) {
                                if (comps.find(find(dsu, i)) != comps.end()) {
                                    good = false;
                                    break;
                                }
                            }
                            if (good) {
                                found_uncloseable = true;
                                break;
                            }
                        }
                    }
                    if (found_uncloseable) {
                        break;
                    }
                    if (vertex2 == copy.vertexes[second_root_number] && !big.matrix[first_n][vertex1]) {
                        bool good = true;
                        for (int i: big.edges[vertex1]) {
                            if (comps.find(find(dsu, i)) != comps.end()) {
                                good = false;
                                break;
                            }
                        }
                        if (good) {
                            found_uncloseable = true;
                        }
                    }
                    if (found_uncloseable) {
                        break;
                    }
                }
                if (vertex2 == copy.vertexes[0]) {
                    std::unordered_set<int> comps;
                    for (int i: big.edges[vertex1]) {
                        if (dsu[i] != -1) {
                            comps.insert(find(dsu, i));
                        }
                    }
                    for (int second_n: copy.second_e) {
                        if (second_n != vertex1 && !big.matrix[second_n][vertex1]) {
                            bool good = true;
                            for (int i: big.edges[second_n]) {
                                if (comps.find(find(dsu, i)) != comps.end()) {
                                    good = false;
                                    break;
                                }
                            }
                            if (good) {
                                found_uncloseable = true;
                                break;
                            }
                        }
                    }
                }
                if (found_uncloseable && copy.avoidable) {
                    delta_avoid -= 1;
                } else if (!found_uncloseable && !copy.avoidable) {
                    delta_avoid += 1;
                }
            } else if (std::find(copy.first_e.begin(), copy.first_e.end(), vertex2) != copy.first_e.end()) {
                bool found_uncloseable = false;
                for (int first_n: copy.first_e) {
                    std::unordered_set<int> comps;
                    for (int i: big.edges[first_n]) {
                        if (copy.dsu[i] != -1) {
                            comps.insert(find(copy.dsu, i));
                        }
                    }
                    if (vertex2 == first_n) {
                        comps.insert(find(copy.dsu, vertex1));
                    }
                    for (int second_n: copy.second_e) {
                        if (second_n != first_n && !big.matrix[second_n][first_n]) {
                            bool good = true;
                            for (int i: big.edges[second_n]) {
                                if (comps.find(find(copy.dsu, i)) != comps.end()) {
                                    good = false;
                                    break;
                                }
                            }
                            if (vertex2 == second_n) {
                                if (comps.find(find(copy.dsu, vertex1)) != comps.end()) {
                                    good = false;
                                }
                            }
                            if (good) {
                                found_uncloseable = true;
                                break;
                            }
                        }
                    }
                    if (found_uncloseable) {
                        break;
                    }
                }
                if (found_uncloseable && copy.avoidable) {
                    delta_avoid -= 1;
                } else if (!found_uncloseable && !copy.avoidable) {
                    delta_avoid += 1;
                }
            } else if (std::find(copy.second_e.begin(), copy.second_e.end(), vertex2) != copy.second_e.end()) {
                bool found_uncloseable = false;
                for (int first_n: copy.first_e) {
                    std::unordered_set<int> comps;
                    for (int i: big.edges[first_n]) {
                        if (copy.dsu[i] != -1) {
                            comps.insert(find(copy.dsu, i));
                        }
                    }
                    if (vertex2 == first_n) {
                        comps.insert(find(copy.dsu, vertex1));
                    }
                    for (int second_n: copy.second_e) {
                        if (second_n != first_n && !big.matrix[second_n][first_n]) {
                            bool good = true;
                            for (int i: big.edges[second_n]) {
                                if (comps.find(find(copy.dsu, i)) != comps.end()) {
                                    good = false;
                                    break;
                                }
                            }
                            if (vertex2 == second_n) {
                                if (comps.find(find(copy.dsu, vertex1)) != comps.end()) {
                                    good = false;
                                }
                            }
                            if (good) {
                                found_uncloseable = true;
                                break;
                            }
                        }
                    }
                    if (found_uncloseable) {
                        break;
                    }
                }
                if (found_uncloseable && copy.avoidable) {
                    delta_avoid -= 1;
                } else if (!found_uncloseable && !copy.avoidable) {
                    delta_avoid += 1;
                }
            }
        } else if (copy.dsu[vertex1] == -1 && copy.dsu[vertex2] == -1) {
            if (copy.vertexes[0] == copy.vertexes[second_root_number] && vertex1 != copy.vertexes[0]
            && std::find(copy.vertexes.begin(), copy.vertexes.end(), vertex1) != copy.vertexes.end()
            && std::find(copy.first_e.begin(), copy.first_e.end(), vertex2) != copy.first_e.end()) {
                bool found_uncloseable = false;
                for (int first_n: copy.first_e) {
                    if (first_n != vertex2) {
                        std::unordered_set<int> comps;
                        for (int i: big.edges[first_n]) {
                            if (copy.dsu[i] != -1) {
                                comps.insert(find(copy.dsu, i));
                            }
                        }
                        for (int second_n: copy.second_e) {
                            if (second_n != first_n && !big.matrix[second_n][first_n]) {
                                if (second_n != vertex2) {
                                    bool good = true;
                                    for (int i: big.edges[second_n]) {
                                        if (comps.find(find(copy.dsu, i)) != comps.end()) {
                                            good = false;
                                            break;
                                        }
                                    }
                                    if (good) {
                                        found_uncloseable = true;
                                        break;
                                    }
                                }
                            }
                        }
                        if (found_uncloseable) {
                            break;
                        }
                    }
                }
                if (found_uncloseable && copy.avoidable) {
                    delta_avoid -= 1;
                } else if (!found_uncloseable && !copy.avoidable) {
                    delta_avoid += 1;
                }
            } else if (copy.vertexes[0] == copy.vertexes[second_root_number] && vertex2 != copy.vertexes[0]
                       && std::find(copy.vertexes.begin(), copy.vertexes.end(), vertex2) != copy.vertexes.end()
                       && std::find(copy.first_e.begin(), copy.first_e.end(), vertex1) != copy.first_e.end()) {
                std::swap(vertex1, vertex2);
                bool found_uncloseable = false;
                for (int first_n: copy.first_e) {
                    if (first_n != vertex2) {
                        std::unordered_set<int> comps;
                        for (int i: big.edges[first_n]) {
                            if (copy.dsu[i] != -1) {
                                comps.insert(find(copy.dsu, i));
                            }
                        }
                        for (int second_n: copy.second_e) {
                            if (second_n != first_n && !big.matrix[second_n][first_n]) {
                                if (second_n != vertex2) {
                                    bool good = true;
                                    for (int i: big.edges[second_n]) {
                                        if (comps.find(find(copy.dsu, i)) != comps.end()) {
                                            good = false;
                                            break;
                                        }
                                    }
                                    if (good) {
                                        found_uncloseable = true;
                                        break;
                                    }
                                }
                            }
                        }
                        if (found_uncloseable) {
                            break;
                        }
                    }
                }
                if (found_uncloseable && copy.avoidable) {
                    delta_avoid -= 1;
                } else if (!found_uncloseable && !copy.avoidable) {
                    delta_avoid += 1;
                }
            } else if (copy.vertexes[0] != copy.vertexes[second_root_number]
            && std::find(copy.vertexes.begin(), copy.vertexes.end(), vertex1) != copy.vertexes.end()
            && std::find(copy.first_e.begin(), copy.first_e.end(), vertex2) != copy.first_e.end()) {
                bool found_unclosable = false;
                for (int first_n: copy.first_e) {
                    if (first_n != vertex2) {
                        std::unordered_set<int> comps;
                        for (int i: big.edges[first_n]) {
                            if (copy.dsu[i] != -1) {
                                comps.insert(find(copy.dsu, i));
                            }
                        }
                        for (int second_n: copy.second_e) {
                            if (second_n != first_n && !big.matrix[second_n][first_n]) {
                                bool good = true;
                                for (int i: big.edges[second_n]) {
                                    if (comps.find(find(copy.dsu, i)) != comps.end()) {
                                        good = false;
                                        break;
                                    }
                                }
                                if (good) {
                                    found_unclosable = true;
                                    break;
                                }
                            }
                        }
                        if (found_unclosable) {
                            break;
                        }
                    }
                }
                if (found_unclosable && copy.avoidable) {
                    delta_avoid -= 1;
                } else if (!found_unclosable && !copy.avoidable) {
                    delta_avoid += 1;
                }
            } else if (copy.vertexes[0] != copy.vertexes[second_root_number]
                       && std::find(copy.vertexes.begin(), copy.vertexes.end(), vertex2) != copy.vertexes.end()
                       && std::find(copy.first_e.begin(), copy.first_e.end(), vertex1) != copy.first_e.end()) {
                std::swap(vertex1, vertex2);
                bool found_unclosable = false;
                for (int first_n: copy.first_e) {
                    if (first_n != vertex2) {
                        std::unordered_set<int> comps;
                        for (int i: big.edges[first_n]) {
                            if (copy.dsu[i] != -1) {
                                comps.insert(find(copy.dsu, i));
                            }
                        }
                        for (int second_n: copy.second_e) {
                            if (second_n != first_n && !big.matrix[second_n][first_n]) {
                                bool good = true;
                                for (int i: big.edges[second_n]) {
                                    if (comps.find(find(copy.dsu, i)) != comps.end()) {
                                        good = false;
                                        break;
                                    }
                                }
                                if (good) {
                                    found_unclosable = true;
                                    break;
                                }
                            }
                        }
                        if (found_unclosable) {
                            break;
                        }
                    }
                }
                if (found_unclosable && copy.avoidable) {
                    delta_avoid -= 1;
                } else if (!found_unclosable && !copy.avoidable) {
                    delta_avoid += 1;
                }
            } else if (copy.vertexes[0] != copy.vertexes[second_root_number]
                       && std::find(copy.vertexes.begin(), copy.vertexes.end(), vertex1) != copy.vertexes.end()
                       && std::find(copy.second_e.begin(), copy.second_e.end(), vertex2) != copy.second_e.end()) {
                bool found_unclosable = false;
                for (int first_n: copy.second_e) {
                    if (first_n != vertex2) {
                        std::unordered_set<int> comps;
                        for (int i: big.edges[first_n]) {
                            if (copy.dsu[i] != -1) {
                                comps.insert(find(copy.dsu, i));
                            }
                        }
                        for (int second_n: copy.first_e) {
                            if (second_n != first_n && !big.matrix[second_n][first_n]) {
                                bool good = true;
                                for (int i: big.edges[second_n]) {
                                    if (comps.find(find(copy.dsu, i)) != comps.end()) {
                                        good = false;
                                        break;
                                    }
                                }
                                if (good) {
                                    found_unclosable = true;
                                    break;
                                }
                            }
                        }
                        if (found_unclosable) {
                            break;
                        }
                    }
                }
                if (found_unclosable && copy.avoidable) {
                    delta_avoid -= 1;
                } else if (!found_unclosable && !copy.avoidable) {
                    delta_avoid += 1;
                }
            } else if (copy.vertexes[0] != copy.vertexes[second_root_number]
                       && std::find(copy.vertexes.begin(), copy.vertexes.end(), vertex2) != copy.vertexes.end()
                       && std::find(copy.second_e.begin(), copy.second_e.end(), vertex1) != copy.second_e.end()) {
                std::swap(vertex1, vertex2);
                bool found_unclosable = false;
                for (int first_n: copy.second_e) {
                    if (first_n != vertex2) {
                        std::unordered_set<int> comps;
                        for (int i: big.edges[first_n]) {
                            if (copy.dsu[i] != -1) {
                                comps.insert(find(copy.dsu, i));
                            }
                        }
                        for (int second_n: copy.first_e) {
                            if (second_n != first_n && !big.matrix[second_n][first_n]) {
                                bool good = true;
                                for (int i: big.edges[second_n]) {
                                    if (comps.find(find(copy.dsu, i)) != comps.end()) {
                                        good = false;
                                        break;
                                    }
                                }
                                if (good) {
                                    found_unclosable = true;
                                    break;
                                }
                            }
                        }
                        if (found_unclosable) {
                            break;
                        }
                    }
                }
                if (found_unclosable && copy.avoidable) {
                    delta_avoid -= 1;
                } else if (!found_unclosable && !copy.avoidable) {
                    delta_avoid += 1;
                }
            } else if (copy.vertexes[0] != copy.vertexes[second_root_number]
                       && std::find(copy.first_e.begin(), copy.first_e.end(), vertex1) != copy.first_e.end()
                       && std::find(copy.second_e.begin(), copy.second_e.end(), vertex2) != copy.second_e.end()) {
                bool found_unclosable = false;
                for (int first_n: copy.first_e) {
                    std::unordered_set<int> comps;
                    for (int i: big.edges[first_n]) {
                        if (copy.dsu[i] != -1) {
                            comps.insert(find(copy.dsu, i));
                        }
                    }
                    for (int second_n: copy.second_e) {
                        if (second_n != first_n && !big.matrix[second_n][first_n] && (vertex1 != first_n
                                                                                        || vertex2 != second_n)) {
                            bool good = true;
                            for (int i: big.edges[second_n]) {
                                if (comps.find(find(copy.dsu, i)) != comps.end()) {
                                    good = false;
                                    break;
                                }
                            }
                            if (good) {
                                found_unclosable = true;
                                break;
                            }
                        }
                    }
                    if (found_unclosable) {
                        break;
                    }
                }
                if (found_unclosable && copy.avoidable) {
                    delta_avoid -= 1;
                } else if (!found_unclosable && !copy.avoidable) {
                    delta_avoid += 1;
                }
            } else if (copy.vertexes[0] != copy.vertexes[second_root_number] && std::find(copy.first_e.begin(), copy.first_e.end(), vertex2) != copy.first_e.end()
                       && std::find(copy.second_e.begin(), copy.second_e.end(), vertex1) != copy.second_e.end()) {
                std::swap(vertex2, vertex1);
                bool found_unclosable = false;
                for (int first_n: copy.first_e) {
                    std::unordered_set<int> comps;
                    for (int i: big.edges[first_n]) {
                        if (copy.dsu[i] != -1) {
                            comps.insert(find(copy.dsu, i));
                        }
                    }
                    for (int second_n: copy.second_e) {
                        if (second_n != first_n && !big.matrix[second_n][first_n] && (vertex1 != first_n
                                                                                        || vertex2 != second_n)) {
                            bool good = true;
                            for (int i: big.edges[second_n]) {
                                if (comps.find(find(copy.dsu, i)) != comps.end()) {
                                    good = false;
                                    break;
                                }
                            }
                            if (good) {
                                found_unclosable = true;
                                break;
                            }
                        }
                    }
                    if (found_unclosable) {
                        break;
                    }
                }
                if (found_unclosable && copy.avoidable) {
                    delta_avoid -= 1;
                } else if (!found_unclosable && !copy.avoidable) {
                    delta_avoid += 1;
                }
            } else if (copy.vertexes[0] == copy.vertexes[second_root_number]
                       && std::find(copy.second_e.begin(), copy.second_e.end(), vertex1) != copy.second_e.end()
                       && std::find(copy.second_e.begin(), copy.second_e.end(), vertex2) != copy.second_e.end()) {
                bool found_unclosable = false;
                for (int first_n: copy.first_e) {
                    std::unordered_set<int> comps;
                    for (int i: big.edges[first_n]) {
                        if (copy.dsu[i] != -1) {
                            comps.insert(find(copy.dsu, i));
                        }
                    }
                    for (int second_n: copy.first_e) {
                        if (second_n != first_n && !big.matrix[second_n][first_n] && (vertex1 != first_n
                                                                                        || vertex2 != second_n) && (vertex2 != first_n || vertex1 != second_n)) {
                            bool good = true;
                            for (int i: big.edges[second_n]) {
                                if (comps.find(find(copy.dsu, i)) != comps.end()) {
                                    good = false;
                                    break;
                                }
                            }
                            if (good) {
                                found_unclosable = true;
                                break;
                            }
                        }
                    }
                    if (found_unclosable) {
                        break;
                    }
                }
                if (found_unclosable && copy.avoidable) {
                    delta_avoid -= 1;
                } else if (!found_unclosable && !copy.avoidable) {
                    delta_avoid += 1;
                }
            }
        }
    }

    return {delta_avoid, delta_copy};
}
