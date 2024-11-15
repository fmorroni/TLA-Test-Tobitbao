#include "../../../shared/CompilerState.h"
#include "../../../shared/Logger.h"
#include "../../../shared/utils.h"
#include "../AbstractSyntaxTree.h"
#include "ActionsLogger.h"
#include <stdbool.h>

extern unsigned int flexCurrentContext(void);
extern Logger* bisonActionsLogger;

Program* ProgramSemanticAction(CompilerState* compilerState, SentenceArray sentences) {
  logSyntacticAnalyzerAction(__func__);
  Program* program = safeMalloc(sizeof(Program));
  program->sentences = sentences;
  compilerState->abstractSyntaxtTree = program;

  if (0 < flexCurrentContext()) {
    logError(bisonActionsLogger, "The final context is not the default (0): %d", flexCurrentContext());
    compilerState->succeed = false;
  } else {
    compilerState->succeed = true;
  }

  return program;
}
