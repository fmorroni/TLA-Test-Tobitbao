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

char _languageExpressionType_toChar(LanguageExpressionType type);

void LanguageBinding_free(LanguageBinding* languageBinding) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  LanguageExpression_free(languageBinding->LanguageExpression);
  free(languageBinding->id.id);
  free(languageBinding);
}

void LanguageExpression_free(LanguageExpression* languageExpression) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  switch (languageExpression->type) {
  case LANG_T:
    Language_free(languageExpression->language);
    break;
  case LANG_UNION_T:
  case LANG_INTERSECTION_T:
  case LANG_SUBTRACTION_T:
  case LANG_CONCATENATION_T:
    LanguageExpression_free(languageExpression->leftLanguageExpression);
    LanguageExpression_free(languageExpression->rightLanguageExpression);
    break;
  case LANG_REVERSE_T:
  case LANG_COMPLEMENT_T:
    LanguageExpression_free(languageExpression->unaryLanguageExpression);
    break;
  default:
    break;
  }
  free(languageExpression);
}

void Language_free(Language* language) {
  free(language->id.id);
  free(language);
}

// LANG_UNION, LANG_INTERSEC, LANG_MINUS, LANG_CONCAT, LANG_REVERSE
char _languageExpressionType_toChar(LanguageExpressionType type) {
  switch (type) {
  case LANG_UNION_T:
    return 'u';
  case LANG_INTERSECTION_T:
    return 'n';
  case LANG_CONCATENATION_T:
    return '.';
  case LANG_SUBTRACTION_T:
    return '-';
  case LANG_REVERSE_T:
    return 'R';
  case LANG_COMPLEMENT_T:
    return 'N';
  default:
    return '?';
  }
}

char* LanguageExpression_toString(LanguageExpression* languageExpression) {
  if (languageExpression->type == LANG_T) {
    if (languageExpression->language->type == GRAMMAR_ID) {
      return safeAsprintf("L(" COLORIZE_ID("%s") ")", languageExpression->language->id);
    }
    return safeAsprintf("(" COLORIZE_ID("%s") ")", languageExpression->language->id);
  }
  if (languageExpression->type == LANG_COMPLEMENT_T || languageExpression->type == LANG_REVERSE_T) {
    char* unaryExpression = LanguageExpression_toString(languageExpression->unaryLanguageExpression);
    char languageExpressionType = _languageExpressionType_toChar(languageExpression->type);
    char* str = safeAsprintf("%c(%s)", languageExpressionType, unaryExpression);
    free(unaryExpression);
    return str;
  }

  char* leftExpression = LanguageExpression_toString(languageExpression->leftLanguageExpression);
  char* rightExpression = LanguageExpression_toString(languageExpression->rightLanguageExpression);
  char languageExpressionType = _languageExpressionType_toChar(languageExpression->type);
  char* str = safeAsprintf("%s %c %s", leftExpression, languageExpressionType, rightExpression);
  free(leftExpression);
  free(rightExpression);
  return str;
}

char* LanguageBinding_toString(LanguageBinding* languageBinding) {
  char* languageExpression = LanguageExpression_toString(languageBinding->LanguageExpression);
  char* str = safeAsprintf(
    "LanguageBinding{ id: " COLORIZE_ID("%s") ", languageExpression: %s }", languageBinding->id.id, languageExpression
  );
  free(languageExpression);
  return str;
}
