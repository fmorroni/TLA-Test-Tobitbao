#ifndef LANGUAGE_ACTIONS_H
#define LANGUAGE_ACTIONS_H

#include "../AbstractSyntaxTree.h"

Language* Language_new(Id referenceId, LanguageIdType type);
LanguageExpression* SimpleLanguageExpression_new(Language* language);
LanguageExpression* UnaryTypeLanguageExpression_new(LanguageExpression* lang, LanguageExpressionType type);
LanguageExpression*
ComplexLanguageExpression_new(LanguageExpression* leftLang, LanguageExpression* rightLang, LanguageExpressionType type);
LanguageBinding* LanguageBinding_new(Id langId, LanguageExpression* langExpression);
Sentence* LanguageBindingSentence_new(LanguageBinding* languageBinding);

#endif
