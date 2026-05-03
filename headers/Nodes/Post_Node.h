//
// Created by User on 20.04.2026.
//

#ifndef REGEXLIBRARY_POST_NODE_H
#define REGEXLIBRARY_POST_NODE_H
#include "Node.h"
class Post_Node: public Node {
    protected:
        unique_ptr<Node> operand;
    public:
    Post_Node(const NodeType& type) : Node(type){}
        void set_operand(unique_ptr<Node> operand) {
            this->operand = std::move(operand);
        }
        Node* get_operand() {
            return operand.get();
        }
    bool in_Nullable() {
        if (type == NodeType::OPTIONAL || type == NodeType::CLINI) {
            nullable=true;
            operand->in_Nullable();
            return  true;
        }
        nullable = false;
        return false;
    }
    void make_FL() {
        operand->make_FL();
        F = operand->get_F();
        L = operand->get_L();
    }
 void make_Follow(vector<set<int>>& Follow_Pos) {
        if (type == NodeType::REPEAT || type == NodeType::CLINI) {//Цикл, из последней поз может перепрыгнуть в первую
            auto child_L = operand->get_L();
            auto child_F = operand->get_F();
            for (int i:child_L) {
                Follow_Pos[i].insert(child_F.begin(), child_F.end());
            }
        }
        operand->make_Follow(Follow_Pos);
    }
    unique_ptr<Node> clone(){
        auto cloned = std::make_unique<Post_Node>(this->get_type());
        cloned->operand = operand->clone();
        return cloned;
    }
};
#endif //REGEXLIBRARY_POST_NODE_H