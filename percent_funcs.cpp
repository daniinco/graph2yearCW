#include "percent_funcs.h"
#include "search_occur.h"
#include "copy_functions.h"

std::pair<int, int> calc_copies_counts(Graph& graph, Graph& small, int second_root_number) {
    int bad_copies = 0;
    for (auto i: graph.copies_and_comp) {
        bad_copies += i.avoidable;
    }
    return {bad_copies, graph.copies_and_comp.size()};
}
