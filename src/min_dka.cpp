#include "../headers/DKA/min_dka.h"
using std::optional, std::vector, std::set, std::map;

struct stG {
    int id;
    set<State> elements;
    void insert(const State& S) {elements.insert(S);};
};

int find(const vector<stG>& P, int state_id) {
    for (auto& el : P) {
        for (auto& state: el.elements) {
            if (state.id == state_id)return el.id;
        }
    }
    return -1;
}

Automat_DKA min_dka(const Automat_DKA& DKA, const set<char>& alphabet) {
    Automat_DKA minDKA;
    stG G1{0};
    stG G2{1};
    vector<stG> P;
    for (const State& state : DKA.states) {
        if (state.final) {
            G1.insert(state);continue;
        }
        G2.insert(state);
    }
    if (G1.elements.empty() == false)P.push_back(G1);
    if (G2.elements.empty() == false)P.push_back(G2);
    while (true) {// крутимся пока P будет меняться
        bool success = true;
        int index = 0; // Чтобы id G совпадал с индексом в векторе
        vector<stG > new_P;
        for (auto& G : P) {
            stG new_G;
            // Нам нужно, чтобы состояния в одной G, по каждой букве шли в ОДИНАКОВУЮ G
            map<map<char, int>, set<State>> ways; //, Какие буквы к каким G ведут/ Состояния которые имеют такие переходы
            for (auto state : G.elements) {
                map<char, int> perexod; // буква и в какую G ведёт
                for (auto c : alphabet) {
                    int id = -1;
                    if (state.next.find(c) != state.next.end()) {
                        id = state.next[c];
                    }

                    int id_G = find(P, id);
                    perexod[c] = id_G;
                }
                //Нашли id группы, в которую ведеёт переход
                // Будем разделять состояния по их наборам переходов
                ways[perexod].insert(state);
            }
            if (ways.size() == 1) {
                stG copy = G;
                copy.id = index++;
                new_P.push_back(copy);
            }
            else {
                success = false; // Дробим P
                for (auto& way : ways) {
                    new_P.push_back(stG{index++, way.second});
                }
            }
        }
        if (success)break;
        P = new_P;
    }
    bool f_break = false;
    for (size_t i = 0; i < P.size(); i++) {
        for (auto& el : P[i].elements) {
            if (el.id == 0) {
                std::swap(P[0], P[i]); // стартовое состояние в начало
                P[i].id = i;
                P[0].id = 0;
                f_break = true;
                break;
            }
        }
        if (f_break)break;
    }
    // получили верное разбиение, каждое G в нём новое состояние ДКА
    int i = 0;
    for (auto& G : P) {
        State state = State{i++};
        auto G_state = *(G.elements.begin());
        state.final = G_state.final;
        for (char c : alphabet) {
            if (G_state.next.find(c) != G_state.next.end()) {
                int state_id = G_state.next[c]; // id Состояния куда переходит
                int id = find(P, state_id); // id G где Это состояние
                state.next[c] = id;
            }
        }
        minDKA.states.push_back(state);
    }

    return minDKA;

}