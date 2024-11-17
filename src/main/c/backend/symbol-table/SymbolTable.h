#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include <stdbool.h>

typedef struct {
  Id id;
  VariableType type;
  union {
    SymbolSet set;
    LanguageExpression* langExpression;
    GrammarDefinition* grammar;
  };
} SymbolTableEntry;

void initializeSymbolTableModule();
void destroySymbolTableModule();
SymbolTableEntry* SymbolTable_get(Id id);
bool SymbolTable_has(Id id);
bool SymbolTable_putGrammar(Id id, GrammarDefinition* grammar);
bool SymbolTable_putLanguage(Id id, LanguageExpression* expr);
bool SymbolTable_putSymbolSet(Id id, SymbolSet expr);

#endif
