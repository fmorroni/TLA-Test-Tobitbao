#include "../../../shared/ColorMacros.h"
#include "../../../shared/Logger.h"
#include "../../../shared/Set.h"
#include "../../../shared/SetElement.h"
#include "../../../shared/String.h"
#include "../../../shared/hashUtils.h"
#include "../../../shared/utils.h"
#include "../AbstractSyntaxTree.h"
#include "SymbolUtils.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static Logger* _logger = NULL;

void initializeProductionUtilsModule() {
  _logger = createLogger("ProductionUtils");
}

void shutdownProductionUtilsModule() {
  if (_logger != NULL) {
    destroyLogger(_logger);
  }
}

uint32_t Production_hash(Production* production) {
  if (production == NULL) return 0;
  return murmurHash3(1, production->lhs.symbol, production->lhs.length);
}

uint32_t Production_hashEle(SetElement ele) {
  return Production_hash(ele.production);
}

bool Production_equals(Production* prod1, Production* prod2) {
  if (prod1 == prod2) return true;
  if (prod1 == NULL || prod2 == NULL) return false;
  return strcmp(prod1->lhs.symbol, prod2->lhs.symbol) == 0;
}

bool Production_equalsEle(SetElement ele1, SetElement ele2) {
  return Production_equals(ele1.production, ele2.production);
}

SetElement Production_cloneEle(SetElement ele) {
  Production* clone = safeMalloc(sizeof(Production));
  clone->lhs = Symbol_clone(ele.production->lhs);
  clone->rhs = Set_clone(ele.production->rhs);
  SetElement eleClone = {.production = clone};
  return eleClone;
}

void Production_freeEle(SetElement ele) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  free(ele.production->lhs.symbol);
  Set_free(ele.production->rhs);
  free(ele.production);
}

char* Production_toString(Production* production) {
  char* rhs = Set_toString(production->rhs);
  char* str = safeAsprintf(COLORIZE_SYMBOL("%s") " -> %s", production->lhs.symbol, rhs);
  free(rhs);
  return str;
}

char* Production_toStringEle(SetElement ele) {
  return Production_toString(ele.production);
}

void ProductionSetBinding_free(ProductionSetBinding* productionSetBinding) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  Set_free(productionSetBinding->productions);
  free(productionSetBinding->id.id);
  free(productionSetBinding);
}

char* ProductionSetBinding_toString(ProductionSetBinding* productionSetBinding) {
  char* productions = Set_toString(productionSetBinding->productions);
  char* str = safeAsprintf(
    "ProductionSetBinding{ id: " COLORIZE_ID("%s") ", productions: %s }", productionSetBinding->id.id, productions
  );
  free(productions);
  return str;
}
