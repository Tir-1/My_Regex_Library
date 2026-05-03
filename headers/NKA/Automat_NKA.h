//
// Created by User on 22.04.2026.
//

#ifndef REGEXLIBRARY_AUTOMAT_NKA_H
#define REGEXLIBRARY_AUTOMAT_NKA_H
#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <string_view>
typedef std::pair<int, int> borders;
using std::vector, std::string;
struct Epsilon {
    int next_id{};
    int group_id=-1; // -1 если это не группа, иначе индекс группы
    bool start = true; //Для группы указывает начало ли это
};
struct NKA_State {
    std::unordered_map<char, std::vector<int>> next;
    std::vector<Epsilon> epsilon;
    bool final = false;
};

struct Way {
    int state_id;
    vector<int> groups_start;//ндекс = номер группы, число где началось
    vector<int> groups_end;
    bool operator<(const Way& S) const {
        if (state_id != S.state_id) return state_id < S.state_id;
        if (groups_start != S.groups_start)return groups_start < S.groups_start;
        return groups_end < S.groups_end;
    }
};

struct DTO {
    bool success=false;
    size_t end;
    vector<string> names_group;
    vector<borders> ranges; //начало и конец подстроки группы, индекс в векторе совпдадает с индексом группы
    //[start, end)
};

struct RES {
    bool success=false;
    size_t end;
    vector<int> groups_start;
    vector<int> groups_end;
};

struct  Automat_NKA {
    vector<string> groups;
    vector<NKA_State>states;
    int start_state;
    std::set<Way> end_set;
    RES result;

    [[nodiscard]] bool is_second_left(const vector<int>& start1, const vector<int>& end1, const vector<int>& start2, const vector<int>& end2) const;
    void select_epsilon(int id,  size_t j, std::set<Way>& next_state, vector<int>& start_groups, vector<int>& end_groups, vector<bool>& visited);
    DTO find_pref(std::string_view text, size_t i) ;
};
#endif //REGEXLIBRARY_AUTOMAT_NKA_H