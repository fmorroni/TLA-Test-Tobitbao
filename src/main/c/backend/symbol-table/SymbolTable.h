#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "../../backend/domain-specific/DFA.h"
#include "../../backend/domain-specific/Grammar.h"
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include <stdbool.h>

typedef struct SetIteratorCDT* SetIterator;

typedef enum { LANGUAGE_T = 0, SYMBOL_SET_T, PRODUCTION_SET_T, GRAMMAR_DEFINITION_T } VariableType;

typedef struct {
  Id id;
  VariableType type;
  union {
    GrammarDefinition* grammarDefinition;
    LanguageExpression* langExpression;
    ProductionSet productionSet;
    SymbolSet symbolSet;
    Grammar* grammar;
    DFA* dfa;
  };
} SymbolTableEntry;

void initializeSymbolTableModule();
void shutdownSymbolTableModule();
SymbolTableEntry* SymbolTable_get(Id id);
bool SymbolTable_has(Id id);
bool SymbolTable_putGrammarDefinition(Id id, GrammarDefinition* grammar);
bool SymbolTable_putLanguage(Id id, LanguageExpression* expr);
bool SymbolTable_putProductionSet(Id id, ProductionSet set);
bool SymbolTable_putSymbolSet(Id id, SymbolSet expr);
bool SymbolTable_replaceGrammarDefinitionForGrammar(Id id, Grammar* grammar);

bool GrammarTable_put(Id id, Grammar* grammar);
Grammar* GrammarTable_get(Id id);

DFA* DfaTable_get(Id id);
bool DfaTable_put(Id id, DFA* dfa);
SetIterator DfaTableIterator_new();
void DfaTableIterator_free(SetIterator iter);
bool DfaTableIterator_hasNext(SetIterator iter);
DFA* DfaTableIterator_next(SetIterator iter);

#endif
