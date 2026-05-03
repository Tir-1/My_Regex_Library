#include "../headers/NKA/Automat_NKA.h"

using std::vector, std::set;
[[nodiscard]] bool Automat_NKA::is_second_left(const vector<int>& start1, const vector<int>& end1, const vector<int>& start2, const vector<int>& end2) const {
     if (start1.empty())return true;
     if (groups.empty())return false; // Если групп в целом нет, то и менять не нужно
     for (size_t i = 0; i < groups.size(); i++) {
         if (start1[i] == -1 && start2[i] == -1)continue;
         if (start1[i] != start2[i]) {
             if (start2[i] != -1 && start2[i] < start1[i])return true;
             if (start2[i] != -1 && start1[i] == -1)return true;

             return false;
         }
         if (end1[i] != end2[i]) {
             if (end2[i] > end1[i])return true;
             return false;
         }
     }

     //Вообще всё совпало
     return false;
 }


void Automat_NKA::select_epsilon(int id,  size_t j, set<Way>& next_state, vector<int>& start_groups, vector<int>& end_groups, vector<bool>& visited){
    if (visited[id])return;
    visited[id] = true;

    auto& s = states[id];
    if (s.next.empty() == false || s.final) {
        next_state.insert(Way{id, start_groups, end_groups});
        if (s.final) {
            if (!result.success || result.end < j || (result.end == j && is_second_left(result.groups_start, result.groups_end, start_groups, end_groups))) {
                result.success = true;
                result.end = j;
                result.groups_start = start_groups;
                result.groups_end = end_groups;
            }
        }
    }
    for (auto& eps : states[id].epsilon) {
        int old = -1;
        bool change = false;
        if (eps.group_id != -1) {
            change = true;
            if (eps.start) {
                old = start_groups[eps.group_id];
                start_groups[eps.group_id] = j;
            }
            else {
                old = end_groups[eps.group_id];
                end_groups[eps.group_id] = j;
            }
        }
        select_epsilon(eps.next_id, j, next_state ,start_groups, end_groups, visited);
        if (change) { // возвращаем значение, чтобы из рекурсии пришёл тот же вектор и можно было отдать его в другую рекурсию
            if (eps.start)start_groups[eps.group_id] = old;
            else end_groups[eps.group_id] = old;
        }

    }

}

DTO Automat_NKA::find_pref(std::string_view text, size_t i) {
    result.end = -1;
    result.success = false;
    result.groups_end.assign(groups.size(), -1);
    result.groups_start.assign(groups.size(), -1);
    set<Way> cur_state;
    set<Way> next_state;
    vector<bool> visited(states.size(), false);
    vector<int> start_groups(groups.size(), -1);
    vector<int> end_groups(groups.size(), -1);
    select_epsilon(start_state, i, cur_state, start_groups, end_groups, visited);
    // cur_state.insert(Way{start_state, start_groups, end_groups});
    for (size_t j = i; j < text.size(); ++j) {
        for (auto& el : cur_state) {
            auto perexod = states[el.state_id].next;
            char c = text[j];
            if (perexod.find(c) == perexod.end())continue;
            auto vec = perexod[c];
            for (auto& s : vec) {
                auto copy_start = el.groups_start;
                auto copy_end = el.groups_end;
                std::fill(visited.begin(), visited.end(), false);
                select_epsilon(s, j + 1, next_state, copy_start, copy_end, visited);
            }
        }
        if (next_state.empty())break;
        cur_state = next_state;
        next_state.clear();
    }
    DTO dto{};
    if (result.success) {
        dto.success = true;
        dto.end = result.end;
        dto.names_group = groups;
        dto.ranges.resize(groups.size(), {-1, -1});
        for (size_t k = 0; k < groups.size(); ++k) {
            auto start = result.groups_start[k];
            auto end = result.groups_end[k];
            if (start != -1 && end != -1) {
                dto.ranges[k] = {start, end};
            }
        }
    }
    return dto;
}