//
// Created by User on 22.04.2026.
//

#ifndef REGEXLIBRARY_MIN_DKA_H
#define REGEXLIBRARY_MIN_DKA_H
#include "Automat_DKA.h"
#include <optional>
#include <set>
#include <vector>
#include  <unordered_map>

#include "Builder_DKA.h"
Automat_DKA min_dka(const Automat_DKA& DKA, const set<char>& alphabet);

#endif //REGEXLIBRARY_MIN_DKA_H