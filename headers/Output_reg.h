//
// Created by User on 22.04.2026.
//

#ifndef REGEXLIBRARY_OUTPUT_REG_H
#define REGEXLIBRARY_OUTPUT_REG_H
#include <string>
#include <vector>
#include <unordered_map>
using std::string, std::vector;
class Output_reg {
    vector<string> result; // Вектор групп
    std::unordered_map<string, size_t> groups; //имя группы и её индекс в result
    bool success=false;
public:
    Output_reg(){result.emplace_back("");};
    string operator[](size_t index) {
        if (index >= result.size()) return "";
        return result[index];
    }
    string operator[](string index) {
        if (groups.find(index) == groups.end()) return "";
        return result[groups[index]];
    }
    auto begin() {return result.begin();}
    auto end() {return result.end();}
    void set_res(const string& str) {
        result[0] = str;
        success = true;
    }
    void append_to(const string& str, const string& group_name) {
        result.push_back(str);
        groups[group_name] = result.size() - 1;
    }
    bool get_success() {return success;}
};
#endif //REGEXLIBRARY_OUTPUT_REG_H