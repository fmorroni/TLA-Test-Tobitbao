#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include <stdbool.h>

typedef struct {
  Id id;
  VariableType type;
  union {
    GrammarDefinition* grammar;
    LanguageExpression* langExpression;
    ProductionSet productionSet;
    SymbolSet symbolSet;
  };
} SymbolTableEntry;

void initializeSymbolTableModule();
void destroySymbolTableModule();
SymbolTableEntry* SymbolTable_get(Id id);
SymbolTableEntry* SymbolTable_getValidated(Id id, VariableType expectedType, const char* functionName);
bool SymbolTable_has(Id id);
bool SymbolTable_putGrammar(Id id, GrammarDefinition* grammar);
bool SymbolTable_putLanguage(Id id, LanguageExpression* expr);
bool SymbolTable_putProductionSet(Id id, ProductionSet set);
bool SymbolTable_putSymbolSet(Id id, SymbolSet expr);

#endif
