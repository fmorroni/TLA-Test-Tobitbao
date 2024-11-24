#include "../../../backend/errors/Errors.h"
#include "../../../backend/symbol-table/SymbolTable.h"
#include "../../../shared/utils.h"
#include "../AbstractSyntaxTree.h"
#include "ActionsLogger.h"
#include <stddef.h>

GrammarDefinition*
GrammarDefinition_new(Id grammarId, Id terminalSetId, Id nonTerminalSetId, Id productionSetId, Symbol initialSymbolId) {
  logSyntacticAnalyzerAction(__func__);
  GrammarDefinition* grammar = safeMalloc(sizeof(GrammarDefinition));
  grammar->id = grammarId;
  grammar->terminalSetId = terminalSetId;
  grammar->nonTerminalSetId = nonTerminalSetId;
  grammar->productionSetId = productionSetId;
  grammar->initialSymbol = initialSymbolId;

  SymbolTable_putGrammarDefinition(grammarId, grammar);

  return grammar;
}

void GrammarDefinition_alreadyDefined(Id grammarId) {
  Error_alreadyDefined(__func__, grammarId.id);
}
