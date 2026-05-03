//
// Created by User on 20.04.2026.
//
#include "../headers/AST/Parser.h"
using std::make_unique;
optional<unique_ptr<Node>> Parser::parse() {
    auto it = tokens.begin();
    return build_recur(it);
}

optional<unique_ptr<Node>> Parser::build_recur(vector<unique_ptr<Node>>::iterator& it) {
    stack<unique_ptr<Node>> stack1;
    while (it != tokens.end()) {
        if ((*it)->get_type() == NodeType::OPEN) {
            auto group = std::move((*it));
            Group_Node* ptr = static_cast<Group_Node *>(group.get());
            it++;
            auto res = build_recur(it);
            if (res) {
                ptr->set_next( std::move(res.value()) );
                stack1.push(std::move(group));
                continue;
            }
            return std::nullopt;
        }
        if ((*it)->get_type() == NodeType::CLOSE) { // В стеке ctack лежат узлы в обратном порядке, верхний узел = последний
            it++;
            vector<unique_ptr<Node>> nodes;
            while (!stack1.empty()) {
                nodes.push_back(std::move(stack1.top()));
                stack1.pop();
            }
            std::reverse(nodes.begin(), nodes.end());

            vector<unique_ptr<Node>> parts; // Бдуем хранить конкатенации, которые были поделены OR, дополняя пустыми узлами
            auto cur_concat = make_unique<Big_Node>(NodeType::CONCAT);
            auto temp = static_cast<Big_Node*>(cur_concat.get());
            for (auto& node : nodes) {
                temp = static_cast<Big_Node*>(cur_concat.get());
                if (node->get_type() == NodeType::OR) {
                    if (temp->empty()) {
                        parts.push_back(make_unique<Empty_Node>());
                    }
                    else {
                        parts.push_back(std::move(cur_concat));
                    }
                    cur_concat = make_unique<Big_Node>(NodeType::CONCAT);
                }
                else {
                    temp->add_child(std::move(node));
                }
            }
            temp = static_cast<Big_Node*>(cur_concat.get());
            if (temp->empty()) {
                parts.push_back(make_unique<Empty_Node>());
            }
            else{parts.push_back(std::move(cur_concat));}
            if (parts.size() == 1) return std::move(parts[0]);
            unique_ptr<Node> many_or = make_unique<Big_Node>(NodeType::OR);
            auto ptr_many = static_cast<Big_Node*>(many_or.get());
            for (auto& node : parts) {
                ptr_many->add_child(std::move(node));
            }
            return std::move(many_or);
        }
        if ((*it)->get_type() == NodeType::OPTIONAL || (*it)->get_type() == NodeType::CLINI) {
            if (stack1.empty())return std::nullopt;
            unique_ptr<Node> post_node = std::move(*it);
            auto post = static_cast<Post_Node*>(post_node.get());
            auto n = std::move(stack1.top());
            if (n->get_type() == NodeType::OR)return std::nullopt;
            post->set_operand(std::move(n));
            stack1.pop();
            stack1.emplace(std::move(post_node));
            ++it;continue;
        }
        if ((*it)->get_type() == NodeType::REPEAT) {
            if (stack1.empty())return std::nullopt;
            auto operand = std::move(stack1.top());
            stack1.pop();
            Repeat_Node* ptr = static_cast<Repeat_Node *>(it->get());
            int count = ptr->get_count();
            if (count == 0) {
                stack1.emplace(make_unique<Empty_Node>());
            }
            else {
                auto concat = make_unique<Big_Node>(NodeType::CONCAT);
                for (size_t i = 0; i < count; i++) {
                    concat->add_child(operand->clone());
                }
                stack1.emplace(std::move(concat));
            }
            ++it;continue;
        }
        stack1.push(std::move(*it));
        ++it;
    }
    if (stack1.empty()) return (std::nullopt);
    return std::move(stack1.top());
}

