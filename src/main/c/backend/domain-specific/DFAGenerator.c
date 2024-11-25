#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/Array.h"
#include "../../shared/ArrayElement.h"
#include "../symbol-table/SymbolTable.h"
#include "DFA.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void generateDFAs(Program* program) {
  size_t sentencesLen = Array_getLen(program->sentences);
  for (int i = 0; i < sentencesLen; ++i) {
    Sentence* sentence = Array_get(program->sentences, i).sentence;
    DFA* dfa;
    switch (sentence->type) {
    case GRAMMAR_DEFINITION:
      dfa = DFA_fromGrammar(sentence->grammarDefinition->id, GrammarTable_get(sentence->grammarDefinition->id));
      DfaTable_put(sentence->grammarDefinition->id, dfa);
      break;
    case LANGUAGE_BINDING:
      dfa = DFA_fromLanguageExpression(sentence->languageBinding->id, sentence->languageBinding->languageExpression);
      DfaTable_put(sentence->languageBinding->id, dfa);
    default:
      break;
    }
  }
}
