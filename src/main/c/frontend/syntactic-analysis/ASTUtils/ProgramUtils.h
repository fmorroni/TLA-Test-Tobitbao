#ifndef PROGRAM_UTILS_H
#define PROGRAM_UTILS_H

#include "../AbstractSyntaxTree.h"

void initializeProgramUtilsModules();
void shutdownProgramUtilsModule();

void Program_free(Program* program);

#endif
