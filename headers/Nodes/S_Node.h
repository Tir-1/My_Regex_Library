//
// Created by User on 19.04.2026.
//

#ifndef REGEXLIBRARY_S_NODE_H
#define REGEXLIBRARY_S_NODE_H
#include "Node.h"

class S_Node: public Node {
    char info;
    int pos;
    public:
    S_Node(const NodeType& type, const char& info) : Node(type), info(info) {nullable=false;}
    char get_info() const {
        return info;
    }
    void set_pos(int pos) {this->pos = pos;}

    bool in_Nullable()override{return false;}
    void make_FL() override {
        F.insert(pos);
        L.insert(pos);
    }
    void make_Follow(vector<set<int>> &f) override{}
    unique_ptr<Node> clone() override {
        auto cloned = std::make_unique<S_Node>(this->type, this->info);
        return cloned;
    }
};
#endif //REGEXLIBRARY_S_NODE_H