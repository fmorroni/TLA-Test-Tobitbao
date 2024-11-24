#ifndef ARRAY_ELEMENT_H
#define ARRAY_ELEMENT_H

#include "../backend/domain-specific/DFA.h"
#include "../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "SetElement.h"

typedef union ArrayElement {
  Production* production;
  ProductionRhsRule* productionRhsRule;
  Sentence* sentence;
  Symbol symbol;
  SetElement* setElement;
  Transition* transition;
} ArrayElement;

#endif
