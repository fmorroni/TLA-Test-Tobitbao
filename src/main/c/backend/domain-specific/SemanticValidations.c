#include "../../frontend/syntactic-analysis/ASTUtils/ProductionUtils.h"
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/Array.h"
#include "../../shared/Logger.h"
#include "../../shared/Set.h"
#include "../../shared/SetElement.h"
#include "../errors/Errors.h"
#include "../symbol-table/SymbolTable.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static Logger* _logger = NULL;

void computeGrammar(GrammarDefinition* grammar);
void validateProductionSet(
  GrammarDefinition* grammar, SymbolSet terminalSet, SymbolSet nonTerminalSet, ProductionSet productionSet
);
void validateSymbolSymbolRhsRule(
  ProductionRhsRule* rule, SymbolSet terminalSet, SymbolSet nonTerminalSet, const char* prodStr,
  const char* productionSetId, bool* isLinearRight, bool* isLinearLeft
);

bool semanticValidation(Program* program) {
  size_t sentencesLen = Array_getLen(program->sentences);
  bool errors = false;
  for (int i = 0; i < sentencesLen; ++i) {
    Sentence* sentence = Array_get(program->sentences, i).sentence;
    if (sentence->type == GRAMMAR_DEFINITION) {
      GrammarDefinition* grammar = sentence->grammarDefinition;
      SymbolSet terminalSet = SymbolTable_get(grammar->terminalSetId)->symbolSet;
      char* terminalSetId = grammar->terminalSetId.id;
      SymbolSet nonTerminalSet = SymbolTable_get(grammar->nonTerminalSetId)->symbolSet;
      char* nonTerminalSetId = grammar->nonTerminalSetId.id;
      ProductionSet productionSet = SymbolTable_get(grammar->productionSetId)->productionSet;
      char* productionSetId = grammar->productionSetId.id;

      if (terminalSet == NULL) {
        Error_incompatibleType(__func__, terminalSetId);
        break;
      }
      if (nonTerminalSet == NULL) {
        Error_incompatibleType(__func__, nonTerminalSetId);
        break;
      }
      if (productionSet == NULL) {
        Error_incompatibleType(__func__, productionSetId);
        break;
      }

      if (!Set_hasVoidIntersection(terminalSet, nonTerminalSet)) {
        Error_terminalsAndNonTerminalsCollision(__func__, terminalSetId, nonTerminalSetId);
        break;
      }

      if (!Set_has(nonTerminalSet, (SetElement){.symbol = grammar->initialSymbol})) {
        Error_undefinedInitialSymbol(__func__, nonTerminalSetId, grammar->initialSymbol.symbol);
        break;
      }

      validateProductionSet(grammar, terminalSet, nonTerminalSet, productionSet);
    }
  }
  return !errors;
}

void validateProductionSet(
  GrammarDefinition* grammar, SymbolSet terminalSet, SymbolSet nonTerminalSet, ProductionSet productionSet
) {
  char* terminalSetId = grammar->terminalSetId.id;
  char* nonTerminalSetId = grammar->nonTerminalSetId.id;
  char* productionSetId = grammar->productionSetId.id;

  SetIterator productionsIter = SetIterator_new(productionSet);
  bool isLinearRight = false;
  bool isLinearLeft = false;
  while (SetIterator_hasNext(productionsIter)) {
    Production* prod = SetIterator_next(productionsIter)->production;
    char* prodStr = Production_toString(prod);

    if (!Set_has(nonTerminalSet, (SetElement){.symbol = prod->lhs})) {
      Error_undefinedNonTerminalSymbolUsedInProduction(
        __func__, prod->lhs.symbol, prodStr, productionSetId, nonTerminalSetId
      );
    }

    SetIterator rhsIter = SetIterator_new(prod->rhs);
    while (SetIterator_hasNext(rhsIter)) {
      ProductionRhsRule* rule = SetIterator_next(rhsIter)->productionRhsRule;
      switch (rule->type) {
      case SYMBOL_SYMBOL_T:
        validateSymbolSymbolRhsRule(
          rule, terminalSet, nonTerminalSet, prodStr, productionSetId, &isLinearRight, &isLinearLeft
        );
        break;
      case SYMBOL_T:
        if (!Set_has(terminalSet, (SetElement){.symbol = rule->symbol})) {
          Error_undefinedTerminalSymbolUsedInProduction(
            __func__, rule->symbol.symbol, prodStr, productionSetId, terminalSetId
          );
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
  if (isLinearRight && isLinearLeft) Error_linearRightAndLeftProductionMix(__func__, productionSetId);
}

void validateSymbolSymbolRhsRule(
  ProductionRhsRule* rule, SymbolSet terminalSet, SymbolSet nonTerminalSet, const char* prodStr,
  const char* productionSetId, bool* isLinearRight, bool* isLinearLeft
) {
  SetElement leftSymbolEle = (SetElement){.symbol = rule->leftSymbol};
  SetElement rightSymbolEle = (SetElement){.symbol = rule->rightSymbol};
  bool leftIsTerminal = Set_has(terminalSet, leftSymbolEle);
  bool rightIsTerminal = Set_has(terminalSet, rightSymbolEle);
  if (leftIsTerminal && rightIsTerminal) {
    Error_invalidProduction(__func__, prodStr, productionSetId);
  } else if (leftIsTerminal && !rightIsTerminal) {
    if (!Set_has(nonTerminalSet, rightSymbolEle)) {
      Error_undefinedSymbol(__func__, rule->rightSymbol.symbol, prodStr, productionSetId);
    } else *isLinearRight = true;
  } else if (!leftIsTerminal && rightIsTerminal) {
    if (!Set_has(nonTerminalSet, leftSymbolEle)) {
      Error_undefinedSymbol(__func__, rule->leftSymbol.symbol, prodStr, productionSetId);
    } else *isLinearLeft = true;
  } else {
    bool leftIsNonTerminal = Set_has(nonTerminalSet, leftSymbolEle);
    bool rightIsNonTerminal = Set_has(nonTerminalSet, rightSymbolEle);
    if (leftIsNonTerminal && rightIsNonTerminal) {
      Error_invalidProduction(__func__, prodStr, productionSetId);
    } else {
      if (!leftIsNonTerminal) Error_undefinedSymbol(__func__, rule->leftSymbol.symbol, prodStr, productionSetId);
      if (!rightIsNonTerminal) Error_undefinedSymbol(__func__, rule->rightSymbol.symbol, prodStr, productionSetId);
    }
  }
}

void computeProgram(Program* program) {
  size_t sentencesLen = Array_getLen(program->sentences);
  for (int i = 0; i < sentencesLen; ++i) {
    Sentence* sentence = Array_get(program->sentences, i).sentence;
    switch (sentence->type) {
    case GRAMMAR_DEFINITION:
      computeGrammar(sentence->grammarDefinition);
    case LANGUAGE_BINDING:
    default:
      break;
    }
  }
}

void computeGrammar(GrammarDefinition* grammar) {
}

void initializeSemanticValidationModule() {
  _logger = createLogger("SemanticValidation");
}

void shutdownSemanticValidationModule() {
  destroyLogger(_logger);
}
