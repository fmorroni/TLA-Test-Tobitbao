#ifndef PRODUCTION_ACTIONS_H
#define PRODUCTION_ACTIONS_H

#include "../AbstractSyntaxTree.h"

ProductionSetBinding* ProductionSetBinding_new(Id setId, ProductionSet productions);
ProductionSet ProductionSet_new(Production* production);
ProductionSet ProductionSet_add(ProductionSet set, Production* production);
Production* Production_new(Symbol lhs, ProductionRhsRuleSet productionRhsRules);
ProductionSet ProductionSet_union(ProductionSet left, ProductionSet right);
ProductionSet ProductionSet_intersection(ProductionSet left, ProductionSet right);
ProductionSet ProductionSet_subtraction(ProductionSet left, ProductionSet right);

#endif
