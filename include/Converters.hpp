#pragma once

#include "DFA.hpp"
#include "NFA.hpp"
#include "RegEx.hpp"

NFA NFA2RegularNFA(const NFA& nfa);
