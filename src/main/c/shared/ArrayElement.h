#ifndef ARRAY_ELEMENT_H
#define ARRAY_ELEMENT_H

#include "../frontend/syntactic-analysis/AbstractSyntaxTree.h"

typedef union ArrayElement {
  Production* production;
  ProductionRhsRule* productionRhsRule;
  Sentence* sentence;
  Symbol symbol;
} ArrayElement;

#endif
