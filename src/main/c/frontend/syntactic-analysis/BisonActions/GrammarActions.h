#ifndef GRAMMAR_ACTIONS_H
#define GRAMMAR_ACTIONS_H

#include "../AbstractSyntaxTree.h"

GrammarDefinition*
GrammarDefinition_new(Id grammarId, Id terminalSetId, Id nonTerminalSetId, Id productionSetId, Symbol initialSymbolId);
void GrammarDefinition_alreadyDefined(Id grammarId);

#endif
