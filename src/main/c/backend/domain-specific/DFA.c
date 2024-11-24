#include "DFA.h"
#include "../../frontend/syntactic-analysis/ASTUtils/SymbolUtils.h"
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/Array.h"
// #include "../../shared/Logger.h"
#include "../../shared/ArrayElement.h"
#include "../../shared/Set.h"
#include "../../shared/SetElement.h"
#include "../../shared/String.h"
// #include "../errors/Errors.h"
#include "../../shared/utils.h"
#include "../symbol-table/SymbolTable.h"
#include "Grammar.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10

Transition* Transition_fromProduction(Symbol lhs, ProductionRhsRule* rule, State finalState, const char* statePrefix);
ArrayElement Transition_cloneEle(ArrayElement ele);
void Transition_freeEle(ArrayElement ele);
char* Transition_toString(ArrayElement ele);
DFA* DFA_clone(DFA* dfa);
DFA* DFA_cloneWithIdPrefix(DFA* dfa, Id id);
State State_format(State state, const char* fmt);
State State_addPrefix(State state, const char* prefix);
StateSet StateSet_addPrefix(StateSet states, const char* prefix);
StateSet StateSet_addPrefixFreeSrc(StateSet srcStates, const char* prefix);
Array Transitions_addIdPrefix(Array transitions, Id id);
Transition* Transition_newWithSymbol(State from, State to, Symbol symbol);
Transition* Transition_newWithLambda(State from, State to);
DFA* DFA_union(DFA* left, DFA* right, int counter);
DFA* DFA_fromLanguageExpressionRec(LanguageExpression* expr);

DFA* DFA_fromGrammar(Id id, Grammar* grammar) {
  DFA* dfa = safeMalloc(sizeof(DFA));

  // if (!grammar->linearRight) {

  // }

  dfa->id = id;
  char* statePrefix = "q";
  dfa->states = StateSet_addPrefix(grammar->nonTerminalSet, statePrefix);
  // Note: as we've added the prefix 'q_' to every original state the symbol `qf` won't collide with any of them.
  dfa->finalState = (State){.symbol = strdup("qf"), .length = 2};
  Set_add(dfa->states, (SetElement){.state = Symbol_clone(dfa->finalState)});

  dfa->symbols = Set_clone(grammar->terminalSet);
  dfa->initialState = State_addPrefix(grammar->initialSymbol, statePrefix);

  dfa->transitions = Array_newCloneable(INITIAL_CAPACITY, Transition_freeEle, Transition_toString, Transition_cloneEle);

  SetIterator productionsIter = SetIterator_new(grammar->productionSet);
  while (SetIterator_hasNext(productionsIter)) {
    Production* prod = SetIterator_next(productionsIter)->production;
    SetIterator rhsIter = SetIterator_new(prod->rhs);
    while (SetIterator_hasNext(rhsIter)) {
      ProductionRhsRule* rule = SetIterator_next(rhsIter)->productionRhsRule;
      Array_push(
        dfa->transitions,
        (ArrayElement){.transition = Transition_fromProduction(prod->lhs, rule, dfa->finalState, statePrefix)}
      );
    }
    SetIterator_free(rhsIter);
  }
  SetIterator_free(productionsIter);

  return dfa;
}

DFA* DFA_fromLanguageExpression(Id languageId, LanguageExpression* expr) {
  DFA* dfa = DFA_fromLanguageExpressionRec(expr);
  dfa->id = languageId;
  return dfa;
}

DFA* DFA_fromLanguageExpressionRec(LanguageExpression* expr) {
  static int counter = 0;
  DFA* dfa;
  StateSet oldStates;
  switch (expr->type) {
  case LANG_OF_GRAMMAR_T:
    dfa = DFA_cloneWithIdPrefix(DfaTable_get(expr->grammarId), expr->grammarId);
    break;
  case LANG_ID_T:
    dfa = DFA_cloneWithIdPrefix(DfaTable_get(expr->languageId), expr->languageId);
    break;
  case LANG_UNION_T:;
    DFA* leftDfa = DFA_fromLanguageExpressionRec(expr->leftExpression);
    DFA* rightDfa = DFA_fromLanguageExpressionRec(expr->rightExpression);
    DFA_print(leftDfa);
    DFA_print(rightDfa);
    dfa = DFA_union(leftDfa, rightDfa, counter++);
    break;
  default:
    // Should never happen.
    return NULL;
    break;
  }
  return dfa;
}

DFA* DFA_union(DFA* left, DFA* right, int counter) {
  char* newFinalStateStr = safeAsprintf("qf%d", counter);
  State newFinalState = (State){.symbol = newFinalStateStr, .length = (int)strlen(newFinalStateStr)};
  char* newInitStateStr = safeAsprintf("qi%d", counter);
  State newInitState = (State){.symbol = newInitStateStr, .length = (int)strlen(newInitStateStr)};
  Array_push(
    left->transitions, (ArrayElement){.transition = Transition_newWithLambda(newInitState, left->initialState)}
  );
  Array_push(
    left->transitions,
    (ArrayElement){.transition = Transition_newWithLambda(newInitState, Symbol_clone(right->initialState))}
  );
  Array_push(
    left->transitions, (ArrayElement){.transition = Transition_newWithLambda(left->finalState, newFinalState)}
  );
  Array_push(
    left->transitions,
    (ArrayElement){.transition = Transition_newWithLambda(Symbol_clone(right->finalState), newFinalState)}
  );
  left->initialState = newInitState;
  left->finalState = newFinalState;
  Set_add(left->states, (SetElement){.state = Symbol_clone(newInitState)});
  Set_add(left->states, (SetElement){.state = Symbol_clone(newFinalState)});

  Array_concat(left->transitions, right->transitions);
  right->transitions = NULL;
  // size_t len = Array_getLen(right->transitions);
  // for (int i = 0; i < len; ++i) {
  //   Transition* transition = Array_get(right->transitions, i).transition;
  // }

  Set_union(left->states, right->states);
  right->states = NULL;
  Set_union(left->symbols, right->symbols);
  right->symbols = NULL;

  DFA_free(right);
  return left;
}

DFA* DFA_clone(DFA* dfa) {
  DFA* clone = safeMalloc(sizeof(DFA));
  clone->initialState = Symbol_clone(dfa->initialState);
  clone->states = Set_clone(dfa->states);
  clone->transitions = Array_clone(dfa->transitions);
  clone->symbols = Set_clone(dfa->symbols);
  clone->finalState = Symbol_clone(dfa->finalState);
  clone->id = dfa->id;

  return clone;
}

DFA* DFA_cloneWithIdPrefix(DFA* dfa, Id id) {
  dfa = DFA_clone(dfa);
  StateSet oldStates = dfa->states;
  dfa->states = StateSet_addPrefixFreeSrc(oldStates, id.id);
  dfa->transitions = Transitions_addIdPrefix(dfa->transitions, id);
  State oldInitialState = dfa->initialState;
  dfa->initialState = State_addPrefix(oldInitialState, id.id);
  free(oldInitialState.symbol);
  State oldFinalState = dfa->finalState;
  dfa->finalState = State_addPrefix(oldFinalState, id.id);
  free(oldFinalState.symbol);
  return dfa;
}

void DFA_free(DFA* dfa) {
  free(dfa->initialState.symbol);
  free(dfa->finalState.symbol);
  if (dfa->states != NULL) Set_free(dfa->states);
  if (dfa->symbols != NULL) Set_free(dfa->symbols);
  if (dfa->transitions != NULL) Array_free(dfa->transitions);
  free(dfa);
}

void DFA_print(DFA* dfa) {
  char* transitionsStr = Array_toString(dfa->transitions);
  char* statesStr = Set_toString(dfa->states);
  printf("DFA %s transitions: %s -- states: %s\n", dfa->id.id, transitionsStr, statesStr);
  free(transitionsStr);
  free(statesStr);
}

Transition* Transition_newWithSymbol(State from, State to, Symbol symbol) {
  Transition* transition = safeMalloc(sizeof(Transition));
  transition->from = from;
  transition->to = to;
  transition->symbol = symbol;
  transition->isLambda = false;

  return transition;
}

Transition* Transition_newWithLambda(State from, State to) {
  Transition* transition = safeMalloc(sizeof(Transition));
  transition->from = from;
  transition->to = to;
  transition->isLambda = true;

  return transition;
}

Transition* Transition_fromProduction(Symbol lhs, ProductionRhsRule* rule, State finalState, const char* statePrefix) {
  Transition* transition = safeMalloc(sizeof(Transition));
  transition->from = State_addPrefix(lhs, statePrefix);
  transition->isLambda = false;
  switch (rule->type) {
  case SYMBOL_SYMBOL_T:
    transition->to = State_addPrefix(rule->rightSymbol, statePrefix);
    transition->symbol = rule->leftSymbol;
    break;
  case SYMBOL_T:
    transition->to = Symbol_clone(finalState);
    transition->symbol = rule->symbol;
    break;
  case LAMBDA_T:
    transition->to = Symbol_clone(finalState);
    transition->isLambda = true;
    break;
  }
  return transition;
}

ArrayElement Transition_cloneEle(ArrayElement ele) {
  Transition* transition = safeMalloc(sizeof(Transition));
  transition->isLambda = ele.transition->isLambda;
  transition->symbol = ele.transition->symbol;
  transition->from = Symbol_clone(ele.transition->from);
  transition->to = Symbol_clone(ele.transition->to);
  return (ArrayElement){.transition = transition};
}

void Transition_freeEle(ArrayElement ele) {
  free(ele.transition->from.symbol);
  free(ele.transition->to.symbol);
  free(ele.transition);
}

char* Transition_toString(ArrayElement ele) {
  Transition* transition = ele.transition;
  char* asdf = safeAsprintf(
    "(%s, %s -> %s)", transition->from.symbol, transition->isLambda ? "λ" : transition->symbol.symbol,
    transition->to.symbol
  );
  return asdf;
}

State State_format(State state, const char* fmt) {
  char* newState = safeAsprintf(fmt, state.symbol);
  int newLen = (int)strlen(newState);
  return (State){.symbol = newState, .length = newLen};
}

State State_addPrefix(State state, const char* prefix) {
  char* fmt = safeAsprintf("%s_%%s", prefix);
  State newState = State_format(state, fmt);
  free(fmt);
  return newState;
}

StateSet StateSet_addPrefix(StateSet states, const char* prefix) {
  char* fmt = safeAsprintf("%s_%%s", prefix);
  StateSet newStates = Set_cloneEmpty(states);
  SetIterator iter = SetIterator_new(states);
  while (SetIterator_hasNext(iter)) {
    State state = SetIterator_next(iter)->state;
    Set_add(newStates, (SetElement){.state = State_format(state, fmt)});
  }
  SetIterator_free(iter);
  free(fmt);
  return newStates;
}

StateSet StateSet_addPrefixFreeSrc(StateSet srcStates, const char* prefix) {
  StateSet newStates = StateSet_addPrefix(srcStates, prefix);
  free(srcStates);
  return newStates;
}

Array Transitions_addIdPrefix(Array transitions, Id id) {
  char* fmt = safeAsprintf("%s_%%s", id.id);
  Array newTransitions = Array_cloneEmpty(transitions);
  size_t len = Array_getLen(transitions);
  for (int i = 0; i < len; ++i) {
    Transition* transition = Array_get(transitions, i).transition;
    State newFrom = State_format(transition->from, fmt);
    State newTo = State_format(transition->to, fmt);
    Transition* newTransition = transition->isLambda ? Transition_newWithLambda(newFrom, newTo)
                                                     : Transition_newWithSymbol(newFrom, newTo, transition->symbol);
    Array_push(newTransitions, (ArrayElement){.transition = newTransition});
  }
  free(fmt);
  Array_free(transitions);
  return newTransitions;
}
