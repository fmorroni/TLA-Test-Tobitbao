#ifndef GRAMMAR_UTILS_H
#define GRAMMAR_UTILS_H

#include "../AbstractSyntaxTree.h"

void initializeGrammarUtilsModule();
void shutdownGrammarUtilsModule();

void GrammarDefinition_free(GrammarDefinition* grammarDefinition);
char* GrammarDefinition_toString(GrammarDefinition* grammarDefinition);

#endif
