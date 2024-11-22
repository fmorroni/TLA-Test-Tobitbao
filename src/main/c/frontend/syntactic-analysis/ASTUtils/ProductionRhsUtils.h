#ifndef PRODUCTION_RHS_UTILS_H
#define PRODUCTION_RHS_UTILS_H

#include "../../../shared/SetElement.h"
#include "../AbstractSyntaxTree.h"
#include <stdbool.h>
#include <stdint.h>

void initializeProductionRhsUtilsModule();
void shutdownProductionRhsUtilsModule();

uint32_t ProductionRhsRule_hashEle(SetElement ele);
bool ProductionRhsRule_equalsEle(SetElement ele1, SetElement ele2);
SetElement ProductionRhsRule_cloneEle(SetElement ele);
void ProductionRhsRule_freeEle(SetElement ele);
char* ProductionRhsRule_toString(ProductionRhsRule* productionRhsRule);
char* ProductionRhsRule_toStringEle(SetElement ele);

#endif
