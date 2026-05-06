#include "../headers/my_reg.h"
#include <memory>
using std::make_unique, std::queue, std::unordered_map;
Output_reg search(const string& reg, const string& text) {
    Reg_Handler handler(reg, make_unique<Lexer>(), make_unique<Parser>());
    auto pattern= handler.work();
    if (pattern == std::nullopt)throw std::invalid_argument("WRONG regular");
    Output_reg result = pattern.value().search(text);
    return result;
}
Pattern compile(const string& reg) {
    Reg_Handler handler(reg, make_unique<Lexer>(), make_unique<Parser>());
    auto pattern= handler.work();
    if (pattern == std::nullopt)throw std::invalid_argument("WRONG regular");
    return pattern.value();
}

Automat_DKA get_null_dka() {
    Automat_DKA dka;
    State s{0, {}, false};
    dka.start_state = 0;
    dka.states.push_back(s);
    return dka;
}

struct Matrix { // \1 Тут означает epsilon, а пустая строка отсутсвие пути
private:
    vector<vector<string>> data;
    size_t n;
    bool correct=true;
public:
    explicit Matrix(size_t n){
        data.resize(n);
        this->n = n;
        for (auto& vec : data) {
            vec.resize(n);
        }
    }
    void init() {
        for (size_t i = 0; i < data.size(); ++i) {
            data[i][i] = "\1";
        }
    }
    void append(size_t i, size_t j, const char& c) {
        if (i >= n || j >= n){correct = false;return;}
        data[i][j] = func_or(data[i][j], string(1, c));
    }
    string concat(string a, string b) {
        if (a.empty() || b.empty()) return "";
        if (a == "\1")return b;
        if (b == "\1")return a;
        if (a.find('|') != string::npos) a = "(" + a + ")";
        if (b.find('|') != string::npos) b = "(" + b + ")";
        return a + b;
    }
    string func_or(string a, string b) {
        if (a.empty()) return b;
        if (b.empty()) return a;
        if (a == b) return a;
        if (a == "\1")return "(" + b +")?";
        if (b == "\1")return "(" + a +")?";
        return a + "|" + b;
    }
    void update(size_t i, size_t j, size_t k, const Matrix& m) {
        if (i >= n || j >= n || k >= n){correct = false;return;}
        string ij =m.data[i][j];
        string ik =m.data[i][k];
        string kk =m.data[k][k];
        string kj = m.data[k][j];
        if (ik.empty() || kj.empty()) { //пути нет
            data[i][j] = ij;return;
        }
        string Rkk = "\1";
        if (kk != "\1" && !kk.empty()) {
            Rkk = "(" + kk +")..."; // цикл из k в k
        }
        string Rij = concat(concat(ik, Rkk),kj);
        if (ij.empty()){data[i][j] = Rij;return;}
        data[i][j] = func_or(ij, Rij);
    }
    string get(size_t i, size_t j) {
        if (i >= n || j >= n) {correct = false;return "";}
        return data[i][j];
    }
    [[nodiscard]] bool get_correct() const {return correct;}
};


string recover_reg(const Automat_DKA& automat) {
    auto& states = automat.states;
    size_t n = states.size();
    Matrix matrix(n);
    matrix.init();
    for (size_t i = 0; i <n; i++) {
        auto& next = states[i].next;
        for (auto& el : next) {
            matrix.append(i, el.second, el.first);
        }
    }
    for (size_t k =0; k < n; k++) {
        Matrix next_matrix(n);
        for (size_t i = 0; i < states.size(); i++) {
            for (size_t j = 0; j < states.size(); j++) {
                next_matrix.update(i, j, k, matrix);
            }
        }
        matrix = std::move(next_matrix);
    }
    if (!matrix.get_correct())return "";
    string reg;
    for (size_t i =0; i < n; i++) {
        if (states[i].final) {
            string way = matrix.get(automat.start_state, i);
            reg = matrix.func_or(reg, way);
        }
    }
    if (reg == "\1")return "";
    return reg;

}
//графы изоморфны = Можно переименовать вершины и получить два одинаковых
//Одноврменено обходим два графа
bool isomorphic(const Automat_DKA& first, const Automat_DKA& second) {
    if (first.states.empty() && second.states.empty()) return true;
    if (first.states.empty() || second.states.empty()) return false;
    if (first.states.size() != second.states.size())return false;
    queue<pair<int, int>> q;
    unordered_map<int, int> equ; // эквивалентные состояния
    q.push({first.start_state, second.start_state});
    equ[first.start_state] = second.start_state;
    while (!q.empty()) {
        auto [id1, id2] = q.front();q.pop();
        const State &s1 = first.states[id1];
        const State &s2 = second.states[id2];
        if (s1.next.size() != s2.next.size())return false;
        if (s1.final != s2.final) return false;
        for (const auto& [c, next1] : s1.next) {
            if (s2.next.contains(c) == false) {return false;}
            int next2 = s2.next.at(c);// next1 и next2 должны быть одинаковыми состояниями, потому что переход по одному символу
            if (equ.count(next1) == false) {
                equ[next1] = next2;
                q.push({next1, next2});
            }
            if (equ[next1] != next2) return false; //next1 уже сопоставлено с другим состоянием

        }

    }
    if (equ.size() != first.states.size())return false; // Записали меньше вершин
    return true;
}