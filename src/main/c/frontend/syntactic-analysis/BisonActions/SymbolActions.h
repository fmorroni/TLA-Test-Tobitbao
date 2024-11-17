#ifndef SYMBOL_ACTIONS_H
#define SYMBOL_ACTIONS_H

#include "../AbstractSyntaxTree.h"

SymbolSetBinding* SymbolSetBinding_new(Id setId, SymbolSet set);

SymbolSet SymbolSet_new(Symbol symbol);
SymbolSet SymbolSet_add(SymbolSet array, Symbol symbol);
SymbolSet SymbolSet_clone(Id id);

SymbolSet SymbolSet_union(SymbolSet left, SymbolSet right);
SymbolSet SymbolSet_intersection(SymbolSet left, SymbolSet right);
SymbolSet SymbolSet_subtraction(SymbolSet left, SymbolSet right);

#endif
