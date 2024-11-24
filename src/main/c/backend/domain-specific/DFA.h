#ifndef DFA_H
#define DFA_H

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "Grammar.h"
#include <stdbool.h>

typedef Symbol State;
typedef struct Transition Transition;
typedef struct DFA DFA;
typedef SymbolSet StateSet;
// typedef Set TransitionSet;

struct DFA {
  Id id;
  SymbolSet symbols;
  StateSet states;
  State initialState;
  State finalState;
  Array transitions;
};

struct Transition {
  State from;
  Symbol symbol;
  State to;
  bool isLambda;
};

DFA* DFA_fromGrammar(Id id, Grammar* grammar);
DFA* DFA_fromLanguageExpression(Id languageId, LanguageExpression* expr);
void DFA_free(DFA* dfa);
void DFA_print(DFA* dfa);

#endif
