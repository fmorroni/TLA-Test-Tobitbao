#include "Grammar.h"
// #include "../../frontend/syntactic-analysis/ASTUtils/ProductionUtils.h"
// #include "../../frontend/syntactic-analysis/ASTUtils/SymbolUtils.h"
// #include "../../frontend/syntactic-analysis/BisonActions/ProductionActions.h"
// #include "../../frontend/syntactic-analysis/BisonActions/ProductionRhsActions.h"
// #include "../../frontend/syntactic-analysis/ASTUtils/SymbolUtils.h"
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
// #include "../../shared/Set.h"
// #include "../../shared/SetElement.h"
#include "../../shared/utils.h"
#include "../symbol-table/SymbolTable.h"
#include <stdlib.h>
#include <string.h>

Grammar* Grammar_new(GrammarDefinition* grammarDefinition) {
  Grammar* grammar = safeMalloc(sizeof(Grammar));
  // The gets should never fail because if the grammar was in the sentences array then it's also in the symbol table.
  grammar->terminalSet = SymbolTable_get(grammarDefinition->terminalSetId)->symbolSet;
  grammar->nonTerminalSet = SymbolTable_get(grammarDefinition->nonTerminalSetId)->symbolSet;
  grammar->productionSet = SymbolTable_get(grammarDefinition->productionSetId)->productionSet;
  grammar->initialSymbol = grammarDefinition->initialSymbol;
  grammar->validated = false;
  return grammar;
}

void Grammar_free(Grammar* grammar) {
  // if (grammar->normalized) {
  //   // Set_free(grammar->terminalSet);
  //   Set_free(grammar->nonTerminalSet);
  //   Set_free(grammar->productionSet);
  // }
  free(grammar);
}

// void Grammar_toLinearRight(Grammar* grammar) {
//   if (!grammar->validated || grammar->linearRight) return;
//   Symbol newInitialSymbol = (Symbol){.symbol = strdup("<S0>"), .length = 4};
//   // Set_add(grammar->nonTerminalSet, (SetElement){.symbol = newInitialSymbol});
//   // Adds `S0 -> S`
//   // Production* initialProdWrapper =
//   //   Production_new(newInitialSymbol,
//   ProductionRhsRuleSet_new(ProductionRhsRuleSymbol_new(grammar->initialSymbol)));
//   // Set_add(grammar->productionSet, (SetElement){.production = initialProdWrapper});
//   // grammar->initialSymbol = newInitialSymbol;

//   ProductionSet linearRightProductionSet = Set_cloneEmpty(grammar->productionSet);
//   Production* initialProdWrapper =
//     Production_new(grammar->initialSymbol, ProductionRhsRuleSet_new(ProductionRhsRuleLambda_new()));
//   Set_add(linearRightProductionSet, (SetElement){.production = initialProdWrapper});

//   SetIterator productionsIter = SetIterator_new(grammar->productionSet);
//   while (SetIterator_hasNext(productionsIter)) {
//     Production* prod = SetIterator_next(productionsIter)->production;
//     char* prodStr = Production_toString(prod);

//     SetIterator rhsIter = SetIterator_new(prod->rhs);
//     while (SetIterator_hasNext(rhsIter)) {
//       ProductionRhsRule* rule = SetIterator_next(rhsIter)->productionRhsRule;
//       if (Set_has(grammar->nonTerminalSet, (SetElement){.symbol = prod->lhs})) {
//         switch (rule->type) {
//           case:
//         }
//       }
//     }
//   }
// }
