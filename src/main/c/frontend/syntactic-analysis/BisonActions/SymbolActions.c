#include "SymbolActions.h"
#include "../../../backend/errors/Errors.h"
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

  if (Set_isEmpty(set)) Error_emptySetDefinition(__func__, setId.id);

  return symbolSetBinding;
}

void SymbolSetBinding_initialize(Id setId, SymbolSet set) {
  if (!SymbolTable_putSymbolSet(setId, set)) {
    Error_alreadyDefined(__func__, setId.id);
    return;
  }
}

void SymbolSetBinding_assign(Id setId, SymbolSet set) {
  // Note `entry` should never be NULL because to get here an ID_SYM must've been found and that only
  // happens when flex finds the id in the symbol table.
  SymbolTableEntry* entry = SymbolTable_get(setId);
  if (entry->symbolSet != NULL) {
    Error_alreadyDefined(__func__, setId.id);
    return;
  }
  entry->symbolSet = set;
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
  // Note: at this point `entry` will never be NULL because to get here flex must've generated an `ID_PROD`
  // and for that it must've found `id` in the table, otherwise it would generate an `ID` and bison would
  // fail with a sintactic error. The `entry` will also be of the correct type, as flex would otherwise have
  // generated an `ID_<type>` with `<type>` other than `PROD` which would also cause a sintactic error.
  SymbolTableEntry* entry = SymbolTable_get(id);
  free(id.id);
  SymbolSet set = Set_clone(entry->symbolSet);
  return set;
}
