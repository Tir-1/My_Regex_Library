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

Output_reg search(const string& reg, const string& text);
Pattern compile(const string& reg);
string recover_reg(const Automat_DKA& automat);
#endif //REGEXLIBRARY_MYREG_H