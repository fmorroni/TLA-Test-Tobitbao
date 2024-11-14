#include "ProductionRhsUtils.h"
#include "../../../shared/Logger.h"
#include "../../../shared/SetElement.h"
#include "../../../shared/String.h"
#include "../../../shared/hashUtils.h"
#include "../AbstractSyntaxTree.h"
#include "ColorMacros.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static Logger* _logger = NULL;

void initializeProductionRhsUtilsModule() {
  _logger = createLogger("ProductionRhsUtils");
}

void shutdownProductionRhsUtilsModule() {
  if (_logger != NULL) {
    destroyLogger(_logger);
  }
}

uint32_t ProductionRhsRule_hash(ProductionRhsRule* rule) {
  if (rule == NULL) return 0;
  switch (rule->type) {
  case SYMBOL_SYMBOL_T:
    return murmurHash3(
      3, &rule->type, sizeof(ProductionRhsRuleType), rule->leftSymbol.symbol, rule->leftSymbol.length,
      rule->rightSymbol.symbol, rule->rightSymbol.length
    );
    break;
  case SYMBOL_T:
    return murmurHash3(2, &rule->type, sizeof(ProductionRhsRuleType), rule->symbol.symbol, rule->symbol.length);
    break;
  case LAMBDA_T:
    return murmurHash3(1, &rule->type, sizeof(ProductionRhsRuleType));
    break;
  }
}

uint32_t ProductionRhsRule_hashEle(SetElement ele) {
  return ProductionRhsRule_hash(ele.productionRhsRule);
}

bool ProductionRhsRule_equals(ProductionRhsRule* rule1, ProductionRhsRule* rule2) {
  if (rule1 == rule2) return true;
  if (rule1 == NULL || rule2 == NULL) return false;

  bool cmp = rule1->type == rule2->type;

  if (cmp) {
    switch (rule1->type) {
    case SYMBOL_SYMBOL_T:
      cmp = strcmp(rule1->leftSymbol.symbol, rule2->leftSymbol.symbol) == 0;
      if (cmp) cmp = strcmp(rule1->rightSymbol.symbol, rule2->rightSymbol.symbol) == 0;
      break;
    case SYMBOL_T:
      cmp = strcmp(rule1->symbol.symbol, rule2->symbol.symbol) == 0;
      break;
    case LAMBDA_T:
      break;
    }
  }
  return cmp;
}

bool ProductionRhsRule_equalsEle(SetElement ele1, SetElement ele2) {
  return ProductionRhsRule_equals(ele1.productionRhsRule, ele2.productionRhsRule);
}

void ProductionRhsRule_free(ProductionRhsRule* rule) {
  char* str = ProductionRhsRule_toString(rule);
  logDebugging(_logger, "Executing destructor: %s(%s)", __func__, str);
  free(str);
  switch (rule->type) {
  case SYMBOL_SYMBOL_T:
    free(rule->leftSymbol.symbol);
    free(rule->rightSymbol.symbol);
    break;
  case SYMBOL_T:
    free(rule->symbol.symbol);
    break;
  case LAMBDA_T:
    break;
  }
  free(rule);
}

void ProductionRhsRule_freeEle(SetElement ele) {
  ProductionRhsRule_free(ele.productionRhsRule);
}

char* ProductionRhsRule_toString(ProductionRhsRule* productionRhsRule) {
  char* str;
  switch (productionRhsRule->type) {
  case SYMBOL_SYMBOL_T:
    str = safeAsprintf(
      "[ " COLORIZE_SYMBOL("%s") ", " COLORIZE_SYMBOL("%s") " ]", productionRhsRule->leftSymbol.symbol,
      productionRhsRule->rightSymbol
    );
    break;
  case SYMBOL_T:
    str = safeAsprintf("[ " COLORIZE_SYMBOL("%s") " ]", productionRhsRule->symbol.symbol);
    break;
  case LAMBDA_T:
    str = safeAsprintf("[ " COLORIZE_SYMBOL("󰘧") " ]");
    break;
  }
  return str;
}

char* ProductionRhsRule_toStringEle(SetElement ele) {
  return ProductionRhsRule_toString(ele.productionRhsRule);
}
