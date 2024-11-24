#ifndef TYPES_H
#define TYPES_H

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include <stdbool.h>

typedef struct {
  SymbolSet terminalSet;
  SymbolSet nonTerminalSet;
  ProductionSet productionSet;
  Symbol initialSymbol;
  bool linearRight;
  bool validated;
  // bool normalized;
} Grammar;

Grammar* Grammar_new(GrammarDefinition* grammarDefinition);
void Grammar_free(Grammar* grammar);

#endif
