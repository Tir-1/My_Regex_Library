//
// Created by User on 20.04.2026.
//

#ifndef REGEXLIBRARY_EMPTY_NODE_H
#define REGEXLIBRARY_EMPTY_NODE_H
#include "Node.h"
class Empty_Node: public Node {
    public:
    Empty_Node(): Node(NodeType::EMPTY) {nullable=true;}
    bool in_Nullable()override{return true;}
    void make_FL() override{return;};
    void make_Follow(vector<set<int>> &f) override{}
    unique_ptr<Node> clone() override {
        return std::make_unique<Empty_Node>();
    }
};
#endif //REGEXLIBRARY_EMPTY_NODE_H