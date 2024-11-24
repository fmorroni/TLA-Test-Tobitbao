#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/ColorMacros.h"
#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include <stdbool.h>
#include <stddef.h>

static Logger* _logger = NULL;
static bool* errors;

void initializeErrorsModule(CompilerState* state) {
  _logger = createLogger("ErrorLogger");
  errors = &state->errors;
}

void shutdownErrorsModule() {
  destroyLogger(_logger);
}

void Error_incompatibleType(const char* functionName, const char* id) {
  *errors = true;
  logError(_logger, "%s: " COLORIZE_ID("%s") " isn't defined.", functionName, id);
}

void Error_alreadyDefined(const char* functionName, const char* id) {
  *errors = true;
  logError(_logger, "%s: " COLORIZE_ID("%s") " already defined.", functionName, id);
}

void Error_emptySetDefinition(const char* functionName, const char* setId) {
  *errors = true;
  logError(_logger, "%s: set " COLORIZE_ID("%s") " is empty", functionName, setId);
}

void Error_noInitialSymbolProduction(const char* functionName, GrammarDefinition* grammarDefinition) {
  *errors = true;
  logError(
    _logger, "%s: no production using initial symbol " COLORIZE_SYMBOL("%s") " in production set " COLORIZE_ID("%s"),
    functionName, grammarDefinition->initialSymbol.symbol, grammarDefinition->productionSetId.id
  );
}

void Error_terminalsAndNonTerminalsCollision(const char* functionName, GrammarDefinition* grammarDefinition) {
  *errors = true;
  logError(
    _logger,
    "%s: non void intersection between terminal symbol set " COLORIZE_ID("%s"
    ) " and non terminal symbol set " COLORIZE_ID("%s"),
    functionName, grammarDefinition->terminalSetId.id, grammarDefinition->nonTerminalSetId.id
  );
}

void Error_undefinedInitialSymbol(const char* functionName, GrammarDefinition* grammarDefinition) {
  *errors = true;
  logError(
    _logger,
    "%s: non terminal symbol set " COLORIZE_ID("%s") " doesn't contain expected initial symbol " COLORIZE_SYMBOL("%s"),
    functionName, grammarDefinition->nonTerminalSetId.id, grammarDefinition->initialSymbol.symbol
  );
}

void Error_undefinedTerminalSymbolUsedInProduction(
  const char* functionName, const char* symbol, const char* production, GrammarDefinition* grammarDefinition
) {
  *errors = true;
  logError(
    _logger,
    "%s: undefined terminal symbol " COLORIZE_SYMBOL("%s") " in terminal set " COLORIZE_ID("%s"
    ) " used in production %s of production set " COLORIZE_SYMBOL("%s"),
    functionName, symbol, grammarDefinition->terminalSetId.id, production, grammarDefinition->productionSetId.id
  );
}

void Error_undefinedNonTerminalSymbolUsedInProduction(
  const char* functionName, const char* symbol, const char* production, GrammarDefinition* grammarDefinition
) {
  *errors = true;
  logError(
    _logger,
    "%s: undefined non terminal symbol " COLORIZE_SYMBOL("%s") " in non terminal set " COLORIZE_ID("%s"
    ) " used in production %s of production set " COLORIZE_SYMBOL("%s"),
    functionName, symbol, grammarDefinition->nonTerminalSetId.id, production, grammarDefinition->productionSetId.id
  );
}

void Error_invalidProduction(const char* functionName, const char* production, GrammarDefinition* grammarDefinition) {
  *errors = true;
  logError(
    _logger, "%s: production %s of production set " COLORIZE_SYMBOL("%s") " is not valid for a regular grammar",
    functionName, production, grammarDefinition->productionSetId.id
  );
}

void Error_undefinedSymbol(
  const char* functionName, const char* symbol, const char* production, GrammarDefinition* grammarDefinition
) {
  *errors = true;
  logError(
    _logger,
    "%s: " COLORIZE_SYMBOL("%s") " symbol in production %s of production set " COLORIZE_SYMBOL("%s") " is not defined",
    functionName, symbol, production, grammarDefinition->productionSetId.id
  );
}

void Error_linearLeftProductionSet(const char* functionName, GrammarDefinition* grammarDefinition) {
  *errors = true;
  logError(
    _logger, "%s: linear left grammars not allowed. Found in " COLORIZE_ID("%s"), functionName,
    grammarDefinition->productionSetId.id
  );
}

void Error_linearRightAndLeftProductionMix(const char* functionName, GrammarDefinition* grammarDefinition) {
  *errors = true;
  logError(
    _logger, "%s: mix of linear right and left productions in " COLORIZE_ID("%s"), functionName,
    grammarDefinition->productionSetId.id
  );
}
