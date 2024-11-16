#include "LanguageUtils.h"
#include "../../../shared/Logger.h"
#include "../../../shared/String.h"
#include "../AbstractSyntaxTree.h"
#include "ColorMacros.h"
#include <stdlib.h>

static Logger* _logger = NULL;

void initializeLanguageUtilsModule() {
  _logger = createLogger("LanguageUtils");
}

void shutdownLanguageUtilsModule() {
  if (_logger != NULL) {
    destroyLogger(_logger);
  }
}

char* _languageExpressionType_toString(LanguageExpressionType type);

void LanguageBinding_free(LanguageBinding* languageBinding) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  LanguageExpression_free(languageBinding->languageExpression);
  free(languageBinding->id.id);
  free(languageBinding);
}

void LanguageExpression_free(LanguageExpression* languageExpression) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  switch (languageExpression->type) {
  case LANG_ID_T:
    free(languageExpression->languageId.id);
    break;
  case LANG_OF_GRAMMAR_T:
    free(languageExpression->grammarId.id);
    break;
  case LANG_UNION_T:
  case LANG_INTERSECTION_T:
  case LANG_SUBTRACTION_T:
  case LANG_CONCATENATION_T:
    LanguageExpression_free(languageExpression->leftExpression);
    LanguageExpression_free(languageExpression->rightExpression);
    break;
  case LANG_REVERSE_T:
  case LANG_COMPLEMENT_T:
    LanguageExpression_free(languageExpression->unaryExpression);
    break;
  default:
    break;
  }
  free(languageExpression);
}

char* _languageExpressionType_toString(LanguageExpressionType type) {
  switch (type) {
  case LANG_UNION_T:
    return "u";
  case LANG_INTERSECTION_T:
    return "n";
  case LANG_CONCATENATION_T:
    return ".";
  case LANG_SUBTRACTION_T:
    return "-";
  default:
    logError(_logger, "Invalid LanguageExpressionType: %d", type);
    return "?";
  }
}

char* LanguageExpression_toString(LanguageExpression* languageExpression) {
  char* str;
  char* exprStr1;
  char* exprStr2;
  switch (languageExpression->type) {
  case LANG_ID_T:
    return safeAsprintf(COLORIZE_ID("%s"), languageExpression->grammarId);
  case LANG_OF_GRAMMAR_T:
    return safeAsprintf("L(" COLORIZE_ID("%s") ")", languageExpression->languageId);
  case LANG_COMPLEMENT_T:
    exprStr1 = LanguageExpression_toString(languageExpression->unaryExpression);
    str = safeAsprintf("(¬%s)", exprStr1);
    free(exprStr1);
    return str;
  case LANG_REVERSE_T:
    exprStr1 = LanguageExpression_toString(languageExpression->unaryExpression);
    str = safeAsprintf("(%s^R)", exprStr1);
    free(exprStr1);
    return str;
  case LANG_UNION_T:
  case LANG_INTERSECTION_T:
  case LANG_SUBTRACTION_T:
  case LANG_CONCATENATION_T:;
    exprStr1 = LanguageExpression_toString(languageExpression->leftExpression);
    exprStr2 = LanguageExpression_toString(languageExpression->rightExpression);
    str = safeAsprintf("(%s %s %s)", exprStr1, _languageExpressionType_toString(languageExpression->type), exprStr2);
    free(exprStr1);
    free(exprStr2);
    return str;
  default:
    logError(_logger, "Invalid LanguageExpressionType: %d", languageExpression->type);
    return NULL;
  }
}

char* LanguageBinding_toString(LanguageBinding* languageBinding) {
  char* exprStr = LanguageExpression_toString(languageBinding->languageExpression);
  char* str =
    safeAsprintf("LanguageBinding{ id: " COLORIZE_ID("%s") ", expression: %s }", languageBinding->id.id, exprStr);
  free(exprStr);
  return str;
}
