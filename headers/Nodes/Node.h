#ifndef NODE_H
#define NODE_H

/*
 *Group_node открывающая строка, возможно с именем
*ПРосто класс Node:
 *Close_Node = Закрыв скобка
 *S_Node = буква
 *Класс Big_Node:
 * OR_Node = Или
 * Conc_Node = конкатенация
 * Класс Post_Node:
 *Repeat_Node = Повтор
 *Clini_Node = ноль или более раз = 8
 *Opt_Node = ноль или 1 =?
 *
 *
 */

enum class NodeType {
    SYMBOL=0,
    OR=1,
    CONCAT=2,
    CLINI=3,
    OPTIONAL=4,
    GROUP=5,
    REPEAT=6,
    END=7,
    EMPTY=8,
    OPEN=9,
    CLOSE=10,
};
#include <string>
#include <set>
#include <vector>
#include <memory>
#include <optional>
using std::vector, std::optional;
using std::string, std::set,std::unique_ptr;
class Node {
    protected:
        NodeType type;
        set<int> F;
        set<int> L;
        bool nullable = false;
    public:
        Node(const NodeType& type) {
            this->type = type;
        }
    virtual std::unique_ptr<Node> clone()=0;
    set<int>& get_F() {return F;}
    set<int>& get_L() {return L;}
    virtual void make_FL()=0;
    virtual void make_Follow(vector<set<int>>&)=0;
        //void add_F(int el){F.insert(el);}
      //  void add_L(int el){L.insert(el);}
        virtual ~Node() {}
        virtual bool in_Nullable() = 0;
    bool get_nullable() {return nullable;}
        NodeType get_type() {return type;}
};


#endif