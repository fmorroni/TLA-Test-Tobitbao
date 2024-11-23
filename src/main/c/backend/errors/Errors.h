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

void Error_IncompatibleType(const char* functionName, const char* id);
void Error_AlreadyDefined(const char* functionName, const char* id);

#endif
