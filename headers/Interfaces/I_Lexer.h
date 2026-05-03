//
// Created by User on 21.04.2026.
//

#ifndef REGEXLIBRARY_I_LEXER_H
#define REGEXLIBRARY_I_LEXER_H
#include "../Nodes/Node.h"
class I_Lexer {
public:
    virtual optional<vector<unique_ptr<Node>>> lex(const string &reg)=0;
    virtual bool get_use_group()=0;
    virtual ~I_Lexer() = default;
};
#endif //REGEXLIBRARY_I_LEXER_H