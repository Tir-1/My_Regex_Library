
#include "../headers/DKA/Automat_DKA.h"
#include "../headers/DKA/min_dka.h"
#include "../headers/my_reg.h"
#include <map>
#include <stack>
#include <unordered_set>
using std::stack, std::map, std::pair;
//Вместо Декартового произведения ищем только пары, достижимые из обоих состояний
Automat_DKA intersect(const Automat_DKA& first, const Automat_DKA& second) {
    Automat_DKA result;
    if (first.states.empty() || second.states.empty())return result;
    set<char> alphabet = first.alphabet;
    alphabet.insert(second.alphabet.begin(), second.alphabet.end());
    stack<pair<int, int>> s;
    map<pair<int, int>, int> new_id;

    pair p = {first.start_state, second.start_state};
    s.push(p);
    new_id[p] = 0;
    result.start_state = 0;
    int id = 1;
    result.states.emplace_back();
    while (s.empty() == false) {
        auto [id1, id2] = s.top();s.pop();
        p = {id1, id2};
        //if (new_id.count(p))continue;
        size_t add_el = 0;
        State& state = result.states[new_id[p]];
       // new_id[{id1, id2}] = id;
        state.id = new_id[p];
        state.final = first.states[id1].final && second.states[id2].final;
        for (const auto& [c, next1] : first.states[id1].next) {
            if (second.states[id2].next.contains(c)) {
                //alphabet.insert(c);
                p = {next1, second.states[id2].next.at(c)};
                if (new_id.contains(p) == false) {
                    new_id[p] = id;
                    s.push(p);
                    state.next[c] = id++;
                    add_el++;
                }
                else  state.next[c] = new_id[p];
            }
        }
        //emplace_back после всех действия с state, иначе может инвалидироваться ссылка
        while (add_el != 0) {
            result.states.emplace_back();//расширяем вектор states, чтобы можно было записать по индексу
            add_el--;
        }
    }
    result.alphabet = alphabet;
    result = min_dka(result);
    return result;
}
void remove_dead_states(Automat_DKA& dka) {
    if (dka.states.empty())return;
    set<int> good_states;
    for (const auto& state : dka.states) {
        if (state.final) good_states.insert(state.id);
    }

    //Добавляем все, что ведут в финальные
    bool changed = true;
    while (changed) {
        changed = false;
        for (const auto& state : dka.states) {
            if (good_states.contains(state.id))continue;
            for (const auto& [c, next] : state.next) {
                if (good_states.contains(next)) {
                    good_states.insert(state.id);
                    changed = true;break;
                }
            }
        }
    }
    if (good_states.size() == dka.states.size())return;
    if (good_states.contains(dka.start_state) == false) {
        dka = get_null_dka();
        return;
    }
    vector<State> new_states;
    unordered_map<int, int> new_id;
    int id = 0;
    for (const auto& state : dka.states) {
        if (good_states.contains(state.id)) {
            new_id[state.id] = id;
            new_states.push_back(state);
            new_states.back().id = id;
            id++;
        }
    }
    for (auto& state : new_states) {
        unordered_map<char, int> new_next;
        for (const auto& [c, next_id] : state.next) {
            if (new_id.contains(next_id))new_next[c] = new_id[next_id];
        }
        state.next = new_next;
    }
    dka.states = std::move(new_states);
    dka.start_state = new_id[dka.start_state];
}
//Принимает строки первого, но не второго. Ищем пары, достижимые хотя бы из первого состояния.
Automat_DKA difference(const Automat_DKA& first, const Automat_DKA& second) {
    Automat_DKA result;
    if (first.states.empty())return result;
    if (second.states.empty())return first;
    set<char> alphabet = first.alphabet;
    alphabet.insert(second.alphabet.begin(), second.alphabet.end());
    stack<pair<int, int>> s;
    map<pair<int, int>, int> new_id;

    pair p = {first.start_state, second.start_state};
    s.push(p);
    new_id[p] = 0;
    result.start_state = 0;
    int id = 1;
    result.states.emplace_back();
    while (s.empty() == false) {
        auto [id1, id2] = s.top();s.pop();
        p = {id1, id2};
        size_t add_el = 0;
        State& state = result.states[new_id[p]];
        state.id = new_id[p];
        state.final = false;
        if (first.states[id1].final) {
            if (id2 != -1 && (second.states[id2].final == false))state.final = true;
            if (id2 == -1)state.final = true;
        }
        for (const auto& [c, next1] : first.states[id1].next) {
            int new_id2 = -1;
            if (id2 != -1 && second.states[id2].next.contains(c)) {
                new_id2 = second.states[id2].next.at(c);
            }
            p = {next1, new_id2};
            //alphabet.insert(c);
            if (new_id.contains(p) == false) {
                new_id[p] = id;
                s.push(p);
                add_el++;
                state.next[c] = id++;
            }
            else  state.next[c] = new_id[p];
        }
        while (add_el != 0) {
            result.states.emplace_back();
            add_el--;
        }

    }
    result.alphabet = alphabet;
    remove_dead_states(result);
    return min_dka(result);
}