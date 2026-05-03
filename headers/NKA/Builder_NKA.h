//
// Created by User on 23.04.2026.
//

#ifndef REGEXLIBRARY_AST_NKA_H
#define REGEXLIBRARY_AST_NKA_H
#include "Automat_NKA.h"
#include "../Nodes/All_Nodes.h"
using std::pair;
class Builder_NKA {
    vector<NKA_State> states;
    vector<string> groups;
    int add_state() {
        states.emplace_back();
        return states.size() - 1;
    }
    int add_group(Group_Node* ptr_g) {
        auto name = ptr_g->get_name();
        if (name) {
            groups.push_back(name.value());
            return groups.size() - 1;
        }
       // else{groups.push_back(std::to_string(groups.size()));}
        return -1;
    }
    pair<int, int> build(Node* node);
    public:
        Builder_NKA(){groups.reserve(10);states.reserve(100);}
        Automat_NKA build_NKA(Node* root) {
            Automat_NKA automat;
            auto [start, end] = build(root);
            states[end].final = true;
            states.shrink_to_fit();
            groups.shrink_to_fit();
            automat.states = std::move(states);
            automat.groups = std::move(groups);
            automat.start_state = start;
            return automat;
        }
};
#endif //REGEXLIBRARY_AST_NKA_H