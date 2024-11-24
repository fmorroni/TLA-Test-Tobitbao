#ifndef ERRORS_H
#define ERRORS_H

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
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
void Error_noInitialSymbolProduction(const char* functionName, GrammarDefinition* grammarDefinition);
void Error_terminalsAndNonTerminalsCollision(const char* functionName, GrammarDefinition* grammarDefinition);
void Error_undefinedInitialSymbol(const char* functionName, GrammarDefinition* grammarDefinition);
void Error_undefinedTerminalSymbolUsedInProduction(
  const char* functionName, const char* symbol, const char* production, GrammarDefinition* grammarDefinition
);
void Error_undefinedNonTerminalSymbolUsedInProduction(
  const char* functionName, const char* symbol, const char* production, GrammarDefinition* grammarDefinition
);
void Error_invalidProduction(const char* functionName, const char* production, GrammarDefinition* grammarDefinition);
void Error_undefinedSymbol(
  const char* functionName, const char* symbol, const char* production, GrammarDefinition* grammarDefinition
);
void Error_linearLeftProductionSet(const char* functionName, GrammarDefinition* grammarDefinition);
void Error_linearRightAndLeftProductionMix(const char* functionName, GrammarDefinition* grammarDefinition);

#endif
