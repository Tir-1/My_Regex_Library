#include  "../headers/NKA/Builder_NKA.h"
pair<int, int> Builder_NKA::build(Node *node) {
    {
        NodeType type = node->get_type();
        if (type == NodeType::SYMBOL) {
            S_Node* ptr_s = static_cast<S_Node*>(node);
            int first = add_state();
            int second = add_state();
            char c = ptr_s->get_info();
            states[first].next[c].push_back(second);
            return pair<int, int>(first, second);
        }
        if (type == NodeType::CONCAT || type == NodeType::OR) {
            Big_Node* ptr_b = static_cast<Big_Node*>(node);
            int start_b = add_state();
            int end_b = add_state();
            vector<pair<int, int>> parts;
            for (auto& children : ptr_b->get_children()) {
                auto res = build(children.get());
                parts.push_back(res);
            }
            if (type == NodeType::CONCAT) {
                auto e = Epsilon{parts[0].first, -1};
                states[start_b].epsilon.push_back(e);
                size_t i = 1;
                for (; i < parts.size(); i++) {
                    e = Epsilon{parts[i].first, -1};
                    states[parts[i-1].second].epsilon.push_back(e);
                }
                e = Epsilon{end_b, -1};
                states[parts[i-1].second].epsilon.push_back(e);
                return {start_b, end_b};
            }
            if (type == NodeType::OR) {
                for (auto part: parts) {
                    auto e = Epsilon{part.first, -1};
                    states[start_b].epsilon.push_back(e);
                    e = Epsilon{end_b, -1};
                    states[part.second].epsilon.push_back(e);
                }
                return {start_b, end_b};
            }
        }
        if (type == NodeType::CLINI || type == NodeType::OPTIONAL) {
            Post_Node* ptr_p = static_cast<Post_Node*>(node);
            auto [first, second] = build(ptr_p->get_operand());
            int start = add_state();
            int end = add_state();

            auto e = Epsilon{first, -1};
            states[start].epsilon.push_back(e);
            e = Epsilon{end, -1}; // Переход сразу в конец, для отсутсвия
            states[start].epsilon.push_back(e);
            if (type == NodeType::CLINI) { // Нужно ещё добавить цикл
                e = Epsilon{start, -1};
                states[second].epsilon.push_back(e);
            }
            e = Epsilon{end, -1};
            states[second].epsilon.push_back(e);
            return {start, end};
        }
        if (type == NodeType::OPEN) {
            Group_Node* ptr_g = static_cast<Group_Node*>(node);
            int group_id = add_group(ptr_g);
            auto [group_s, group_e] = build(ptr_g->get_child().get());
            if (group_id == -1)return {group_s, group_e};
            int start = add_state();
            int end = add_state();
            auto e = Epsilon{group_s, group_id, true};
            states[start].epsilon.push_back(e);
            e = Epsilon{end, group_id, false};
            states[group_e].epsilon.push_back(e);
            return {start, end};
        }
        int start = add_state();
        int end = add_state();
        auto e = Epsilon{end, -1};
        states[start].epsilon.push_back(e);
        return {start, end};
    }
}
