//
// Created by User on 20.04.2026.
//

#ifndef REGEXLIBRARY_REPEAT_NODE_H
#define REGEXLIBRARY_REPEAT_NODE_H
#include "Post_Node.h"

class Repeat_Node: public Post_Node{
    int count;
    public:
    Repeat_Node(const NodeType& type, int count) : Post_Node(type), count(count){}
    bool in_Nullable() override{
        if (count == 0){nullable=true;return true;}
        nullable = operand->in_Nullable();
        return nullable;
    }
    unique_ptr<Node> get_operand(){return move(operand);};
    int get_count(){return count;}
    void make_FL() override {
        operand->make_FL();
        F = operand->get_F();
        L = operand->get_L();
    }
    unique_ptr<Node> clone() override {
        auto cloned = std::make_unique<Repeat_Node>(this->type, this->count);
        if (operand)cloned->set_operand(this->operand->clone());
        return cloned;
    }
};
#endif //REGEXLIBRARY_REPEAT_NODE_H