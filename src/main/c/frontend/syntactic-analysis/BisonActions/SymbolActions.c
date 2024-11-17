#include "SymbolActions.h"
#include "../../../backend/symbol-table/SymbolTable.h"
#include "../../../shared/Set.h"
#include "../../../shared/SetElement.h"
#include "../../../shared/utils.h"
#include "../ASTUtils/SymbolUtils.h"
#include "../AbstractSyntaxTree.h"
#include "ActionsLogger.h"
#include <stdlib.h>

SymbolSetBinding* SymbolSetBinding_new(Id setId, SymbolSet set) {
  logSyntacticAnalyzerAction(__func__);
  SymbolSetBinding* symbolSetBinding = safeMalloc(sizeof(SymbolSetBinding));
  symbolSetBinding->id = setId;
  symbolSetBinding->set = set;

  if (!SymbolTable_putSymbolSet(setId, set)) {
    logAlreadyDefinedError(__func__, setId.id);
    // TODO: Change this... Prolly the best is to push all errors to a list and if the list isn't
    // empty at the end we exit with an error code.
    exit(1);
  }

  return symbolSetBinding;
}

SymbolSet SymbolSet_new(Symbol symbol) {
  logSyntacticAnalyzerAction(__func__);
  SymbolSet set = Set_new(Symbol_hashEle, Symbol_equalsEle, Symbol_cloneEle, Symbol_freeEle, Symbol_toStringEle);
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

SymbolSet SymbolSet_clone(Id id) {
  SymbolTableEntry* entry = SymbolTable_get(id);
  if (entry == NULL) {
    logUndefined(__func__, id.id);
    return NULL;
  } else if (entry->type != SYMBOL_SET_T) {
    logInvalidType(__func__, id.id, VariableType_toString(SYMBOL_SET_T), VariableType_toString(entry->type));
    return NULL;
  }
  free(id.id);
  Set set = Set_clone(entry->set);
  return set;
}
