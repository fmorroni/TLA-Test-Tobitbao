#ifndef SENTENCE_ACTIONS_H
#define SENTENCE_ACTIONS_H

#include "../AbstractSyntaxTree.h"

SentenceArray SentenceArray_new(Sentence* sentence);
SentenceArray SentenceArray_push(SentenceArray array, Sentence* sentence);
Sentence* GrammarDefinitionSentence_new(GrammarDefinition* grammarDefinition);
Sentence* SymbolSetBindingSentence_new(SymbolSetBinding* symbolSetBinding);
Sentence* ProductionSetBindingSentence_new(ProductionSetBinding* productionSetBinding);

#endif
