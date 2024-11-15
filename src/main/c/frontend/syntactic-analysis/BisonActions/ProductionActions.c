#include "ProductionActions.h"
#include "../../../shared/Set.h"
#include "../../../shared/SetElement.h"
#include "../../../shared/utils.h"
#include "../ASTUtils/ProductionUtils.h"
#include "../AbstractSyntaxTree.h"
#include "ActionsLogger.h"
#include <stdlib.h>

ProductionSetBinding* ProductionSetBinding_new(Id setId, ProductionSet productions) {
  logSyntacticAnalyzerAction(__func__);
  ProductionSetBinding* productionSetBinding = safeMalloc(sizeof(ProductionSetBinding));
  productionSetBinding->id = setId;
  productionSetBinding->productions = productions;

  return productionSetBinding;
}

ProductionSet ProductionSet_new(Production* production) {
  logSyntacticAnalyzerAction(__func__);
  ProductionSet set = Set_new(Production_hashEle, Production_equalsEle, Production_freeEle, Production_toStringEle);
  ProductionSet_add(set, production);
  return set;
}

ProductionSet ProductionSet_add(ProductionSet set, Production* production) {
  char* str = Production_toString(production);
  logSyntacticAnalyzerPushAction(__func__, "Production(%s)", str);
  free(str);
  SetElement ele = {.production = production};
  SetElement* foundEle = Set_find(set, ele);
  if (foundEle == NULL) Set_add(set, ele);
  else {
    Set_union(foundEle->production->rhs, production->rhs);
    free(production->lhs.symbol);
    free(production);
  }
  return set;
}

ProductionSet ProductionSet_remove(ProductionSet set, Production* production) {
  char* str = Production_toString(production);
  logSyntacticAnalyzerPushAction(__func__, "Production(%s)", str);
  free(str);
  SetElement ele = {.production = production};
  SetElement* foundEle = Set_find(set, ele);
  if (foundEle != NULL) {
    Set_subtraction(foundEle->production->rhs, production->rhs);
    if (Set_isEmpty(foundEle->production->rhs)) {
      Set_remove(set, *foundEle);
    }
  }
  return set;
}

bool ProductionSet_has(ProductionSet set, Production* production) {
  SetElement ele = {.production = production};
  return Set_Has(set, ele);
}

Production* Production_new(Symbol lhs, ProductionRhsRuleSet productionRhsRules) {
  logSyntacticAnalyzerAction(__func__);
  Production* production = safeMalloc(sizeof(Production));
  production->lhs = lhs;
  production->rhs = productionRhsRules;

  return production;
}

ProductionSet ProductionSet_union(ProductionSet left, ProductionSet right) {
  char* leftStr = Set_toString(left);
  char* rightStr = Set_toString(right);
  logSyntacticAnalyzerPushAction(__func__, "ProductionSet(%s) ∪ ProductionSet(%s)", leftStr, rightStr);
  free(leftStr);
  free(rightStr);
  SetIterator rightIter = SetIterator_new(right);
  while (SetIterator_hasNext(rightIter)) {
    Production* prod = SetIterator_next(rightIter)->production;
    ProductionSet_add(left, prod);
  }
  SetIterator_free(rightIter);
  Set_freeNotElements(right);
  return left;
}

ProductionSet ProductionSet_intersection(ProductionSet left, ProductionSet right) {
  char* leftStr = Set_toString(left);
  char* rightStr = Set_toString(right);
  logSyntacticAnalyzerPushAction(__func__, "ProductionSet(%s) n ProductionSet(%s)", leftStr, rightStr);
  free(leftStr);
  free(rightStr);
  SetIterator leftIter = SetIterator_new(left);
  while (SetIterator_hasNext(leftIter)) {
    Production* leftProd = SetIterator_next(leftIter)->production;
    SetElement ele = {.production = leftProd};
    SetElement* foundEle = Set_find(right, ele);
    if (foundEle == NULL) {
      SetElement ele = {.production = leftProd};
      Set_remove(left, ele);
    } else {
      Set_intersection(leftProd->rhs, foundEle->production->rhs);
    }
  }
  SetIterator_free(leftIter);
  Set_free(right);
  return left;
}

ProductionSet ProductionSet_subtraction(ProductionSet left, ProductionSet right) {
  char* leftStr = Set_toString(left);
  char* rightStr = Set_toString(right);
  logSyntacticAnalyzerPushAction(__func__, "ProductionSet(%s) n ProductionSet(%s)", leftStr, rightStr);
  free(leftStr);
  free(rightStr);
  SetIterator rightIter = SetIterator_new(right);
  while (SetIterator_hasNext(rightIter)) {
    Production* prod = SetIterator_next(rightIter)->production;
    ProductionSet_remove(left, prod);
  }
  SetIterator_free(rightIter);
  Set_free(right);
  return left;
}
