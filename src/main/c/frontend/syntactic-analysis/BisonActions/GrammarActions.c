#include "../../../backend/symbol-table/SymbolTable.h"
#include "../../../shared/utils.h"
#include "../AbstractSyntaxTree.h"
#include "ActionsLogger.h"

GrammarDefinition*
GrammarDefinition_new(Id grammarId, Id terminalSetId, Id nonTerminalSetId, Id productionSetId, Id initialSymbolId) {
  logSyntacticAnalyzerAction(__func__);
  GrammarDefinition* grammar = safeMalloc(sizeof(GrammarDefinition));
  grammar->id = grammarId;
  grammar->terminalSetId = terminalSetId;
  grammar->nonTerminalSetId = nonTerminalSetId;
  grammar->productionSetId = productionSetId;
  grammar->initialSymbolId = initialSymbolId;

  if (!SymbolTable_putGrammar(grammarId, grammar)) {
    logAlreadyDefinedError(__func__, grammarId.id);
  }

  return grammar;
}
