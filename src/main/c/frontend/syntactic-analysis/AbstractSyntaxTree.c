#include "AbstractSyntaxTree.h"
#include "../../shared/Logger.h"
#include "ASTUtils/GrammarUtils.h"
#include "ASTUtils/LanguageUtils.h"
#include "ASTUtils/ProductionRhsUtils.h"
#include "ASTUtils/ProductionUtils.h"
#include "ASTUtils/ProgramUtils.h"
#include "ASTUtils/SentenceUtils.h"
#include "ASTUtils/SymbolUtils.h"
#include <stddef.h>
#include <stdlib.h>

/* MODULE INTERNAL STATE */

static Logger* _logger = NULL;

void initializeAbstractSyntaxTreeModule() {
  _logger = createLogger("AbstractSyntxTree");
  initializeGrammarUtilsModule();
  initializeLanguageUtilsModule();
  initializeProductionRhsUtilsModule();
  initializeProductionUtilsModule();
  initializeProgramUtilsModules();
  initializeSentenceUtilsModule();
  initializeSymbolUtilsModule();
}

void shutdownAbstractSyntaxTreeModule() {
  if (_logger != NULL) {
    destroyLogger(_logger);
  }
  shutdownGrammarUtilsModule();
  shutdownLanguageUtilsModule();
  shutdownProductionRhsUtilsModule();
  shutdownProductionUtilsModule();
  shutdownProgramUtilsModule();
  shutdownSenteceUtilsModule();
  shutdownSymbolUtilsModule();
}

/** PUBLIC FUNCTIONS */

void releaseProgram(Program* program) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  if (program != NULL) {
    Program_free(program);
  }
}
