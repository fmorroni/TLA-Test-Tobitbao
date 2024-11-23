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

void Error_terminalsAndNonTerminalsCollision(
  const char* functionName, const char* terminalSetId, const char* nonTerminalSetId
) {
  *errors = true;
  logError(
    _logger,
    "%s: non void intersection between terminal symbol set " COLORIZE_ID("%s"
    ) " and non terminal symbol set " COLORIZE_ID("%s"),
    functionName, terminalSetId, nonTerminalSetId
  );
}

void Error_undefinedInitialSymbol(
  const char* functionName, const char* nonTerminalSetId, const char* expectedInitialSymbol
) {
  *errors = true;
  logError(
    _logger,
    "%s: non terminal symbol set " COLORIZE_ID("%s") " doesn't contain expected initial symbol " COLORIZE_SYMBOL("%s"),
    functionName, nonTerminalSetId, expectedInitialSymbol
  );
}

void Error_undefinedTerminalSymbolUsedInProduction(
  const char* functionName, const char* symbol, const char* production, const char* productionSetId,
  const char* terminalSetId
) {
  *errors = true;
  logError(
    _logger,
    "%s: undefined terminal symbol " COLORIZE_SYMBOL("%s") " in terminal set " COLORIZE_ID("%s"
    ) " used in production %s of production set " COLORIZE_SYMBOL("%s"),
    functionName, symbol, terminalSetId, production, productionSetId
  );
}

void Error_undefinedNonTerminalSymbolUsedInProduction(
  const char* functionName, const char* symbol, const char* production, const char* productionSetId,
  const char* nonTerminalSetId
) {
  *errors = true;
  logError(
    _logger,
    "%s: undefined non terminal symbol " COLORIZE_SYMBOL("%s") " in non terminal set " COLORIZE_ID("%s"
    ) " used in production %s of production set " COLORIZE_SYMBOL("%s"),
    functionName, symbol, nonTerminalSetId, production, productionSetId
  );
}

void Error_invalidProduction(const char* functionName, const char* production, const char* productionSetId) {
  *errors = true;
  logError(
    _logger, "%s: production %s of production set " COLORIZE_SYMBOL("%s") " is not valid for a regular grammar",
    functionName, production, productionSetId
  );
}

void Error_undefinedSymbol(
  const char* functionName, const char* symbol, const char* production, const char* productionSetId
) {
  *errors = true;
  logError(
    _logger,
    "%s: " COLORIZE_SYMBOL("%s") " symbol in production %s of production set " COLORIZE_SYMBOL("%s") " is not defined",
    functionName, symbol, production, productionSetId
  );
}

void Error_linearRightAndLeftProductionMix(const char* functionName, const char* productionSetId) {
  *errors = true;
  logError(
    _logger, "%s: mix of linear right and left productions in " COLORIZE_ID("%s"), functionName, productionSetId
  );
}
