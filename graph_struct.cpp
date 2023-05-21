//
// Created by Daniil Klochkov on 11.01.2023.
//
#include "graph_struct.h"
#include "set"
#include "unordered_set"

int Graph::add_vertex() {
    edges.emplace_back();
    antiedges.emplace_back(); // Мб они и не понадобятся, пока тут пусто оставлю
    for (auto& _: matrix) {
        _.push_back(false);
    }
    std::vector<bool> tmp(edges.size(), false);
    matrix.push_back(tmp);
    matrix[matrix.size() - 1].resize(matrix.size());
    for (auto& copy: this->copies_and_comp) {
        copy.dsu.emplace_back(copy.dsu.size());
    }
    return edges.size() - 1;
}

void Graph::add_edge(Graph& small, int vertex1, int vertex2, int second_root_number) {

    // обрабатываем исчезающие копии
    auto iter = copies_and_comp.begin();
    while (iter != copies_and_comp.end()) {
        if (std::find(iter->vertexes.begin(), iter->vertexes.end(), vertex1) != iter->vertexes.end() &&
            std::find(iter->vertexes.begin(), iter->vertexes.end(), vertex2) != iter->vertexes.end()) {
            if (iter->avoidable) {
                --bad_copies;
            }
            iter = copies_and_comp.erase(iter);
        } else {
            iter++;
        }
    }

    // обрабатываем меняющие статус копии(самое сложное)
    for (Copy& copy: this->copies_and_comp) {
        if (find(copy.dsu, vertex1) != -1 && find(copy.dsu, vertex2) != -1 &&
            find(copy.dsu, vertex1) != find(copy.dsu, vertex2)) {
            bool found_uncloseable = false;
            copy.dsu[find(copy.dsu, vertex2)] = find(copy.dsu, vertex1);
            for (int first_n: copy.first_e) {
                std::unordered_set<int> comps;
                for (int i: this->edges[first_n]) {
                    if (copy.dsu[i] != -1) {
                        comps.insert(find(copy.dsu, i));
                    }
                }
                for (int second_n: copy.second_e) {
                    if (second_n != first_n && !this->matrix[second_n][first_n]) {
                        bool good = true;
                        for (int i: this->edges[second_n]) {
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
                copy.avoidable = false;
                this->bad_copies -= 1;
            } else if (!found_uncloseable && !copy.avoidable) {
                copy.avoidable = true;
                this->bad_copies += 1;
            }
        } else if (find(copy.dsu, vertex1) != -1 && find(copy.dsu, vertex1)
                                                    != find(copy.dsu, vertex2)) {
            if (std::find(copy.vertexes.begin(), copy.vertexes.end(), vertex2) != copy.vertexes.end()) {
                std::vector<int> dsu = copy.dsu;
                int bad_comp = find(dsu, vertex1);
                for (int i = 0; i < copy.dsu.size(); ++i) {
                    if (find(copy.dsu, i) == bad_comp) {
                        dsu[i] = -2;
                    }
                }
                for (int i = 0; i < dsu.size(); ++i) {
                    if (dsu[i] == -2 && i != vertex1) {
                        std::vector<bool> visited;
                        visited.resize(dsu.size());
                        dfs(*this, visited, copy.vertexes, i, i, dsu, vertex1);
                    }
                }
                dsu[vertex1] = -1;
                copy.dsu = dsu;
                bool found_uncloseable = false;
                if (vertex2 == copy.vertexes[0]) {
                    copy.first_e.push_back(vertex1);
                }
                if (vertex2 == copy.vertexes[second_root_number]) {
                    copy.second_e.push_back(vertex1);
                }
                for (int first_n: copy.first_e) {
                    std::unordered_set<int> comps;
                    for (int i: this->edges[first_n]) {
                        if (copy.dsu[i] != -1) {
                            comps.insert(find(copy.dsu, i));
                        }
                    }
                    for (int second_n: copy.second_e) {
                        if (second_n != first_n && !this->matrix[second_n][first_n]) {
                            bool good = true;
                            for (int i: this->edges[second_n]) {
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
                    copy.avoidable = false;
                    this->bad_copies -= 1;
                } else if (!found_uncloseable && !copy.avoidable) {
                    copy.avoidable = true;
                    this->bad_copies -= 1;
                }
            } else if (std::find(copy.first_e.begin(), copy.first_e.end(), vertex2) != copy.first_e.end()) {
                bool found_uncloseable = false;
                for (int first_n: copy.first_e) {
                    std::unordered_set<int> comps;
                    for (int i: this->edges[first_n]) {
                        if (copy.dsu[i] != -1) {
                            comps.insert(find(copy.dsu, i));
                        }
                    }
                    if (first_n == vertex2) {
                        comps.insert(find(copy.dsu, vertex1));
                    }
                    for (int second_n: copy.second_e) {
                        if (second_n != first_n && !this->matrix[second_n][first_n]) {
                            bool good = true;
                            for (int i: this->edges[second_n]) {
                                if (comps.find(find(copy.dsu, i)) != comps.end()) {
                                    good = false;
                                    break;
                                }
                            }
                            if (second_n == vertex2) {
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
                    copy.avoidable = false;
                    this->bad_copies -= 1;
                } else if (!found_uncloseable && !copy.avoidable) {
                    copy.avoidable = true;
                    this->bad_copies += 1;
                }
            } else if (std::find(copy.second_e.begin(), copy.second_e.end(), vertex2) != copy.second_e.end()) {
                bool found_uncloseable = false;
                for (int first_n: copy.first_e) {
                    std::unordered_set<int> comps;
                    for (int i: this->edges[first_n]) {
                        if (copy.dsu[i] != -1) {
                            comps.insert(find(copy.dsu, i));
                        }
                    }
                    if (first_n == vertex2) {
                        comps.insert(find(copy.dsu, vertex1));
                    }
                    for (int second_n: copy.second_e) {
                        if (second_n != first_n && !this->matrix[second_n][first_n]) {
                            bool good = true;
                            for (int i: this->edges[second_n]) {
                                if (comps.find(find(copy.dsu, i)) != comps.end()) {
                                    good = false;
                                    break;
                                }
                            }
                            if (second_n == vertex2) {
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
                    copy.avoidable = false;
                    this->bad_copies -= 1;
                } else if (!found_uncloseable && !copy.avoidable) {
                    copy.avoidable = true;
                    this->bad_copies += 1;
                }
            }
        } else if (find(copy.dsu, vertex2) != -1 && find(copy.dsu, vertex2)
                                                    != find(copy.dsu, vertex1)) {
            std::swap(vertex1, vertex2);
            if (std::find(copy.vertexes.begin(), copy.vertexes.end(), vertex2) != copy.vertexes.end()) {
                std::vector<int> dsu = copy.dsu;
                int bad_comp = find(dsu, vertex1);
                for (int i = 0; i < copy.dsu.size(); ++i) {
                    if (find(copy.dsu, i) == bad_comp) {
                        dsu[i] = -2;
                    }
                }
                for (int i = 0; i < dsu.size(); ++i) {
                    if (dsu[i] == -2 && i != vertex1) {
                        std::vector<bool> visited;
                        visited.resize(dsu.size());
                        dfs(*this, visited, copy.vertexes, i, i, dsu, vertex1);
                    }
                }
                dsu[vertex1] = -1;
                copy.dsu = dsu;
                bool found_uncloseable = false;
                if (vertex2 == copy.vertexes[0]) {
                    copy.first_e.push_back(vertex1);
                }
                if (vertex2 == copy.vertexes[second_root_number]) {
                    copy.second_e.push_back(vertex1);
                }
                for (int first_n: copy.first_e) {
                    std::unordered_set<int> comps;
                    for (int i: this->edges[first_n]) {
                        if (copy.dsu[i] != -1) {
                            comps.insert(find(copy.dsu, i));
                        }
                    }
                    for (int second_n: copy.second_e) {
                        if (second_n != first_n && !this->matrix[second_n][first_n]) {
                            bool good = true;
                            for (int i: this->edges[second_n]) {
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
                    copy.avoidable = false;
                    this->bad_copies -= 1;
                } else if (!found_uncloseable && !copy.avoidable) {
                    copy.avoidable = true;
                    this->bad_copies -= 1;
                }
            } else if (std::find(copy.first_e.begin(), copy.first_e.end(), vertex2) != copy.first_e.end()) {
                bool found_uncloseable = false;
                for (int first_n: copy.first_e) {
                    std::unordered_set<int> comps;
                    for (int i: this->edges[first_n]) {
                        if (copy.dsu[i] != -1) {
                            comps.insert(find(copy.dsu, i));
                        }
                    }
                    if (first_n == vertex2) {
                        comps.insert(find(copy.dsu, vertex1));
                    }
                    for (int second_n: copy.second_e) {
                        if (second_n != first_n && !this->matrix[second_n][first_n]) {
                            bool good = true;
                            for (int i: this->edges[second_n]) {
                                if (comps.find(find(copy.dsu, i)) != comps.end()) {
                                    good = false;
                                    break;
                                }
                            }
                            if (second_n == vertex2) {
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
                    copy.avoidable = false;
                    this->bad_copies -= 1;
                } else if (!found_uncloseable && !copy.avoidable) {
                    copy.avoidable = true;
                    this->bad_copies += 1;
                }
            } else if (std::find(copy.second_e.begin(), copy.second_e.end(), vertex2) != copy.second_e.end()) {
                bool found_uncloseable = false;
                for (int first_n: copy.first_e) {
                    std::unordered_set<int> comps;
                    for (int i: this->edges[first_n]) {
                        if (copy.dsu[i] != -1) {
                            comps.insert(find(copy.dsu, i));
                        }
                    }
                    if (first_n == vertex2) {
                        comps.insert(find(copy.dsu, vertex1));
                    }
                    for (int second_n: copy.second_e) {
                        if (second_n != first_n && !this->matrix[second_n][first_n]) {
                            bool good = true;
                            for (int i: this->edges[second_n]) {
                                if (comps.find(find(copy.dsu, i)) != comps.end()) {
                                    good = false;
                                    break;
                                }
                            }
                            if (second_n == vertex2) {
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
                    copy.avoidable = false;
                    this->bad_copies -= 1;
                } else if (!found_uncloseable && !copy.avoidable) {
                    copy.avoidable = true;
                    this->bad_copies += 1;
                }
            }
        } else if (copy.dsu[vertex1] == -1 && copy.dsu[vertex2] == -1) {
            if (copy.vertexes[0] == copy.vertexes[second_root_number]
                && std::find(copy.vertexes.begin(), copy.vertexes.end(), vertex1) != copy.vertexes.end()
                && std::find(copy.first_e.begin(), copy.first_e.end(), vertex2) != copy.first_e.end()) {
                bool found_uncloseable = false;
                copy.first_e.erase(std::find(copy.first_e.begin(), copy.first_e.end(), vertex2));
                copy.second_e.erase(std::find(copy.second_e.begin(), copy.second_e.end(), vertex2));
                for (int first_n: copy.first_e) {
                    std::unordered_set<int> comps;
                    for (int i: this->edges[first_n]) {
                        if (copy.dsu[i] != -1) {
                            comps.insert(find(copy.dsu, i));
                        }
                    }
                    for (int second_n: copy.second_e) {
                        if (second_n != first_n && !this->matrix[second_n][first_n]) {
                            if (second_n != vertex2) {
                                bool good = true;
                                for (int i: this->edges[second_n]) {
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
                if (found_uncloseable && copy.avoidable) {
                    copy.avoidable = false;
                    this->bad_copies -= 1;
                } else if (!found_uncloseable && !copy.avoidable) {
                    copy.avoidable = true;
                    this->bad_copies += 1;
                }
            } else if (copy.vertexes[0] == copy.vertexes[second_root_number]
                       && std::find(copy.vertexes.begin(), copy.vertexes.end(), vertex2) != copy.vertexes.end()
                       && std::find(copy.first_e.begin(), copy.first_e.end(), vertex1) != copy.first_e.end()) {
                std::swap(vertex1, vertex2);
                bool found_uncloseable = false;
                copy.first_e.erase(std::find(copy.first_e.begin(), copy.first_e.end(), vertex2));
                copy.second_e.erase(std::find(copy.second_e.begin(), copy.second_e.end(), vertex2));
                for (int first_n: copy.first_e) {
                    std::unordered_set<int> comps;
                    for (int i: this->edges[first_n]) {
                        if (copy.dsu[i] != -1) {
                            comps.insert(find(copy.dsu, i));
                        }
                    }
                    for (int second_n: copy.second_e) {
                        if (second_n != first_n && !this->matrix[second_n][first_n]) {
                            if (second_n != vertex2) {
                                bool good = true;
                                for (int i: this->edges[second_n]) {
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
                if (found_uncloseable && copy.avoidable) {
                    copy.avoidable = false;
                    this->bad_copies -= 1;
                } else if (!found_uncloseable && !copy.avoidable) {
                    copy.avoidable = true;
                    this->bad_copies += 1;
                }
            } else if (copy.vertexes[0] != copy.vertexes[second_root_number]
                       && std::find(copy.vertexes.begin(), copy.vertexes.end(), vertex1) != copy.vertexes.end()
                       && std::find(copy.first_e.begin(), copy.first_e.end(), vertex2) != copy.first_e.end()) {
                copy.first_e.erase(std::find(copy.first_e.begin(), copy.first_e.end(), vertex2));
                bool found_uncloseable = false;
                for (int first_n: copy.first_e) {
                    std::unordered_set<int> comps;
                    for (int i: this->edges[first_n]) {
                        if (copy.dsu[i] != -1) {
                            comps.insert(find(copy.dsu, i));
                        }
                    }
                    for (int second_n: copy.second_e) {
                        if (second_n != first_n && !this->matrix[second_n][first_n]) {
                            bool good = true;
                            for (int i: this->edges[second_n]) {
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
                    copy.avoidable = false;
                    this->bad_copies -= 1;
                } else if (!found_uncloseable && !copy.avoidable) {
                    copy.avoidable = true;
                    this->bad_copies += 1;
                }
            } else if (copy.vertexes[0] != copy.vertexes[second_root_number]
                       && std::find(copy.vertexes.begin(), copy.vertexes.end(), vertex2) != copy.vertexes.end()
                       && std::find(copy.first_e.begin(), copy.first_e.end(), vertex1) != copy.first_e.end()) {
                std::swap(vertex1, vertex2);
                copy.first_e.erase(std::find(copy.first_e.begin(), copy.first_e.end(), vertex2));
                bool found_uncloseable = false;
                for (int first_n: copy.first_e) {
                    std::unordered_set<int> comps;
                    for (int i: this->edges[first_n]) {
                        if (copy.dsu[i] != -1) {
                            comps.insert(find(copy.dsu, i));
                        }
                    }
                    for (int second_n: copy.second_e) {
                        if (second_n != first_n && !this->matrix[second_n][first_n]) {
                            bool good = true;
                            for (int i: this->edges[second_n]) {
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
                    copy.avoidable = false;
                    this->bad_copies -= 1;
                } else if (!found_uncloseable && !copy.avoidable) {
                    copy.avoidable = true;
                    this->bad_copies += 1;
                }
            } else if (copy.vertexes[0] != copy.vertexes[second_root_number]
                       && std::find(copy.vertexes.begin(), copy.vertexes.end(), vertex1) != copy.vertexes.end()
                       && std::find(copy.second_e.begin(), copy.second_e.end(), vertex2) != copy.second_e.end()) {
                copy.second_e.erase(std::find(copy.second_e.begin(), copy.second_e.end(), vertex2));
                bool found_uncloseable = false;
                for (int first_n: copy.second_e) {
                    std::unordered_set<int> comps;
                    for (int i: this->edges[first_n]) {
                        if (copy.dsu[i] != -1) {
                            comps.insert(find(copy.dsu, i));
                        }
                    }
                    for (int second_n: copy.first_e) {
                        if (second_n != first_n && !this->matrix[second_n][first_n]) {
                            bool good = true;
                            for (int i: this->edges[second_n]) {
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
                    copy.avoidable = false;
                    this->bad_copies -= 1;
                } else if (!found_uncloseable && !copy.avoidable) {
                    copy.avoidable = true;
                    this->bad_copies += 1;
                }
            } else if (copy.vertexes[0] != copy.vertexes[second_root_number]
                        && std::find(copy.vertexes.begin(), copy.vertexes.end(), vertex2) != copy.vertexes.end()
                        && std::find(copy.second_e.begin(), copy.second_e.end(), vertex1) != copy.second_e.end()) {
                std::swap(vertex1, vertex2);
                copy.second_e.erase(std::find(copy.second_e.begin(), copy.second_e.end(), vertex2));
                bool found_uncloseable = false;
                for (int first_n: copy.second_e) {
                    std::unordered_set<int> comps;
                    for (int i: this->edges[first_n]) {
                        if (copy.dsu[i] != -1) {
                            comps.insert(find(copy.dsu, i));
                        }
                    }
                    for (int second_n: copy.first_e) {
                        if (second_n != first_n && !this->matrix[second_n][first_n]) {
                            bool good = true;
                            for (int i: this->edges[second_n]) {
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
                    copy.avoidable = false;
                    this->bad_copies -= 1;
                } else if (!found_uncloseable && !copy.avoidable) {
                    copy.avoidable = true;
                    this->bad_copies += 1;
                }
            } else if (copy.vertexes[0] != copy.vertexes[second_root_number]
            && std::find(copy.first_e.begin(), copy.first_e.end(), vertex1) != copy.first_e.end()
            && std::find(copy.second_e.begin(), copy.second_e.end(), vertex2) != copy.second_e.end()) {
                bool found_uncloseable = false;
                for (int first_n: copy.first_e) {
                    std::unordered_set<int> comps;
                    for (int i: this->edges[first_n]) {
                        if (copy.dsu[i] != -1) {
                            comps.insert(find(copy.dsu, i));
                        }
                    }
                    for (int second_n: copy.second_e) {
                        if (second_n != first_n && !this->matrix[second_n][first_n] && (vertex1 != first_n
                        || vertex2 != second_n)) {
                            bool good = true;
                            for (int i: this->edges[second_n]) {
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
                    copy.avoidable = false;
                    this->bad_copies -= 1;
                } else if (!found_uncloseable && !copy.avoidable) {
                    copy.avoidable = true;
                    this->bad_copies += 1;
                }
            } else if (copy.vertexes[0] != copy.vertexes[second_root_number] && std::find(copy.first_e.begin(), copy.first_e.end(), vertex2) != copy.first_e.end()
                       && std::find(copy.second_e.begin(), copy.second_e.end(), vertex1) != copy.second_e.end()) {
                std::swap(vertex2, vertex1);
                bool found_uncloseable = false;
                for (int first_n: copy.first_e) {
                    std::unordered_set<int> comps;
                    for (int i: this->edges[first_n]) {
                        if (copy.dsu[i] != -1) {
                            comps.insert(find(copy.dsu, i));
                        }
                    }
                    for (int second_n: copy.second_e) {
                        if (second_n != first_n && !this->matrix[second_n][first_n] && (vertex1 != first_n
                            || vertex2 != second_n)) {
                            bool good = true;
                            for (int i: this->edges[second_n]) {
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
                    copy.avoidable = false;
                    this->bad_copies -= 1;
                } else if (!found_uncloseable && !copy.avoidable) {
                    copy.avoidable = true;
                    this->bad_copies += 1;
                }
            } else if (copy.vertexes[0] == copy.vertexes[second_root_number]
            && std::find(copy.second_e.begin(), copy.second_e.end(), vertex1) != copy.second_e.end()
            && std::find(copy.second_e.begin(), copy.second_e.end(), vertex2) != copy.second_e.end()) {
                bool found_uncloseable = false;
                for (int first_n: copy.first_e) {
                    std::unordered_set<int> comps;
                    for (int i: this->edges[first_n]) {
                        if (copy.dsu[i] != -1) {
                            comps.insert(find(copy.dsu, i));
                        }
                    }
                    for (int second_n: copy.first_e) {
                        if (second_n != first_n && !this->matrix[second_n][first_n] && (vertex1 != first_n
                            || vertex2 != second_n) && (vertex2 != first_n || vertex1 != second_n)) {
                            bool good = true;
                            for (int i: this->edges[second_n]) {
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
                    copy.avoidable = false;
                    this->bad_copies -= 1;
                } else if (!found_uncloseable && !copy.avoidable) {
                    copy.avoidable = true;
                    this->bad_copies += 1;
                }
            }
        }
    }

    // обновляем граф
    edges[vertex1].push_back(vertex2);
    edges[vertex2].push_back(vertex1);
    matrix[vertex1][vertex2] = true;
    matrix[vertex2][vertex1] = true;

    // обрабатываем появляющиеся копии
    auto new_copies = occur_with_edge(*this, small, vertex1, vertex2);
    for (auto i: new_copies) {
        Copy copy;
        copy.avoidable = !is_copy_unavoidable(*this, i, second_root_number);
        if (copy.avoidable) {
            ++bad_copies;
        }
        copy.vertexes = i;
        copy.dsu = make_dsu(*this, i, second_root_number);
        for (int j: edges[i[0]]) {
            bool good = true;
            for (int vert: copy.vertexes) {
                if (vert != i[0] && (matrix[j][vert] || j == vert)) {
                    good = false;
                    break;
                }
            }
            if (good) {
                copy.first_e.push_back(j);
            }
        }
        copy.second_e = {};
        for (int j: edges[i[second_root_number]]) {
            bool good = true;
            for (int vert: copy.vertexes) {
                if (vert != i[second_root_number] && (matrix[j][vert] || j == vert)) {
                    good = false;
                    break;
                }
            }
            if (good) {
                copy.second_e.push_back(j);
            }
        }
        copies_and_comp.emplace_back(copy);
    }
}

int max_edge(const Graph& graph) {
    int res = 0;
    for (auto vec: graph.edges) {
        for (int i: vec) {
            res = std::max(res, i);
        }
    }
    if (res >= graph.edges.size()) {
        return res;
    } else {
        return 0;
    }
}
