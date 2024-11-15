#include "SentenceActions.h"
#include "../../../shared/Array.h"
#include "../../../shared/ArrayElement.h"
#include "../../../shared/utils.h"
#include "../ASTUtils/SentenceUtils.h"
#include "../AbstractSyntaxTree.h"
#include "ActionsLogger.h"
#include <stdlib.h>

#define INIT_CAP 10

SentenceArray SentenceArray_new(Sentence* sentence) {
  logSyntacticAnalyzerAction(__func__);
  SentenceArray array = Array_new(INIT_CAP, SentenceArray_freeEle, NULL);
  SentenceArray_push(array, sentence);

  return array;
}

SentenceArray SentenceArray_push(SentenceArray array, Sentence* sentence) {
  char* str = Sentence_toString(sentence);
  logSyntacticAnalyzerPushAction(__func__, str);
  free(str);
  ArrayElement ele = {.sentence = sentence};
  Array_push(array, ele);
  return array;
}

Sentence* GrammarDefinitionSentence_new(GrammarDefinition* grammarDefinition) {
  logSyntacticAnalyzerAction(__func__);
  Sentence* sentence = safeMalloc(sizeof(Sentence));
  sentence->type = GRAMMAR_DEFINITION;
  sentence->grammarDefinition = grammarDefinition;

  return sentence;
}

Sentence* SymbolSetBindingSentence_new(SymbolSetBinding* symbolSetBinding) {
  logSyntacticAnalyzerAction(__func__);
  Sentence* sentence = safeMalloc(sizeof(Sentence));
  sentence->type = SYMBOL_SET;
  sentence->symbolSetBinding = symbolSetBinding;

  return sentence;
}

Sentence* ProductionSetBindingSentence_new(ProductionSetBinding* productionSetBinding) {
  logSyntacticAnalyzerAction(__func__);
  Sentence* sentence = safeMalloc(sizeof(Sentence));
  sentence->type = PRODUCTION_SET;
  sentence->productionSetBinding = productionSetBinding;

  return sentence;
}
