//
// Created by User on 22.04.2026.
//

#ifndef REGEXLIBRARY_AUTOMAT_H
#define REGEXLIBRARY_AUTOMAT_H
#include <unordered_map>
#include <vector>
#include <string>

#include <string_view>
using std::vector, std::string;
struct State {
    int id;
    std::unordered_map<char, int> next;
    bool final = false;
    bool operator<(const State& S) const {
        return id < S.id;
    }
};

struct DTO_DKA {
    bool success = false;
    size_t end = 0;
};
struct  Automat_DKA {
    vector<State> states;
    int start_state=0;

    DTO_DKA find_pref(std::string_view text, size_t start = 0)const  {
        DTO_DKA dto;
        if (states.empty())return dto;
        if (states[start_state].final) {
            dto.success = true;
            dto.end = start;
        }
        int end = -1;
        int state_id = start_state;
        for (size_t i = start; i < text.length(); i++) {
            char c = text[i];
            if (states[state_id].next.find(c) == states[state_id].next.end())break;
            state_id = states[state_id].next.at(c);
            if (states[state_id].final) {
                dto.success = true;
                dto.end = i+1;
            }
        }
        return dto;
    }
};

#endif //REGEXLIBRARY_AUTOMAT_H