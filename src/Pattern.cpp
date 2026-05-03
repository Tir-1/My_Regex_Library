#include "../headers/Pattern.h"

Output_reg Pattern::search(std::string_view text) {
    Output_reg out;
    for (size_t i = 0; i <= text.size(); i++) {
        if (use_groups) {
            DTO result = NKA.find_pref(text, i);
            if (result.success && (result.end > i || i == text.size())) { //Вторая проверка, чтобы при замыкании Клини он просто не записывал пустоту как ответ
                out.set_res(string(text.substr(i, result.end - i))); //Второй аргумент ДЛИНА!!!!!
                auto& names = result.names_group;
                auto& ranges = result.ranges;
                for (size_t j = 0; j < names.size(); j++ ) {
                    const int& first = ranges[j].first;
                    string group_text;
                    if (first != -1)group_text = text.substr(first, ranges[j].second - first);
                    out.append_to(group_text, names[j]);
                }
                break;
            }
        }
        else {
            DTO_DKA result = DKA.find_pref(text, i);
            if (result.success && (result.end > i || i == text.size())) {
                out.set_res(string(text.substr(i, result.end - i)));
                break;
            }
        }
    }
    return out;
}