#ifndef PRODUCTION_ACTIONS_H
#define PRODUCTION_ACTIONS_H

#include "../AbstractSyntaxTree.h"

ProductionSetBinding* ProductionSetBinding_new(Id setId, ProductionSet productions);

Production* Production_new(Symbol lhs, ProductionRhsRuleSet productionRhsRules);

ProductionSet ProductionSet_new(Production* production);
void ProductionSetBinding_initialize(Id setId, ProductionSet set);
void ProductionSetBinding_assign(Id setId, ProductionSet set);
ProductionSet ProductionSet_add(ProductionSet set, Production* production);
ProductionSet ProductionSet_clone(Id id);

ProductionSet ProductionSet_union(ProductionSet left, ProductionSet right);
ProductionSet ProductionSet_intersection(ProductionSet left, ProductionSet right);
ProductionSet ProductionSet_subtraction(ProductionSet left, ProductionSet right);

#endif
