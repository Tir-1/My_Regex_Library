//
// Created by User on 20.04.2026.
//

#ifndef REGEXLIBRARY_Lexer_H
#define REGEXLIBRARY_Lexer_H
#include <memory>
#include <vector>

#include "../Interfaces/I_Lexer.h"
#include "../Nodes/All_Nodes.h"


class Lexer: public I_Lexer{
    vector<unique_ptr<Node>> nodes;
    bool use_group = false;
    public:
        Lexer()=default;
        bool get_use_group() override{return use_group;};
        optional<vector<unique_ptr<Node>>> lex(const string &reg)override;
};
#endif //REGEXLIBRARY_PARSER_H