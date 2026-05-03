//
// Created by User on 20.04.2026.
//

#ifndef REGEXLIBRARY_GROUP_NODE_H
#define REGEXLIBRARY_GROUP_NODE_H
#include "Node.h"
#include <optional>
class Group_Node: public Node {
    string name;
    unique_ptr<Node> next;
    bool have_name;
    public:
    Group_Node(const NodeType& type, string name, bool flag) : Node(type), name(std::move(name)), next(nullptr), have_name(flag) {};
    [[nodiscard]] std::optional<string> get_name() const {
        if (have_name)return name;
        return std::nullopt;
    }
    unique_ptr<Node>& get_child() {
        return next;
    }
    void set_next(unique_ptr<Node> next) {
        this->next = std::move(next);
    }
    bool in_Nullable()override {
        nullable=next->in_Nullable();
        return nullable;
    }
    void make_FL() override {
        next->make_FL();
        L = next->get_L();
        F = next->get_F();
    }
    void make_Follow(vector<set<int>> & f) override{next->make_Follow(f);}
unique_ptr<Node> clone() override {
        auto cloned = std::make_unique<Group_Node>(this->type, this->name, this->have_name);
        cloned->next = this->next->clone();
        return cloned;
    }
};
#endif //REGEXLIBRARY_GROUP_NODE_H