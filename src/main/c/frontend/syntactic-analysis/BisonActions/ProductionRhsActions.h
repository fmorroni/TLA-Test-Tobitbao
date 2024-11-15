#ifndef PRODUCTION_RHS_ACTIONS_H
#define PRODUCTION_RHS_ACTIONS_H

#include "../AbstractSyntaxTree.h"

ProductionRhsRuleSet ProductionRhsRuleSet_new(ProductionRhsRule* productionRhsRule);
ProductionRhsRuleSet ProductionRhsRuleSet_add(ProductionRhsRuleSet set, ProductionRhsRule* rule);
ProductionRhsRule* ProductionRhsRuleSymbolSymbol_new(Symbol leftSymbol, Symbol rightSymbol);
ProductionRhsRule* ProductionRhsRuleSymbol_new(Symbol symbol);
ProductionRhsRule* ProductionRhsRuleLambda_new();

#endif
