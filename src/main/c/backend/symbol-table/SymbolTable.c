#include "SymbolTable.h"
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

uint32_t SymbolTable_hashEleFn(SetElement ele);
bool SymbolTable_equalsEleFn(SetElement ele1, SetElement ele2);
void SymbolTable_freeEleFn(SetElement ele);
void logUndefined(const char* functionName, const char* id);
void logInvalidType(const char* functionName, const char* id, const char* expectedType, const char* realType);

void initializeSymbolTableModule() {
  _logger = createLogger("SymbolTableLogger");
  symbolTable = Set_new(SymbolTable_hashEleFn, SymbolTable_equalsEleFn, NULL, NULL, NULL);
}

void destroySymbolTableModule() {
  logDebugging(_logger, "Destroying symbol table");
  destroyLogger(_logger);
  Set_free(symbolTable);
}

SymbolTableEntry* SymbolTable_get(Id id) {
  SymbolTableEntry entry = {.id = id};
  SetElement ele = {.symbolTableEntry = entry};
  return &Set_find(symbolTable, ele)->symbolTableEntry;
}

SymbolTableEntry* SymbolTable_getValidated(Id id, VariableType expectedType, const char* functionName) {
  SymbolTableEntry* entry = SymbolTable_get(id);
  if (entry == NULL) {
    logUndefined(functionName, id.id);
    return NULL;
  } else if (entry->type != expectedType) {
    logInvalidType(functionName, id.id, VariableType_toString(expectedType), VariableType_toString(entry->type));
    return NULL;
  }
  return entry;
}

bool SymbolTable_has(Id id) {
  SymbolTableEntry entry = {.id = id};
  SetElement ele = {.symbolTableEntry = entry};
  return Set_has(symbolTable, ele);
}

bool SymbolTable_put(SetElement ele) {
  // Nvm no elements need to be freed after all si freeEleFn is NULL anyways.
  // Check with `Set_has` first because `Set_add` frees the element if already present and
  // we don't want that in this case.
  // if (Set_has(symbolTable, ele)) return false;

  return Set_add(symbolTable, ele);
}

bool SymbolTable_putGrammar(Id id, GrammarDefinition* grammar) {
  logDebugging(_logger, ADD_TO_SYMBOL_TABLE_FMT_STR("grammar"), id.id);
  SymbolTableEntry entry = {.id = id, .type = GRAMMAR_T, .grammar = grammar};
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

// Utils

uint32_t SymbolTable_hashEleFn(SetElement ele) {
  return murmurHash3(1, ele.symbolTableEntry.id.id, ele.symbolTableEntry.id.length);
}

bool SymbolTable_equalsEleFn(SetElement ele1, SetElement ele2) {
  Id id1 = ele1.symbolTableEntry.id;
  Id id2 = ele2.symbolTableEntry.id;
  return id1.length == id2.length && strcmp(id1.id, id2.id) == 0;
}

void logUndefined(const char* functionName, const char* id) {
  logError(_logger, "%s: %s isn't defined.", functionName, id);
}

void logInvalidType(const char* functionName, const char* id, const char* expectedType, const char* realType) {
  logError(
    _logger, "%s: %s expected to be of type `%s` but was of type `%s`", functionName, id, expectedType, realType
  );
}

static char* varTypeStr[] = {
  [LANGUAGE_T] = "LANGUAGE_T",
  [SYMBOL_SET_T] = "SYMBOL_SET_T",
  [PRODUCTION_SET_T] = "PRODUCTION_SET_T",
  [GRAMMAR_T] = "GRAMMAR_T"
};

const char* VariableType_toString(VariableType type) {
  return varTypeStr[type];
}
