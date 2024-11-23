#include "../../../shared/ColorMacros.h"
#include "../../../shared/Logger.h"
#include "../../../shared/String.h"
#include "../AbstractSyntaxTree.h"
#include <stdlib.h>

static Logger* _logger = NULL;

void initializeGrammarUtilsModule() {
  _logger = createLogger("GrammarUtils");
}

void shutdownGrammarUtilsModule() {
  if (_logger != NULL) {
    destroyLogger(_logger);
  }
}

void GrammarDefinition_free(GrammarDefinition* grammarDefinition) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  free(grammarDefinition->id.id);
  free(grammarDefinition->terminalSetId.id);
  free(grammarDefinition->nonTerminalSetId.id);
  free(grammarDefinition->productionSetId.id);
  free(grammarDefinition->initialSymbol.symbol);
  free(grammarDefinition);
}

char* GrammarDefinition_toString(GrammarDefinition* grammarDefinition) {
  char* str = safeAsprintf(
    "GrammarDefinition{ id: " COLORIZE_ID("%s") ", terminalSetId: " COLORIZE_ID("%s"
    ) ", nonTerminalSetId: " COLORIZE_ID("%s") ", productionSetId: " COLORIZE_ID("%s"
    ) ", initialSymbol: " COLORIZE_SYMBOL("%s") " }",
    grammarDefinition->id.id, grammarDefinition->terminalSetId.id, grammarDefinition->nonTerminalSetId.id,
    grammarDefinition->productionSetId.id, grammarDefinition->initialSymbol.symbol
  );
  return str;
}
