//
// Created by User on 21.04.2026.
//

#ifndef REGEXLIBRARY_REGULAR_H
#define REGEXLIBRARY_REGULAR_H
#include <string>
#include "DKA/Automat_DKA.h"
#include "NKA/Automat_NKA.h"
#include "Output_reg.h"
#include <string_view>
class Pattern {
    std::string reg;
    Automat_DKA DKA;
    Automat_NKA NKA;
    bool use_groups = false;
    public:
        explicit Pattern(string reg): reg(std::move(reg)) {}
        [[nodiscard]] std::string get_reg()const { return reg; }
        void set_DKA(Automat_DKA DKA_){this->DKA = std::move(DKA_);}
        void set_NKA(Automat_NKA NKA_){this->NKA = std::move(NKA_), use_groups = true;}
        Output_reg search(std::string_view text);
        Automat_DKA save_DKA(bool& flag) {
            flag = true;
            if (use_groups)flag = false;
            return DKA;
        }

};

#endif //REGEXLIBRARY_REGULAR_H