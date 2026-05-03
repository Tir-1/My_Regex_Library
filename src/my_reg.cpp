#include "../headers/my_reg.h"
#include <memory>
using std::make_unique;
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


struct Matrix {
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
            data[i][i] = "eps";
        }
    }
    void append(size_t i, size_t j, const char& c) {
        if (i >= n || j >= n){correct = false;return;}
        auto& el = data[i][j];
        if (!el.empty()) el+='|';
        el+=c;
    }
    string concat(string a, string b) {
        if (a.empty() || b.empty()) return "";
        if (a == "eps")return b;
        if (b == "eps")return a;
        if (a.find('|') != string::npos) a = "(" + a + ")";
        if (b.find('|') != string::npos) b = "(" + b + ")";
        return a + b;
    }
    string func_or(string a, string b) {
        if (a.empty()) return b;
        if (b.empty()) return a;
        if (a == b) return a;
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
        string Rkk;
        if (kk != "eps" && !kk.empty()) {
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
            if (way.empty())continue;
            if (reg.empty())reg = way;
            else reg = "(" + reg + ")|(" + way + ")";
        }
    }
    return reg;

}