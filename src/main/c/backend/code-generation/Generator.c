#include "Generator.h"
// #include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../frontend/syntactic-analysis/ASTUtils/SymbolUtils.h"
#include "../../shared/Array.h"
#include "../../shared/Logger.h"
#include "../../shared/Set.h"
#include "../../shared/String.h"
#include "../domain-specific/DFA.h"
#include "../symbol-table/SymbolTable.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger* _logger = NULL;

void initializeGeneratorModule() {
  _logger = createLogger("Generator");
}

void shutdownGeneratorModule() {
  if (_logger != NULL) {
    destroyLogger(_logger);
  }
}

/** PRIVATE FUNCTIONS */

static void _generatePrologue();
static void _generateEpilogue();
static char* _indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char* const format, ...);

static void _generateDFANodes(int indentationLevel, DFA* dfa) {
  // qi [label="qi", shape=circle, style="bold", color="#00FF00"];
  // qf [label="qf", shape=doublecircle];
  // q [label="q", shape=circle];
  SetIterator iter = SetIterator_new(dfa->states);
  while (SetIterator_hasNext(iter)) {
    State state = SetIterator_next(iter)->state;
    if (Symbol_equals(state, dfa->initialState)) {
      _output(
        indentationLevel, "%s [label=\"%s\", shape=circle, style=\"bold\", color=\"#00FF00\"];\n", state.symbol,
        state.symbol
      );
    } else if (Symbol_equals(state, dfa->finalState)) {
      _output(indentationLevel, "%s [label=\"%s\", shape=doublecircle];\n", state.symbol, state.symbol);
    } else {
      _output(indentationLevel, "%s [label=\"%s\", shape=circle];\n", state.symbol, state.symbol);
    }
  }
  SetIterator_free(iter);
}

static void _generateTransitions(int indentationLevel, DFA* dfa) {
  // q1 -> q2 [label="a"];
  size_t len = Array_getLen(dfa->transitions);
  for (int i = 0; i < len; ++i) {
    Transition* transition = Array_get(dfa->transitions, i).transition;
    if (transition->isLambda) {
      _output(indentationLevel, "%s -> %s [label=\"λ\"]\n", transition->from.symbol, transition->to.symbol);
    } else {
      _output(
        indentationLevel, "%s -> %s [label=\"%s\"]\n", transition->from.symbol, transition->to.symbol,
        transition->symbol.symbol
      );
    }
  }
}

static void _generateDFA(DFA* dfa) {
  _generateDFANodes(1, dfa);
  _generateTransitions(1, dfa);
  _generateEpilogue();
}

static void _generatePrologue(void) {
  _output(0, "digraph {\n");
}

static void _generateEpilogue(void) {
  _output(0, "}\n");
}

/**
 * Generates an indentation string for the specified level.
 */
static char* _indentation(const unsigned int level) {
  return indentation(_indentationCharacter, level, _indentationSize);
}

/**
 * Outputs a formatted string to standard output. The "fflush" instruction
 * allows to see the output even close to a failure, because it drops the
 * buffering.
 */
static void _output(const unsigned int indentationLevel, const char* const format, ...) {
  va_list arguments;
  va_start(arguments, format);
  char* indentation = _indentation(indentationLevel);
  char* effectiveFormat = concatenate(2, indentation, format);
  vfprintf(stdout, effectiveFormat, arguments);
  fflush(stdout);
  free(effectiveFormat);
  free(indentation);
  va_end(arguments);
}

/** PUBLIC FUNCTIONS */

void generate() {
  logDebugging(_logger, "Generating final output...");
  SetIterator dfaIter = DfaTableIterator_new();
  while (DfaTableIterator_hasNext(dfaIter)) {
    DFA* dfa = DfaTableIterator_next(dfaIter);
    _output(0, "\n------------ Code for DFA %s ------------\n\n", dfa->id.id);
    _generatePrologue();
    _generateDFA(dfa);
    // _generateEpilogue(compilerState->value);
  }
  DfaTableIterator_free(dfaIter);
  logDebugging(_logger, "Generation is done.");
}
