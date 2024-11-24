#ifndef SYMBOL_UTILS_H
#define SYMBOL_UTILS_H

#include "../../../shared/SetElement.h"
#include "../AbstractSyntaxTree.h"
#include <stdbool.h>
#include <stdint.h>

void initializeSymbolUtilsModule();
void shutdownSymbolUtilsModule();

uint32_t Symbol_hashEle(SetElement ele);
bool Symbol_equals(Symbol symbol1, Symbol symbol2);
bool Symbol_equalsEle(SetElement ele1, SetElement ele2);
Symbol Symbol_clone(Symbol symbol);
SetElement Symbol_cloneEle(SetElement ele);
void Symbol_freeEle(SetElement ele);
char* Symbol_toStringEle(SetElement ele);

void SymbolSetBinding_free(SymbolSetBinding* symbolSetBinding);
char* SymbolSetBinding_toString(SymbolSetBinding* symbolSetBinding);

#endif
