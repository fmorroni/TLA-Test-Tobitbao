#include "backend/domain-specific/SemanticValidations.h"
#include "backend/errors/Errors.h"
#include "backend/symbol-table/SymbolTable.h"
#include "frontend/lexical-analysis/FlexActions.h"
#include "frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "frontend/syntactic-analysis/BisonActions/ActionsLogger.h"
#include "frontend/syntactic-analysis/SyntacticAnalyzer.h"
#include "shared/Array.h"
#include "shared/CompilerState.h"
#include "shared/Logger.h"
#include "shared/Set.h"
#include <stddef.h>
#include <stdlib.h>

/**
 * The main entry-point of the entire application. If you use "strtok" to
 * parse anything inside this project instead of using Flex and Bison, I will
 * find you, and I will kill you (Bryan Mills; "Taken", 2008).
 */
int main(const int count, const char** arguments) {
  Logger* logger = createLogger("EntryPoint");
  CompilerState compilerState = {.abstractSyntaxtTree = NULL, .succeed = false, .errors = false, .value = 0};

  initializeSymbolTableModule();
  initializeFlexActionsModule();
  initializeBisonActionsModule();
  initializeSyntacticAnalyzerModule();
  initializeAbstractSyntaxTreeModule();
  initializeSemanticValidationModule();
  initializeErrorsModule(&compilerState);
  Array_initializeLogger();
  Set_initializeLogger();

  // Logs the arguments of the application.
  for (int k = 0; k < count; ++k) {
    logDebugging(logger, "Argument %d: \"%s\"", k, arguments[k]);
  }

  // Begin compilation process.
  const SyntacticAnalysisStatus syntacticAnalysisStatus = parse(&compilerState);
  CompilationStatus compilationStatus = SUCCEED;
  if (syntacticAnalysisStatus == ACCEPT) {
    // ----------------------------------------------------------------------------------------
    // Beginning of the Backend... ------------------------------------------------------------

    logInformation(logger, "Program:");
    Program* program = compilerState.abstractSyntaxtTree;

    semanticValidation(program);
    if (!compilerState.errors) {
      // compilerState.value = computeProgram(program);
      // generate(&compilerState);
    } else {
      logError(logger, "The semantic validation phase rejects the input program.");
      compilationStatus = FAILED;
    }

    // ...end of the Backend. -----------------------------------------------------------------
    // ----------------------------------------------------------------------------------------
    logDebugging(logger, "Releasing AST resources...");
    releaseProgram(program);
  } else {
    logError(logger, "The syntactic-analysis phase rejects the input program.");
    compilationStatus = FAILED;
  }

  logDebugging(logger, "Releasing modules resources...");
  Set_freeLogger();
  Array_freeLogger();
  shutdownErrorsModule();
  shutdownSemanticValidationModule();
  shutdownAbstractSyntaxTreeModule();
  shutdownSyntacticAnalyzerModule();
  shutdownBisonActionsModule();
  shutdownFlexActionsModule();
  shutdownSymbolTableModule();
  logDebugging(logger, "Compilation is done.");
  destroyLogger(logger);
  return compilationStatus;
}
