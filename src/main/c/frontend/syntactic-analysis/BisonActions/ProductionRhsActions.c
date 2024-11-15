#include "ProductionRhsActions.h"
#include "../../../shared/Set.h"
#include "../../../shared/SetElement.h"
#include "../../../shared/utils.h"
#include "../ASTUtils/ProductionRhsUtils.h"
#include "../AbstractSyntaxTree.h"
#include "ActionsLogger.h"
#include <stdlib.h>

ProductionRhsRuleSet ProductionRhsRuleSet_new(ProductionRhsRule* productionRhsRule) {
  logSyntacticAnalyzerAction(__func__);
  ProductionRhsRuleSet set = Set_new(
    ProductionRhsRule_hashEle, ProductionRhsRule_equalsEle, ProductionRhsRule_freeEle, ProductionRhsRule_toStringEle
  );
  ProductionRhsRuleSet_add(set, productionRhsRule);

  return set;
}

ProductionRhsRuleSet ProductionRhsRuleSet_add(ProductionRhsRuleSet set, ProductionRhsRule* rule) {
  char* str = ProductionRhsRule_toString(rule);
  logSyntacticAnalyzerPushAction(__func__, "ProductionRhsRule%s", str);
  free(str);
  SetElement ele = {.productionRhsRule = rule};
  Set_add(set, ele);
  return set;
}

ProductionRhsRule* ProductionRhsRuleSymbolSymbol_new(Symbol leftSymbol, Symbol rightSymbol) {
  ProductionRhsRule* rule = safeMalloc(sizeof(ProductionRhsRule));
  rule->type = SYMBOL_SYMBOL_T;
  rule->leftSymbol = leftSymbol;
  rule->rightSymbol = rightSymbol;
  return rule;
}

ProductionRhsRule* ProductionRhsRuleSymbol_new(Symbol symbol) {
  ProductionRhsRule* rule = safeMalloc(sizeof(ProductionRhsRule));
  rule->type = SYMBOL_T;
  rule->symbol = symbol;
  return rule;
}

ProductionRhsRule* ProductionRhsRuleLambda_new() {
  ProductionRhsRule* rule = safeMalloc(sizeof(ProductionRhsRule));
  rule->type = LAMBDA_T;
  return rule;
}
