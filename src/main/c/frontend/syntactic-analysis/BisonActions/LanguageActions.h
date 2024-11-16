#ifndef LANGUAGE_ACTIONS_H
#define LANGUAGE_ACTIONS_H

#include "../AbstractSyntaxTree.h"

LanguageExpression* SimpleLanguageExpression_new(Id id, LanguageExpressionType type);
LanguageExpression* UnaryLanguageExpression_new(LanguageExpression* lang, LanguageExpressionType type);
LanguageExpression*
BinaryLanguageExpression_new(LanguageExpression* leftLang, LanguageExpression* rightLang, LanguageExpressionType type);
LanguageBinding* LanguageBinding_new(Id langId, LanguageExpression* langExpression);
Sentence* LanguageBindingSentence_new(LanguageBinding* languageBinding);

#endif
