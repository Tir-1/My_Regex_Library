//
// Created by User on 20.04.2026.
//

#include "../headers/AST/Lexer.h"
using std::make_unique;
optional<vector<unique_ptr<Node>>> Lexer::lex(const string &reg) {
    use_group = false;
    nodes.emplace_back(make_unique<Group_Node>(NodeType::OPEN, "", false)); // РВ в скобки, для алгоритма построения СД
    bool protect = false;
    bool repeat = false;
    int open = 0;
    string count;
    int i = 0;
    while (i < reg.size()) {
        char symbol = reg[i];
        unique_ptr<Node> node;

        if (repeat) {
            if (symbol == '}') {
                if (count.empty())return std::nullopt;
                int num = std::stoi(count);
                node = make_unique<Repeat_Node>(NodeType::REPEAT, num);
                nodes.push_back(std::move(node));
                repeat = false;
                i++;continue;
            }
            count.push_back(symbol);
            i++;continue;
        }
        if (protect) {
            if (symbol == '%') {
                if (i + 1 < reg.size() && reg[i+1] == '%') {
                    nodes.push_back(make_unique<S_Node>(NodeType::SYMBOL, '%'));
                    i += 2;
                    continue;
                }
                protect = false;
                i++;continue;
            }
            node = make_unique<S_Node>(NodeType::SYMBOL, symbol);
            nodes.push_back(std::move(node));
            i++;
            continue;
        }
        switch (symbol) {
            case '|':
                node = make_unique<Big_Node>(NodeType::OR);
                nodes.push_back(std::move(node));
                break;
            case '?':
                node = make_unique<Post_Node>(NodeType::OPTIONAL);
                nodes.push_back(std::move(node));
                break;
            case '.':
                if (reg.length() > i + 2  && reg[i + 1] == '.' && reg[i + 2] == '.') {
                    node = make_unique<Post_Node>(NodeType::CLINI);
                    i += 2;
                }
                else{node = make_unique<S_Node>(NodeType::SYMBOL, '.');}
                nodes.push_back(std::move(node));
                break;
            case '(':
                open += 1;
                if (reg.length() - i >= 2 && reg[i + 1] == '<') {
                    string name;
                    bool success = false;
                    i+=2;
                    for (; i < reg.length();i++) {
                        if (reg[i] == '>') {
                            success= true;
                            use_group = true;
                            node = make_unique<Group_Node>(NodeType::OPEN, name, true);
                            break;
                        }
                        name.push_back(reg[i]);
                    }
                    if (!success || name.empty()){return std::nullopt;}
                }
                else {
                    node = make_unique<Group_Node>(NodeType::OPEN, "NULL", false);
                }
                nodes.push_back(std::move(node));
                break;
            case ')':
                open -= 1;
                node = make_unique<Group_Node>(NodeType::CLOSE,"NULL", false);
                nodes.push_back(std::move(node));
                break;
            case '{':
                repeat = true;
                break;
            case '%':
                protect = true;
                if (i + 1 < reg.size() && reg[i+1] == '%') {
                    nodes.push_back(make_unique<S_Node>(NodeType::SYMBOL, '%'));
                    i += 2;
                    continue;
                }
                break;
            default:
                node = make_unique<S_Node>(NodeType::SYMBOL, symbol);
                nodes.push_back(std::move(node));

        }
        i++;
    }
    if (open != 0 || repeat) {return std::nullopt;}
    nodes.emplace_back( make_unique<Group_Node>(NodeType::CLOSE,"NULL", false));
    return std::move(nodes);
}