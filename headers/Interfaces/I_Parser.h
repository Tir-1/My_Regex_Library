//
// Created by User on 21.04.2026.
//

#ifndef REGEXLIBRARY_I_PARSER_H
#define REGEXLIBRARY_I_PARSER_H
#include "../Nodes/Node.h"
class I_Parser {
    public:
    virtual ~I_Parser()=default;
    virtual void set_tokens(vector<unique_ptr<Node>> tokens)=0;
    virtual bool check_tokens()=0;
    virtual optional<unique_ptr<Node>> parse()=0;
};
#endif //REGEXLIBRARY_I_PARSER_H