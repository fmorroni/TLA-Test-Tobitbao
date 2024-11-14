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

void LanguageBinding_free(LanguageBinding* languageBinding) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  LanguageExpression_free(languageBinding->LanguageExpression);
  free(languageBinding->id.id);
  free(languageBinding);
}

void LanguageExpression_free(LanguageExpression* languageExpression) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  switch (languageExpression->type) {
  case LANGUAGE:
    Language_free(languageExpression->language);
    break;
  case LANG_UNION:
  case LANG_INTERSEC:
  case LANG_MINUS:
  case LANG_CONCAT:
    LanguageExpression_free(languageExpression->leftLanguageExpression);
    LanguageExpression_free(languageExpression->rightLanguageExpression);
    break;
  case LANG_REVERSE:
  case LANG_COMPLEMENT:
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
char LanguageExpressionType_toString(LanguageExpressionType type) {
  switch (type) {
  case LANG_UNION:
    return 'u';
  case LANG_INTERSEC:
    return 'n';
  case LANG_CONCAT:
    return '.';
  case LANG_MINUS:
    return '-';
  case LANG_REVERSE:
    return 'R';
  case LANG_COMPLEMENT:
    return 'N';
  default:
    return '?';
  }
}

char* LanguageExpression_toString(LanguageExpression* languageExpression) {
  if (languageExpression->type == LANGUAGE) {
    if (languageExpression->language->type == GRAMMAR_ID) {
      return safeAsprintf("L(" COLORIZE_ID("%s") ")", languageExpression->language->id);
    }
    return safeAsprintf("(" COLORIZE_ID("%s") ")", languageExpression->language->id);
  }
  if (languageExpression->type == LANG_COMPLEMENT || languageExpression->type == LANG_REVERSE) {
    char* unaryExpression = LanguageExpression_toString(languageExpression->unaryLanguageExpression);
    char languageExpressionType = LanguageExpressionType_toString(languageExpression->type);
    char* str = safeAsprintf("%c(%s)", languageExpressionType, unaryExpression);
    free(unaryExpression);
    return str;
  }

  char* leftExpression = LanguageExpression_toString(languageExpression->leftLanguageExpression);
  char* rightExpression = LanguageExpression_toString(languageExpression->rightLanguageExpression);
  char languageExpressionType = LanguageExpressionType_toString(languageExpression->type);
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
