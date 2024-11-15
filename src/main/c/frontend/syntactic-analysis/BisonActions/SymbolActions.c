#include "SymbolActions.h"
#include "../../../shared/Set.h"
#include "../../../shared/SetElement.h"
#include "../../../shared/utils.h"
#include "../ASTUtils/SymbolUtils.h"
#include "../AbstractSyntaxTree.h"
#include "ActionsLogger.h"
#include <stdlib.h>

SymbolSetBinding* SymbolSetBinding_new(Id setId, SymbolSet symbols) {
  logSyntacticAnalyzerAction(__func__);
  SymbolSetBinding* symbolSetBinding = safeMalloc(sizeof(SymbolSetBinding));
  symbolSetBinding->id = setId;
  symbolSetBinding->symbols = symbols;

  return symbolSetBinding;
}

SymbolSet SymbolSet_new(Symbol symbol) {
  logSyntacticAnalyzerAction(__func__);
  SymbolSet set = Set_new(Symbol_hashEle, Symbol_equalsEle, Symbol_freeEle, Symbol_toStringEle);
  SymbolSet_add(set, symbol);

  return set;
}

SymbolSet SymbolSet_add(SymbolSet array, Symbol symbol) {
  logSyntacticAnalyzerPushAction(__func__, "Symbol(%s)", symbol);
  SetElement ele = {.symbol = symbol};
  Set_add(array, ele);
  return array;
}

SymbolSet SymbolSet_union(SymbolSet left, SymbolSet right) {
  char* leftStr = Set_toString(left);
  char* rightStr = Set_toString(right);
  logSyntacticAnalyzerPushAction(__func__, "SymbolSet(%s) ∪ SymbolSet(%s)", leftStr, rightStr);
  free(leftStr);
  free(rightStr);
  Set_union(left, right);
  return left;
}

SymbolSet SymbolSet_intersection(SymbolSet left, SymbolSet right) {
  char* leftStr = Set_toString(left);
  char* rightStr = Set_toString(right);
  logSyntacticAnalyzerPushAction(__func__, "SymbolSet(%s) n SymbolSet(%s)", leftStr, rightStr);
  free(leftStr);
  free(rightStr);
  Set_intersection(left, right);
  Set_free(right);
  return left;
}

SymbolSet SymbolSet_subtraction(SymbolSet left, SymbolSet right) {
  char* leftStr = Set_toString(left);
  char* rightStr = Set_toString(right);
  logSyntacticAnalyzerPushAction(__func__, "SymbolSet(%s) - SymbolSet(%s)", leftStr, rightStr);
  free(leftStr);
  free(rightStr);
  Set_subtraction(left, right);
  Set_free(right);
  return left;
}
