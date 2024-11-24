#ifndef SET_ELEMENT_H
#define SET_ELEMENT_H

#include "../backend/domain-specific/DFA.h"
#include "../backend/symbol-table/SymbolTable.h"
#include "../frontend/syntactic-analysis/AbstractSyntaxTree.h"

typedef union SetElement {
  Production* production;
  ProductionRhsRule* productionRhsRule;
  Symbol symbol;
  SymbolTableEntry symbolTableEntry;
  State state;
  // Transition* transition;
} SetElement;

#endif
