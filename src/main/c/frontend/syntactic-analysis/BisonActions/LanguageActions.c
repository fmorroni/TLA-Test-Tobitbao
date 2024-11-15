#include "../../../shared/utils.h"
#include "../AbstractSyntaxTree.h"
#include "ActionsLogger.h"
#include <stdio.h>

Language* Language_new(Id referenceId, LanguageIdType type) {
  logSyntacticAnalyzerAction(__FUNCTION__);
  Language* language = safeMalloc(sizeof(Language));
  language->id = referenceId;
  language->type = type;
  return language;
}

LanguageExpression* SimpleLanguageExpression_new(Language* language) {
  logSyntacticAnalyzerAction(__FUNCTION__);
  LanguageExpression* langExpression = safeMalloc(sizeof(LanguageExpression));
  langExpression->type = LANGUAGE;
  langExpression->language = language;
  return langExpression;
}

LanguageExpression* UnaryTypeLanguageExpression_new(LanguageExpression* lang, LanguageExpressionType type) {
  logSyntacticAnalyzerAction(__func__);
  LanguageExpression* langExpression = safeMalloc(sizeof(LanguageExpression));
  langExpression->type = type;
  langExpression->unaryLanguageExpression = lang;
  return langExpression;
}

LanguageExpression* ComplexLanguageExpression_new(
  LanguageExpression* leftLang, LanguageExpression* rightLang, LanguageExpressionType type
) {
  logSyntacticAnalyzerAction(__FUNCTION__);
  LanguageExpression* langExpression = safeMalloc(sizeof(LanguageExpression));
  langExpression->type = type;
  langExpression->leftLanguageExpression = leftLang;
  langExpression->rightLanguageExpression = rightLang;
  return langExpression;
}

LanguageBinding* LanguageBinding_new(Id langId, LanguageExpression* langExpression) {
  LanguageBinding* languageBinding = safeMalloc(sizeof(LanguageBinding));
  languageBinding->id = langId;
  languageBinding->LanguageExpression = langExpression;
  return languageBinding;
}

Sentence* LanguageBindingSentence_new(LanguageBinding* languageBinding) {
  logSyntacticAnalyzerAction(__func__);
  Sentence* sentence = safeMalloc(sizeof(Sentence));
  sentence->type = LANGUAGE_SENTENCE;
  sentence->languageBinding = languageBinding;
  return sentence;
}
