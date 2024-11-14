#ifndef PRODUCTION_UTILS_H
#define PRODUCTION_UTILS_H

#include "../../../shared/SetElement.h"
#include "../AbstractSyntaxTree.h"
#include <stdbool.h>
#include <stdint.h>

void initializeProductionUtilsModule();
void shutdownProductionUtilsModule();

uint32_t Production_hashEle(SetElement ele);
bool Production_equalsEle(SetElement ele1, SetElement ele2);
void Production_freeEle(SetElement ele);
char* Production_toString(Production* production);
char* Production_toStringEle(SetElement ele);

void ProductionSetBinding_free(ProductionSetBinding* productionSetBinding);
char* ProductionSetBinding_toString(ProductionSetBinding* productionSetBinding);

#endif
