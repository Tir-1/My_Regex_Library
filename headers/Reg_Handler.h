//
// Created by User on 21.04.2026.
//

#ifndef REGEXLIBRARY_REG_HANDLER_H
#define REGEXLIBRARY_REG_HANDLER_H
#include <memory>
#include "Interfaces/I_Lexer.h"
#include "Interfaces/I_Parser.h"
#include "Pattern.h"
#include "DKA/Builder_DKA.h"
#include "DKA/min_dka.h"
#include "NKA/Builder_NKA.h"
class Reg_Handler {
    string reg;
    std::unique_ptr<I_Lexer> lexer;
    std::unique_ptr<I_Parser> parser;
    Builder_DKA builder_AST;
    Builder_NKA builder_NKA;
    public:
        Reg_Handler(const string& reg, std::unique_ptr<I_Lexer> _lexer, std::unique_ptr<I_Parser> _parser): reg(reg), lexer(std::move(_lexer)), parser(std::move(_parser)) {};
        optional<Pattern> work();

};

#endif //REGEXLIBRARY_REG_HANDLER_H