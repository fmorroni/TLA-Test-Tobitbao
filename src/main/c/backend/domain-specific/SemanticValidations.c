#include "../../frontend/syntactic-analysis/ASTUtils/ProductionUtils.h"
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/Array.h"
#include "../../shared/Logger.h"
#include "../../shared/Set.h"
#include "../../shared/SetElement.h"
#include "../errors/Errors.h"
#include "../symbol-table/SymbolTable.h"
#include "Grammar.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static Logger* _logger = NULL;

void validateProductionSet(GrammarDefinition* grammarDefinition, Grammar* grammar);
void validateSymbolSymbolRhsRule(
  ProductionRhsRule* rule, GrammarDefinition* grammarDefinition, Grammar* grammar, const char* prodStr,
  bool* isLinearRight, bool* isLinearLeft
);

bool semanticValidation(Program* program) {
  size_t sentencesLen = Array_getLen(program->sentences);
  bool errors = false;
  for (int i = 0; i < sentencesLen; ++i) {
    Sentence* sentence = Array_get(program->sentences, i).sentence;
    if (sentence->type == GRAMMAR_DEFINITION) {
      GrammarDefinition* grammarDefinition = sentence->grammarDefinition;
      Grammar* grammar = Grammar_new(grammarDefinition);

      if (grammar->terminalSet == NULL) {
        Error_incompatibleType(__func__, grammarDefinition->terminalSetId.id);
        break;
      }
      if (grammar->nonTerminalSet == NULL) {
        Error_incompatibleType(__func__, grammarDefinition->nonTerminalSetId.id);
        break;
      }
      if (grammar->productionSet == NULL) {
        Error_incompatibleType(__func__, grammarDefinition->productionSetId.id);
        break;
      }

      if (!Set_hasVoidIntersection(grammar->terminalSet, grammar->nonTerminalSet)) {
        Error_terminalsAndNonTerminalsCollision(__func__, grammarDefinition);
        break;
      }

      if (!Set_has(grammar->nonTerminalSet, (SetElement){.symbol = grammarDefinition->initialSymbol})) {
        Error_undefinedInitialSymbol(__func__, grammarDefinition);
        break;
      }

      validateProductionSet(grammarDefinition, grammar);
      grammar->validated = true;
      GrammarTable_put(grammarDefinition->id, grammar);
      // DFA* dfa = DFA_fromGrammar(grammar);
      // DFA_printTransitions(dfa);
      // DfaTable_put(grammarDefinition->id, dfa);
      // Grammar_free(grammar);
    }
  }
  return !errors;
}

void validateProductionSet(GrammarDefinition* grammarDefinition, Grammar* grammar) {
  Production initialSymbolProd = {.lhs = grammarDefinition->initialSymbol};
  SetElement ele = {.production = &initialSymbolProd};
  if (!Set_has(grammar->productionSet, ele)) {
    Error_noInitialSymbolProduction(__func__, grammarDefinition);
    return;
  }
  SetIterator productionsIter = SetIterator_new(grammar->productionSet);
  bool isLinearRight = false;
  bool isLinearLeft = false;
  while (SetIterator_hasNext(productionsIter)) {
    Production* prod = SetIterator_next(productionsIter)->production;
    char* prodStr = Production_toString(prod);

    if (!Set_has(grammar->nonTerminalSet, (SetElement){.symbol = prod->lhs})) {
      Error_undefinedNonTerminalSymbolUsedInProduction(__func__, prod->lhs.symbol, prodStr, grammarDefinition);
    }

    SetIterator rhsIter = SetIterator_new(prod->rhs);
    while (SetIterator_hasNext(rhsIter)) {
      ProductionRhsRule* rule = SetIterator_next(rhsIter)->productionRhsRule;
      switch (rule->type) {
      case SYMBOL_SYMBOL_T:
        validateSymbolSymbolRhsRule(rule, grammarDefinition, grammar, prodStr, &isLinearRight, &isLinearLeft);
        break;
      case SYMBOL_T:
        if (!Set_has(grammar->terminalSet, (SetElement){.symbol = rule->symbol})) {
          Error_undefinedTerminalSymbolUsedInProduction(__func__, rule->symbol.symbol, prodStr, grammarDefinition);
        }
        break;
      case LAMBDA_T:
        break;
      }
    }
    SetIterator_free(rhsIter);
    free(prodStr);
  }
  SetIterator_free(productionsIter);
  if (isLinearRight && !isLinearLeft) grammar->linearRight = true;
  else if (!isLinearRight && isLinearLeft) Error_linearLeftProductionSet(__func__, grammarDefinition);
  else if (isLinearRight && isLinearLeft) Error_linearRightAndLeftProductionMix(__func__, grammarDefinition);
}

void validateSymbolSymbolRhsRule(
  ProductionRhsRule* rule, GrammarDefinition* grammarDefinition, Grammar* grammar, const char* prodStr,
  bool* isLinearRight, bool* isLinearLeft
) {
  SetElement leftSymbolEle = (SetElement){.symbol = rule->leftSymbol};
  SetElement rightSymbolEle = (SetElement){.symbol = rule->rightSymbol};
  bool leftIsTerminal = Set_has(grammar->terminalSet, leftSymbolEle);
  bool rightIsTerminal = Set_has(grammar->terminalSet, rightSymbolEle);
  if (leftIsTerminal && rightIsTerminal) {
    Error_invalidProduction(__func__, prodStr, grammarDefinition);
  } else if (leftIsTerminal && !rightIsTerminal) {
    if (!Set_has(grammar->nonTerminalSet, rightSymbolEle)) {
      Error_undefinedSymbol(__func__, rule->rightSymbol.symbol, prodStr, grammarDefinition);
    } else *isLinearRight = true;
  } else if (!leftIsTerminal && rightIsTerminal) {
    if (!Set_has(grammar->nonTerminalSet, leftSymbolEle)) {
      Error_undefinedSymbol(__func__, rule->leftSymbol.symbol, prodStr, grammarDefinition);
    } else *isLinearLeft = true;
  } else {
    bool leftIsNonTerminal = Set_has(grammar->nonTerminalSet, leftSymbolEle);
    bool rightIsNonTerminal = Set_has(grammar->nonTerminalSet, rightSymbolEle);
    if (leftIsNonTerminal && rightIsNonTerminal) {
      Error_invalidProduction(__func__, prodStr, grammarDefinition);
    } else {
      if (!leftIsNonTerminal) Error_undefinedSymbol(__func__, rule->leftSymbol.symbol, prodStr, grammarDefinition);
      if (!rightIsNonTerminal) Error_undefinedSymbol(__func__, rule->rightSymbol.symbol, prodStr, grammarDefinition);
    }
  }
}

void initializeSemanticValidationModule() {
  _logger = createLogger("SemanticValidation");
}

void shutdownSemanticValidationModule() {
  destroyLogger(_logger);
}
