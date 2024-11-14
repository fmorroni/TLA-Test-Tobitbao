#ifndef SET_ELEMENT_H
#define SET_ELEMENT_H

#include "../frontend/syntactic-analysis/AbstractSyntaxTree.h"

typedef union SetElement {
  Production* production;
  ProductionRhsRule* productionRhsRule;
  Symbol symbol;
} SetElement;

#endif
