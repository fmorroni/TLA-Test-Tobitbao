%{

// C file dependencies

#include "BisonActions/ActionsLogger.h"
#include "BisonActions/GrammarActions.h"
#include "BisonActions/LanguageActions.h"
#include "BisonActions/ProductionActions.h"
#include "BisonActions/ProductionRhsActions.h"
#include "BisonActions/ProgramActions.h"
#include "BisonActions/SentenceActions.h"
#include "BisonActions/SymbolActions.h"
#include "SyntacticAnalyzer.h"
#include "AbstractSyntaxTree.h"
#include "../../shared/Array.h"
#include <stdio.h>

%}

%code requires {

// Header file dependencies

#include "AbstractSyntaxTree.h"
#include "../../shared/Type.h"

}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. */
	Token token;
  Id id;
  Symbol symbol;

	/** Non-terminals. */
	Program* program;
  SentenceArray sentences;
  Sentence* sentence;
  GrammarDefinition* grammarDefinition;
  SymbolSetBinding* symbolSetBinding;
  SymbolSet symbolSet;
  SymbolSet symbols;
  ProductionSetBinding* productionSetBinding;
  ProductionSet productionSet;
  ProductionSet productions;
  Production* production;
  ProductionRhsRuleSet productionRhsRules;
  ProductionRhsRule* productionRhsRule;
  LanguageBinding* languageBinding;
  LanguageExpression* languageExpression;
}

/** Terminals. */
%token <id> ID
%token <id> ID_GRAM
%token <id> ID_LANG
%token <id> ID_PROD
%token <id> ID_SYM
%token <symbol> SYMBOL
%token <token> ANGLE_BRACKET_CLOSE
%token <token> ANGLE_BRACKET_OPEN
%token <token> BRACES_CLOSE
%token <token> BRACES_OPEN
%token <token> CARET
%token <token> CONCATENATION
%token <token> COMMA
%token <token> COMPLEMENT
%token <token> EQUALS
%token <token> INTERSECTION
%token <token> LANGUAGE
%token <token> REVERSE
%token <token> LAMBDA
%token <token> PARENTHESIS_CLOSE
%token <token> PARENTHESIS_OPEN
%token <token> PIPE
%token <token> RIGHT_ARROW
%token <token> SUBTRACTION
%token <token> UNION

%token <token> UNKNOWN

/** Non-terminals. */
%type <grammarDefinition> grammarDefinition
%type <languageBinding> languageBinding
%type <languageExpression> languageExpression
%type <production> production
%type <productionRhsRule> productionRhsRule
%type <productionRhsRules> productionRhsRules
%type <productionSet> productionSet
%type <productionSetBinding> productionSetBinding
%type <productions> productions
%type <program> program
%type <sentence> sentence
%type <sentences> sentences
%type <symbolSet> symbolSet
%type <symbolSetBinding> symbolSetBinding
%type <symbols> symbols

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach. To use this mechanism, the AST must be translated into
 * another structure.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
/*

From the documentation and my own testing these actually seem to only be executed when an error occurs, so they're
actually needed.

%destructor { printf("\nFreeing id\n\n"); free($$); } ID
%destructor { printf("\nFreeing SymbolSet\n\n"); Array_free($$); } symbols
%destructor { printf("\nFreeing ProductionSet\n\n"); Array_free($$); } symbols
%destructor { printf("\nFreeing GrammarDefinition\n\n"); GrammarDefinition_free($$); } grammarDefinition
%destructor { printf("\nFreeing SymbolSetBinding\n\n"); SymbolSetBinding_free($$); } symbolSetBinding
%destructor { printf("\nFreeing ProductionSetBinding\n\n"); ProductionSetBinding_free($$); } symbolSetBinding
*/

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left UNION INTERSECTION SUBTRACTION
%left CONCATENATION
%left COMPLEMENT
%left CARET


%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

// ProgramSemanticAction should validate that all grammars are properly defined (all its sets exist and have no erros, etc).
program: sentences                              { $$ = ProgramSemanticAction(currentCompilerState(), $1); }

sentences: sentence                             { $$ = SentenceArray_new($1); }
  | sentences[list] sentence[val]               { $$ = SentenceArray_push($list, $val); }
  ;

sentence: grammarDefinition                     { $$ = GrammarDefinitionSentence_new($1); }
  | symbolSetBinding                            { $$ = SymbolSetBindingSentence_new($1); }
  | productionSetBinding                        { $$ = ProductionSetBindingSentence_new($1); }
  | languageBinding                             { $$ = LanguageBindingSentence_new($1); }
  ;

grammarDefinition:
  ID[grammarId] EQUALS ANGLE_BRACKET_OPEN
    ID[terminalsId] COMMA
    ID[nonTerminalsId] COMMA
    ID[productionsId] COMMA
    ID[initialSymbolId]
  ANGLE_BRACKET_CLOSE                           { $$ = GrammarDefinition_new(
                                                         $grammarId, $terminalsId, $nonTerminalsId, $productionsId, $initialSymbolId
                                                       );
                                                }

symbolSetBinding:
  ID[id] EQUALS symbolSet[set]                                            { $$ = SymbolSetBinding_new($id, $set); }

symbolSet: BRACES_OPEN symbols[values] BRACES_CLOSE                       { $$ = $values; }
  | BRACES_OPEN symbols[values] COMMA BRACES_CLOSE                        { $$ = $values; }
  | ID_SYM                                                                { $$ = SymbolSet_clone($1); }
  | symbolSet[left] UNION symbolSet[right]                                { $$ = SymbolSet_union($left, $right); }
  | symbolSet[left] INTERSECTION symbolSet[right]                         { $$ = SymbolSet_intersection($left, $right); }
  | symbolSet[left] SUBTRACTION symbolSet[right]                          { $$ = SymbolSet_subtraction($left, $right); }
  | PARENTHESIS_OPEN symbolSet[set] PARENTHESIS_CLOSE                     { $$ = $set; }
  ;

symbols: SYMBOL                                                           { $$ = SymbolSet_new($1); }
  | symbols[list] COMMA SYMBOL[val]                                       { $$ = SymbolSet_add($list, $val); }
  ;

productionSetBinding:
  ID[id] EQUALS productionSet[setExpr]                                    { $$ = ProductionSetBinding_new($id, $setExpr); }

productionSet: BRACES_OPEN productions[values] BRACES_CLOSE               { $$ = $values; }
  | BRACES_OPEN productions[values] COMMA BRACES_CLOSE                    { $$ = $values; }
  // | ID_PROD                                                               { $$ = SymbolSet_clone($1); }
  | productionSet[left] UNION productionSet[right]                        { $$ = ProductionSet_union($left, $right); }
  | productionSet[left] INTERSECTION productionSet[right]                 { $$ = ProductionSet_intersection($left, $right); }
  | productionSet[left] SUBTRACTION productionSet[right]                  { $$ = ProductionSet_subtraction($left, $right); }
  | PARENTHESIS_OPEN productionSet[pSet] PARENTHESIS_CLOSE                { $$ = $pSet; }
  ;

productions: production                                                   { $$ = ProductionSet_new($1); }
  | productions[list] COMMA production[val]                               { $$ = ProductionSet_add($list, $val); }
  ;

production: SYMBOL[lhs] RIGHT_ARROW productionRhsRules[rhs]               { $$ = Production_new($lhs, $rhs); }

productionRhsRules: productionRhsRule                                     { $$ = ProductionRhsRuleSet_new($1); }
  | productionRhsRules[list] PIPE productionRhsRule[val]                  { $$ = ProductionRhsRuleSet_add($list, $val); }
  ;

productionRhsRule: SYMBOL SYMBOL                                          { $$ = ProductionRhsRuleSymbolSymbol_new($1, $2); }
  | SYMBOL                                                                { $$ = ProductionRhsRuleSymbol_new($1); }
  | LAMBDA                                                                { $$ = ProductionRhsRuleLambda_new(); }
  ;

languageBinding: ID[id] EQUALS languageExpression[lang]                   { $$ = LanguageBinding_new($id, $lang); } 

languageExpression: LANGUAGE PARENTHESIS_OPEN
     ID_GRAM[grammarID]
   PARENTHESIS_CLOSE                                                      { $$ = SimpleLanguageExpression_new($grammarID, LANG_OF_GRAMMAR_T); }
 | ID_LANG[langId]                                                        { $$ = SimpleLanguageExpression_new($langId, LANG_ID_T); }
 | languageExpression[left] UNION languageExpression[right]               { $$ = BinaryLanguageExpression_new($left, $right, LANG_UNION_T); }
 | languageExpression[left] INTERSECTION languageExpression[right]        { $$ = BinaryLanguageExpression_new($left, $right, LANG_INTERSECTION_T); }
 | languageExpression[left] SUBTRACTION languageExpression[right]         { $$ = BinaryLanguageExpression_new($left, $right, LANG_SUBTRACTION_T); }
 | languageExpression[left] CONCATENATION languageExpression[right]       { $$ = BinaryLanguageExpression_new($left, $right, LANG_CONCATENATION_T); }
 | languageExpression[expr] CARET REVERSE                                 { $$ = UnaryLanguageExpression_new($expr, LANG_REVERSE_T); }
 | COMPLEMENT languageExpression[expr]                                    { $$ = UnaryLanguageExpression_new($expr, LANG_COMPLEMENT_T); }
 | PARENTHESIS_OPEN languageExpression[expr] PARENTHESIS_CLOSE            { $$ = $expr; }
 ;
