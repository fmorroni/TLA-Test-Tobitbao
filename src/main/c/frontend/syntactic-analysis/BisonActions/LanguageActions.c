#include "../../../shared/Logger.h"
#include "../../../shared/utils.h"
#include "../ASTUtils/LanguageUtils.h"
#include "../AbstractSyntaxTree.h"
#include "ActionsLogger.h"
#include <stdio.h>
#include <stdlib.h>

extern Logger* bisonActionsLogger;

LanguageExpression* SimpleLanguageExpression_new(Id id, LanguageExpressionType type) {
  LanguageExpression* langExpression = safeMalloc(sizeof(LanguageExpression));
  switch (type) {
  case LANG_ID_T:
    logDebugging(bisonActionsLogger, "%s: %s", __func__, id.id);
    langExpression->languageId = id;
    break;
  case LANG_OF_GRAMMAR_T:
    logDebugging(bisonActionsLogger, "%s: L(%s)", __func__, id.id);
    langExpression->grammarId = id;
    break;
  default:
    logError(
      bisonActionsLogger,
      "Invalid language expression type. "
      "Should be %d or %d, was %d.",
      LANG_ID_T, LANG_OF_GRAMMAR_T, type
    );
    free(langExpression);
    return NULL;
  }
  langExpression->type = type;
  return langExpression;
}

LanguageExpression* UnaryLanguageExpression_new(LanguageExpression* innerExpr, LanguageExpressionType type) {
  LanguageExpression* outerExpr = safeMalloc(sizeof(LanguageExpression));
  outerExpr->type = type;
  outerExpr->unaryExpression = innerExpr;
  char* exprStr = LanguageExpression_toString(outerExpr);
  logDebugging(bisonActionsLogger, "%s: %s", __func__, exprStr);
  free(exprStr);
  return outerExpr;
}

LanguageExpression*
BinaryLanguageExpression_new(LanguageExpression* leftExpr, LanguageExpression* rightExpr, LanguageExpressionType type) {
  LanguageExpression* outerExpr = safeMalloc(sizeof(LanguageExpression));
  outerExpr->type = type;
  outerExpr->leftExpression = leftExpr;
  outerExpr->rightExpression = rightExpr;
  char* exprStr = LanguageExpression_toString(outerExpr);
  logDebugging(bisonActionsLogger, "%s: %s", __func__, exprStr);
  free(exprStr);
  return outerExpr;
}

LanguageBinding* LanguageBinding_new(Id langId, LanguageExpression* langExpression) {
  LanguageBinding* languageBinding = safeMalloc(sizeof(LanguageBinding));
  languageBinding->id = langId;
  languageBinding->languageExpression = langExpression;
  return languageBinding;
}

Sentence* LanguageBindingSentence_new(LanguageBinding* languageBinding) {
  logSyntacticAnalyzerAction(__func__);
  Sentence* sentence = safeMalloc(sizeof(Sentence));
  sentence->type = LANGUAGE_SENTENCE;
  sentence->languageBinding = languageBinding;
  return sentence;
}
