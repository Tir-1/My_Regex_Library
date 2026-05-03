//
// Created by User on 20.04.2026.
//

#ifndef REGEXLIBRARY_BIG_NODE_H
#define REGEXLIBRARY_BIG_NODE_H
#include "Node.h"
class Big_Node: public Node {
    vector<unique_ptr<Node>> children;
    public:
        Big_Node(const NodeType& type) :Node(type) {}
        vector<unique_ptr<Node>>& get_children() {
            return this->children;
        }
        void add_child(unique_ptr<Node> child) {
            children.push_back(std::move(child));
        }
    bool empty(){return children.empty();}
    bool in_Nullable() override{
            nullable = false;
            if (type == NodeType::OR) {
                    for (auto& child : children) {
                        if (child->in_Nullable())nullable = true;
                    }
                return nullable;
            }
            if (type == NodeType::CONCAT) {
                nullable = true;
                for (auto& child : children) {
                    if (child->in_Nullable() == false){nullable = false;}
                }
                return nullable;
            }
            return nullable;
        }
    void make_FL() override {
            if (type == NodeType::OR) {
                for (auto& child : children) {
                    child->make_FL();
                    auto temp = child->get_F();
                    F.insert(temp.begin(), temp.end());
                    temp = child->get_L();
                    L.insert(temp.begin(), temp.end());
                }
            }
            if (type == NodeType::CONCAT) {
                for (auto& child : children) {
                    child->make_FL();
                }
                for (size_t i =0; i < children.size(); i++) {
                    auto& child_F = children[i]->get_F();
                    F.insert(child_F.begin(), child_F.end());
                    if (children[i]->in_Nullable() == false)break;
                }
                for (int i =children.size() - 1; i >= 0; i--) {
                    auto& child_L = children[i]->get_L();
                    L.insert(child_L.begin(), child_L.end());
                    if (children[i]->in_Nullable() == false)break;
                }
            }
        }
    void make_Follow(vector<set<int>>& Follow_Pos) override {
            for (auto& child : children) {
                child->make_Follow(Follow_Pos);
            }
            if (type == NodeType::CONCAT) {
                //Перебираем пары, F второго добавляем всем из L первого, Есои в паре есть nullable нужно взять след
                for (size_t i =0; i < children.size() - 1; i++) {
                    auto& child_L = children[i]->get_L();
                    for (size_t j = i + 1; j < children.size(); j++) {
                        auto& child_F = children[j]->get_F();
                        for (auto k : child_L) {
                            Follow_Pos[k].insert(child_F.begin(), child_F.end());
                        }
                        if (children[j]->get_nullable() == false)break;

                    }
                }
            }
        }
    unique_ptr<Node> clone() override {
            auto cloned = std::make_unique<Big_Node>(this->type);
            for (auto& child : children) {
                cloned->children.push_back(child->clone());
            }
            return cloned;
        }
};
#endif //REGEXLIBRARY_BIG_NODE_H