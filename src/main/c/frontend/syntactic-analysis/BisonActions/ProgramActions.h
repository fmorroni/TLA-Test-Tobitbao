#ifndef PROGRAM_ACTIONS_H
#define PROGRAM_ACTIONS_H

#include "../../../shared/CompilerState.h"
#include "../AbstractSyntaxTree.h"

Program* ProgramSemanticAction(CompilerState* compilerState, SentenceArray sentences);

#endif
