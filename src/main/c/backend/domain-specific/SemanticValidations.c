#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/Array.h"
#include "../../shared/ColorMacros.h"
#include "../../shared/Logger.h"
#include "../errors/Errors.h"
#include "../symbol-table/SymbolTable.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static Logger* _logger = NULL;

void computeGrammar(GrammarDefinition* grammar);
void logUndefined(const char* functionName, const char* id);
void logInvalidType(const char* functionName, const char* id, const char* expectedType, const char* realType);

bool semanticValidation(Program* program) {
  size_t sentencesLen = Array_getLen(program->sentences);
  bool errors = false;
  for (int i = 0; i < sentencesLen; ++i) {
    Sentence* sentence = Array_get(program->sentences, i).sentence;
    if (sentence->type == GRAMMAR_DEFINITION) {
      GrammarDefinition* grammar = sentence->grammarDefinition;
      SymbolSet terminalSet = SymbolTable_get(grammar->terminalSetId)->symbolSet;
      SymbolSet nonTerminalSet = SymbolTable_get(grammar->nonTerminalSetId)->symbolSet;
      ProductionSet productionSet = SymbolTable_get(grammar->productionSetId)->productionSet;

      if (terminalSet == NULL) {
        Error_IncompatibleType(__func__, grammar->terminalSetId.id);
        break;
      }
      if (nonTerminalSet == NULL) {
        Error_IncompatibleType(__func__, grammar->nonTerminalSetId.id);
        break;
      }
      if (productionSet == NULL) {
        Error_IncompatibleType(__func__, grammar->productionSetId.id);
        break;
      }

      // Make a `bool Set_hasVoidIntersection` function that doesn't modify the sets.
      // Set_intersection(terminalSet, nonTerminalSet);
    }
  }
  return !errors;
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

void logUndefined(const char* functionName, const char* id) {
  logError(_logger, "%s: " COLORIZE_ID("%s") " isn't defined.", functionName, id);
}

void logInvalidType(const char* functionName, const char* id, const char* expectedType, const char* realType) {
  logError(
    _logger, "%s: %s expected to be of type `%s` but was of type `%s`", functionName, id, expectedType, realType
  );
}
