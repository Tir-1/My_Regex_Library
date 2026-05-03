//
// Created by User on 20.04.2026.
//

#ifndef REGEXLIBRARY_PARSER_H
#define REGEXLIBRARY_PARSER_H

#include <algorithm>

#include "../Interfaces/I_Parser.h"
#include "../Nodes/All_Nodes.h"
#include "stack"
#include "list"
#include "set"
using std::stack, std::move;
class Parser: public I_Parser {
    vector<unique_ptr<Node>> tokens;
    bool have_tokens = false;
    optional<unique_ptr<Node>> build_recur(vector<unique_ptr<Node>>::iterator& it);
    vector<std::set<int>> follow_pos;
    public:
        Parser()=default;
        void set_tokens(vector<unique_ptr<Node>> _tokens)override{this->tokens = std::move(_tokens);have_tokens=true;}
        bool check_tokens()override{return have_tokens;}
        optional<unique_ptr<Node>> parse()override;

};

#endif //REGEXLIBRARY_PARSER_H