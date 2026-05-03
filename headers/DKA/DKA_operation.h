//
// Created by User on 30.04.2026.
//

#ifndef REGEXLIBRARY_DKA_OPERATION_H
#define REGEXLIBRARY_DKA_OPERATION_H

#include "Automat_DKA.h"
#include <map>
#include <queue>
using std::queue, std::map, std::pair;
inline bool final(const Automat_DKA& dka1, const Automat_DKA& dka2) {
}
inline Automat_DKA BFS_intersection(const Automat_DKA& dka1, const Automat_DKA& dka2) {
    Automat_DKA result{};
    if (dka1.states.empty() || dka2.states.empty())return result;
    queue<pair<int, int>> states;
    states.push({dka1.start_state, dka2.start_state});
    std::map<std::pair<int, int>, int> new_index;
    new_index[{dka1.start_state, dka2.start_state}] = 0;
    result.states.emplace_back(State{0, {}, dka1.states[0].final && dka2.states[0].final});



}
#endif //REGEXLIBRARY_DKA_OPERATION_H