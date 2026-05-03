#include  "../headers/DKA/Builder_DKA.h"
using std::make_unique;
void Builder_DKA::set_positions(Node* node) {
    if (!node)return;
    NodeType t = node->get_type();
    if (t == NodeType::SYMBOL || t == NodeType::END) {
        S_Node* n = static_cast<S_Node*>(node);
        n->set_pos(cur_pos);
        positions[cur_pos] = n->get_info();
        cur_pos++;
    }
    else if (Big_Node* b = dynamic_cast<Big_Node*>(node)) {
        for (auto& child : b->get_children()) {
            set_positions(child.get());
        }
    }
    else if (Group_Node* g = dynamic_cast<Group_Node*>(node)) {
        set_positions(g->get_child().get());
    }
    else if (Post_Node* p = dynamic_cast<Post_Node *>(node)) {
        set_positions(p->get_operand());
    }
}


Automat_DKA Builder_DKA::make_DKA(unique_ptr<Node> root) {
    /*char marker_any = '\1';
    auto any = make_unique<S_Node>("ANY", marker_any);
    auto clini = make_unique<Post_Node>("CLINI");
    clini->set_operand(move(any));*/
    alphabet.clear();
    auto new_root = make_unique<Big_Node>(NodeType::CONCAT);
   // new_root->add_child(move(clini));
    new_root->add_child(std::move(root));
    new_root->add_child(make_unique<S_Node>(NodeType::END, '#'));
    set_positions(new_root.get());
    new_root->in_Nullable();
    new_root->make_FL();
    Follow_Pos.resize(cur_pos);
    new_root->make_Follow(Follow_Pos);

    set<int> start_state = new_root->get_F();
    Q.push(start_state);
    int global_state = 0;
    remember[start_state] = global_state++;
    while (!Q.empty()) {
        set<int> S = Q.front();
        Q.pop();
        int cur_state = remember[S];
        for (auto a: S) {
            char c = positions[a];
            alphabet.insert(c);
        }
        alphabet.erase('#');
        auto dfa_s = State{cur_state};
        if (S.find(cur_pos - 1) != S.end()) {dfa_s.final = true;}// Принимающие состояние, такое где есть #. # Стоит в конце строки, след его индекс это cur_pos - 1
        std::unordered_map<char, int> perexod;
        for (auto a : alphabet) { // для каждой буквы из алфавита смотрим в какие состояния она ведёт, объединение этих состояний = новое состояние ДКА
            set<int> union_FP;
            for (auto pos : S) {
                if (positions[pos] == a)union_FP.insert(Follow_Pos[pos].begin(), Follow_Pos[pos].end());
            }
            if (union_FP.empty() == false) {
                if (remember.find(union_FP) == remember.end()) {
                    Q.push(union_FP);
                    remember[union_FP] = global_state;
                    global_state++;
                }
                perexod[a] = remember[union_FP];
            }
        }

        dfa_s.next = perexod;
        DKA.states.push_back(dfa_s);
    }
    return DKA;
}

