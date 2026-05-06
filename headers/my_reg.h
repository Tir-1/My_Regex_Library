//
// Created by User on 23.04.2026.
//

#ifndef REGEXLIBRARY_MYREG_H
#define REGEXLIBRARY_MYREG_H
#include "Pattern.h"
#include "Output_reg.h"
#include "Reg_Handler.h"
#include "AST/Lexer.h"
#include "AST/Parser.h"
#include "Automat_DKA.h"
Output_reg search(const string& reg, const string& text);
Pattern compile(const string& reg);
string recover_reg(const Automat_DKA& automat);
bool isomorphic(const Automat_DKA& first, const Automat_DKA& second);
Automat_DKA intersect(const Automat_DKA& first, const Automat_DKA& second);
Automat_DKA difference(const Automat_DKA& first, const Automat_DKA& second);
Automat_DKA get_null_dka();
#endif //REGEXLIBRARY_MYREG_H