#ifndef SEMANTIC_VALIDATIONS_H
#define SEMANTIC_VALIDATIONS_H

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include <stdbool.h>

bool semanticValidation(Program* program);
// void computeProgram(Program* program);

void initializeSemanticValidationModule();
void shutdownSemanticValidationModule();

#endif
