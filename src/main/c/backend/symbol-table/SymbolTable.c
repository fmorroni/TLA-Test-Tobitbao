#include "SymbolTable.h"
#include "../../backend/domain-specific/DFA.h"
#include "../../backend/domain-specific/Grammar.h"
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/ColorMacros.h"
#include "../../shared/Logger.h"
#include "../../shared/Set.h"
#include "../../shared/SetElement.h"
#include "../../shared/hashUtils.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ADD_TO_SYMBOL_TABLE_FMT_STR(type) "Adding " type " " COLORIZE_ID("%s") " to symbol table."

static Logger* _logger = NULL;
static Set symbolTable = NULL;
static Set grammarTable = NULL;
static Set dfaTable = NULL;

uint32_t SymbolTable_hashEleFn(SetElement ele);
bool SymbolTable_equalsEleFn(SetElement ele1, SetElement ele2);
void GrammarTable_freeEleFn(SetElement ele);
void DfaTable_freeEleFn(SetElement ele);

void initializeSymbolTableModule() {
  _logger = createLogger("SymbolTableLogger");
  symbolTable = Set_new(SymbolTable_hashEleFn, SymbolTable_equalsEleFn, NULL, NULL, NULL);
  grammarTable = Set_new(SymbolTable_hashEleFn, SymbolTable_equalsEleFn, NULL, GrammarTable_freeEleFn, NULL);
  dfaTable = Set_new(SymbolTable_hashEleFn, SymbolTable_equalsEleFn, NULL, DfaTable_freeEleFn, NULL);
}

void shutdownSymbolTableModule() {
  logDebugging(_logger, "Destroying symbol table");
  destroyLogger(_logger);
  Set_free(symbolTable);
  Set_free(grammarTable);
  Set_free(dfaTable);
}

SymbolTableEntry* SymbolTable_get(Id id) {
  SymbolTableEntry entry = {.id = id};
  SetElement ele = {.symbolTableEntry = entry};
  return &Set_find(symbolTable, ele)->symbolTableEntry;
}

bool SymbolTable_has(Id id) {
  SymbolTableEntry entry = {.id = id};
  SetElement ele = {.symbolTableEntry = entry};
  return Set_has(symbolTable, ele);
}

bool SymbolTable_put(SetElement ele) {
  return Set_add(symbolTable, ele);
}

bool SymbolTable_putGrammarDefinition(Id id, GrammarDefinition* grammar) {
  logDebugging(_logger, ADD_TO_SYMBOL_TABLE_FMT_STR("grammar"), id.id);
  SymbolTableEntry entry = {.id = id, .type = GRAMMAR_DEFINITION_T, .grammarDefinition = grammar};
  SetElement ele = {.symbolTableEntry = entry};
  return SymbolTable_put(ele);
}

bool SymbolTable_putLanguage(Id id, LanguageExpression* expr) {
  logDebugging(_logger, ADD_TO_SYMBOL_TABLE_FMT_STR("language"), id.id);
  SymbolTableEntry entry = {.id = id, .type = LANGUAGE_T, .langExpression = expr};
  SetElement ele = {.symbolTableEntry = entry};
  return SymbolTable_put(ele);
}

bool SymbolTable_putProductionSet(Id id, ProductionSet set) {
  logDebugging(_logger, ADD_TO_SYMBOL_TABLE_FMT_STR("production set"), id.id);
  SymbolTableEntry entry = {.id = id, .type = PRODUCTION_SET_T, .productionSet = set};
  SetElement ele = {.symbolTableEntry = entry};
  return SymbolTable_put(ele);
}

bool SymbolTable_putSymbolSet(Id id, SymbolSet set) {
  logDebugging(_logger, ADD_TO_SYMBOL_TABLE_FMT_STR("symbol set"), id.id);
  SymbolTableEntry entry = {.id = id, .type = SYMBOL_SET_T, .symbolSet = set};
  SetElement ele = {.symbolTableEntry = entry};
  return SymbolTable_put(ele);
}

bool GrammarTable_put(Id id, Grammar* grammar) {
  SymbolTableEntry entry = {.id = id, .grammar = grammar};
  SetElement ele = {.symbolTableEntry = entry};
  return Set_add(grammarTable, ele);
}

Grammar* GrammarTable_get(Id id) {
  SymbolTableEntry entry = {.id = id};
  SetElement ele = {.symbolTableEntry = entry};
  return Set_find(grammarTable, ele)->symbolTableEntry.grammar;
}

DFA* DfaTable_get(Id id) {
  SymbolTableEntry entry = {.id = id};
  SetElement ele = {.symbolTableEntry = entry};
  return Set_find(dfaTable, ele)->symbolTableEntry.dfa;
}

bool DfaTable_put(Id id, DFA* dfa) {
  SymbolTableEntry entry = {.id = id, .dfa = dfa};
  SetElement ele = {.symbolTableEntry = entry};
  return Set_add(dfaTable, ele);
}

// Utils

uint32_t SymbolTable_hashEleFn(SetElement ele) {
  return murmurHash3(1, ele.symbolTableEntry.id.id, ele.symbolTableEntry.id.length);
}

bool SymbolTable_equalsEleFn(SetElement ele1, SetElement ele2) {
  Id id1 = ele1.symbolTableEntry.id;
  Id id2 = ele2.symbolTableEntry.id;
  return id1.length == id2.length && strcmp(id1.id, id2.id) == 0;
}

void GrammarTable_freeEleFn(SetElement ele) {
  Grammar_free(ele.symbolTableEntry.grammar);
}

void DfaTable_freeEleFn(SetElement ele) {
  DFA_free(ele.symbolTableEntry.dfa);
}

SetIterator DfaTableIterator_new() {
  return SetIterator_new(dfaTable);
}

void DfaTableIterator_free(SetIterator iter) {
  SetIterator_free(iter);
}

bool DfaTableIterator_hasNext(SetIterator iter) {
  return SetIterator_hasNext(iter);
}

DFA* DfaTableIterator_next(SetIterator iter) {
  return SetIterator_next(iter)->symbolTableEntry.dfa;
}
