#include "../../../shared/Logger.h"
#include "../../../shared/Set.h"
#include "../../../shared/SetElement.h"
#include "../../../shared/String.h"
#include "../../../shared/hashUtils.h"
#include "../AbstractSyntaxTree.h"
#include "ColorMacros.h"
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

void Symbol_freeEle(SetElement ele) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
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
  logDebugging(_logger, "Executing destructor: %s", __func__);
  Set_free(symbolSetBinding->symbols);
  free(symbolSetBinding->id.id);
  free(symbolSetBinding);
}

char* SymbolSetBinding_toString(SymbolSetBinding* symbolSetBinding) {
  char* symbols = Set_toString(symbolSetBinding->symbols);
  char* str =
    safeAsprintf("SymbolSetBinding{ id: " COLORIZE_ID("%s") ", symbols: %s }", symbolSetBinding->id.id, symbols);
  free(symbols);
  return str;
}
