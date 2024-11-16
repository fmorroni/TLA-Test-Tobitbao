#ifndef LANGUAGE_UTILS_H
#define LANGUAGE_UTILS_H

#include "../AbstractSyntaxTree.h"

void initializeLanguageUtilsModule();
void shutdownLanguageUtilsModule();

void LanguageBinding_free(LanguageBinding* languageBinding);
void LanguageExpression_free(LanguageExpression* languageExpression);
char* LanguageBinding_toString(LanguageBinding* languageBinding);
char* LanguageExpression_toString(LanguageExpression* languageExpression);

#endif
