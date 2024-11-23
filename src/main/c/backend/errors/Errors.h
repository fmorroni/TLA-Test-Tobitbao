#ifndef ERRORS_H
#define ERRORS_H

#include "../../shared/CompilerState.h"

// typedef enum {
//   IncompatibleType,
//   InvalidProductionSet,
//   UndefinedStartingSymbol,
//   UseOfUnassignedVariable,
// } ErrorType;

void initializeErrorsModule(CompilerState* state);
void shutdownErrorsModule();

void Error_incompatibleType(const char* functionName, const char* id);
void Error_alreadyDefined(const char* functionName, const char* id);
void Error_emptySetDefinition(const char* functionName, const char* setId);
void Error_terminalsAndNonTerminalsCollision(
  const char* functionName, const char* terminalSetId, const char* nonTerminalSetId
);
void Error_undefinedInitialSymbol(
  const char* functionName, const char* nonTerminalSetId, const char* expectedInitialSymbol
);
void Error_undefinedTerminalSymbolUsedInProduction(
  const char* functionName, const char* symbol, const char* production, const char* productionSetId,
  const char* terminalSetId
);
void Error_undefinedNonTerminalSymbolUsedInProduction(
  const char* functionName, const char* symbol, const char* production, const char* productionSetId,
  const char* nonTerminalSetId
);
void Error_invalidProduction(const char* functionName, const char* production, const char* productionSetId);
void Error_undefinedSymbol(
  const char* functionName, const char* symbol, const char* production, const char* productionSetId
);
void Error_linearRightAndLeftProductionMix(const char* functionName, const char* productionSetId);

#endif
