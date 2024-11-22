#include "SymbolUtils.h"
#include "../../../shared/ColorMacros.h"
#include "../../../shared/Logger.h"
#include "../../../shared/Set.h"
#include "../../../shared/SetElement.h"
#include "../../../shared/String.h"
#include "../../../shared/hashUtils.h"
#include "../../../shared/utils.h"
#include "../AbstractSyntaxTree.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static Logger* _logger = NULL;

void initializeSymbolUtilsModule() {
  _logger = createLogger("SymbolUtils");
}

void shutdownSymbolUtilsModule() {
  if (_logger != NULL) {
    destroyLogger(_logger);
  }
}

uint32_t Symbol_hash(Symbol symbol) {
  return murmurHash3(1, symbol.symbol, symbol.length);
}

uint32_t Symbol_hashEle(SetElement ele) {
  return Symbol_hash(ele.symbol);
}

bool Symbol_equals(Symbol symbol1, Symbol symbol2) {
  return symbol1.length == symbol2.length && strcmp(symbol1.symbol, symbol2.symbol) == 0;
}

bool Symbol_equalsEle(SetElement ele1, SetElement ele2) {
  return Symbol_equals(ele1.symbol, ele2.symbol);
}

Symbol Symbol_clone(Symbol symbol) {
  char* clone = strndup(symbol.symbol, symbol.length);
  if (clone == NULL) exitWithPerror(__func__, "malloc error");
  return (Symbol){.symbol = clone, .length = symbol.length};
}

SetElement Symbol_cloneEle(SetElement ele) {
  return (SetElement){.symbol = Symbol_clone(ele.symbol)};
}

void Symbol_freeEle(SetElement ele) {
  logDebugging(_logger, "Executing destructor: %s(" COLORIZE_SYMBOL("%s") ")", __func__, ele.symbol.symbol);
  free(ele.symbol.symbol);
}

char* Symbol_toString(Symbol symbol) {
  char* str = safeAsprintf(COLORIZE_SYMBOL("%s"), symbol.symbol);
  return str;
}

char* Symbol_toStringEle(SetElement ele) {
  return Symbol_toString(ele.symbol);
}

void SymbolSetBinding_free(SymbolSetBinding* symbolSetBinding) {
  logDebugging(_logger, "Executing destructor: %s(" COLORIZE_ID("%s") ")", __func__, symbolSetBinding->id.id);
  Set_free(symbolSetBinding->set);
  free(symbolSetBinding->id.id);
  free(symbolSetBinding);
}

char* SymbolSetBinding_toString(SymbolSetBinding* symbolSetBinding) {
  char* symbols = Set_toString(symbolSetBinding->set);
  char* str =
    safeAsprintf("SymbolSetBinding{ id: " COLORIZE_ID("%s") ", expression: %s }", symbolSetBinding->id.id, symbols);
  free(symbols);
  return str;
}
