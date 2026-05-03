//
// Created by User on 21.04.2026.
//

#ifndef REGEXLIBRARY_AST_DKA_H
#define REGEXLIBRARY_AST_DKA_H
#include "../Nodes/All_Nodes.h"
#include <map>
#include <queue>

#include "unordered_map"
using std::unordered_map;

#include "Automat_DKA.h"
class Builder_DKA {
    unordered_map<int, char> positions;
    int cur_pos = 0;
    vector<set<int>> Follow_Pos; // индекс это позиция, множества её следующие состояния
    Automat_DKA DKA;
    std::map<set<int>, int> remember; // Хранятся Множества состояний и его id
    std::queue<set<int>> Q;
    set<char> alphabet;
    void set_positions(Node* node);
public:
    Builder_DKA()=default;
    Automat_DKA make_DKA(unique_ptr<Node> root);
    set<char> get_alphabet(){return alphabet;};
};



#endif //REGEXLIBRARY_AST_DKA_H