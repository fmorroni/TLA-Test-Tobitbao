#include "../../../shared/ArrayElement.h"
#include "../../../shared/Logger.h"
#include "../AbstractSyntaxTree.h"
#include "GrammarUtils.h"
#include "LanguageUtils.h"
#include "ProductionUtils.h"
#include "SymbolUtils.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static Logger* _logger = NULL;

void initializeSentenceUtilsModule() {
  _logger = createLogger("SentenceUtils");
}

void shutdownSenteceUtilsModule() {
  if (_logger != NULL) {
    destroyLogger(_logger);
  }
}

void SentenceArray_freeEle(ArrayElement ele) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  switch (ele.sentence->type) {
  case GRAMMAR_DEFINITION:
    GrammarDefinition_free(ele.sentence->grammarDefinition);
    break;
  case SYMBOL_SET:
    SymbolSetBinding_free(ele.sentence->symbolSetBinding);
    break;
  case PRODUCTION_SET:
    ProductionSetBinding_free(ele.sentence->productionSetBinding);
    break;
  case LANGUAGE_SENTENCE:
    LanguageBinding_free(ele.sentence->languageBinding);
  }
  free(ele.sentence);
}

char* Sentence_toString(Sentence* sentence) {
  char* str;
  switch (sentence->type) {
  case GRAMMAR_DEFINITION:
    str = GrammarDefinition_toString(sentence->grammarDefinition);
    break;
  case SYMBOL_SET:
    str = SymbolSetBinding_toString(sentence->symbolSetBinding);
    break;
  case PRODUCTION_SET:
    str = ProductionSetBinding_toString(sentence->productionSetBinding);
    break;
  case LANGUAGE_SENTENCE:
    str = LanguageBinding_toString(sentence->languageBinding);
    break;
  }

  return str;
}
