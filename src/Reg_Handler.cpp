#include "../headers/Reg_Handler.h"
optional<Pattern> Reg_Handler::work() {
    auto tokens = lexer->lex(reg);
    if (tokens == std::nullopt) return std::nullopt;
    bool use_groups = lexer->get_use_group();
    parser->set_tokens(move(tokens.value()));
    auto maybe_root = parser->parse();
    if (maybe_root == std::nullopt) return std::nullopt;
    auto root = move(maybe_root.value());
    Pattern pattern(reg);
    if (use_groups) {
        Automat_NKA NKA = builder_NKA.build_NKA(root.get());
        pattern.set_NKA(NKA);
    }
    else {
        Automat_DKA DKA = builder_AST.make_DKA(root->clone());
        DKA = min_dka(DKA);
        pattern.set_DKA(DKA);
    }
    return pattern;
}