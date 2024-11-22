#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

/** Initialize module's internal state. */
void initializeAbstractSyntaxTreeModule();

/** Shutdown module's internal state. */
void shutdownAbstractSyntaxTreeModule();

// Forward declaration of Array to avoid circular references.
typedef struct ArrayCDT* Array;
typedef struct SetCDT* Set;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

typedef enum { GRAMMAR_DEFINITION, SYMBOL_SET, PRODUCTION_SET, LANGUAGE_SENTENCE } SentenceType;

typedef enum { SYMBOL_SYMBOL_T, SYMBOL_T, LAMBDA_T } ProductionRhsRuleType;

typedef enum {
  LANG_ID_T,
  LANG_OF_GRAMMAR_T,
  LANG_UNION_T,
  LANG_INTERSECTION_T,
  LANG_SUBTRACTION_T,
  LANG_CONCATENATION_T,
  LANG_REVERSE_T,
  LANG_COMPLEMENT_T
} LanguageExpressionType;

/**
 * This typedefs allows self-referencing types.
 */

typedef struct Id Id;
typedef struct Symbol Symbol;

typedef struct Program Program;
typedef struct Sentence Sentence;
typedef struct GrammarDefinition GrammarDefinition;
typedef struct SymbolSetBinding SymbolSetBinding;
typedef struct ProductionSetBinding ProductionSetBinding;
typedef struct Production Production;
typedef struct ProductionRhsRule ProductionRhsRule;
typedef struct LanguageBinding LanguageBinding;
typedef struct LanguageExpression LanguageExpression;

typedef Array SentenceArray;
typedef Set SymbolSet;
typedef Set ProductionSet;
typedef Set ProductionRhsRuleSet;

struct Id {
  char* id;
  int length;
};

struct Symbol {
  char* symbol;
  int length;
};

struct Program {
  SentenceArray sentences;
};

struct Sentence {
  union {
    GrammarDefinition* grammarDefinition;
    SymbolSetBinding* symbolSetBinding;
    ProductionSetBinding* productionSetBinding;
    LanguageBinding* languageBinding;
  };
  SentenceType type;
};

struct GrammarDefinition {
  Id id;
  Id terminalSetId;
  Id nonTerminalSetId;
  Id productionSetId;
  Symbol initialSymbolId;
};

struct SymbolSetBinding {
  Id id;
  SymbolSet set;
};

struct ProductionSetBinding {
  Id id;
  ProductionSet productions;
};

struct Production {
  Symbol lhs;
  ProductionRhsRuleSet rhs;
};

struct ProductionRhsRule {
  union {
    struct {
      Symbol leftSymbol;
      Symbol rightSymbol;
    };
    Symbol symbol;
  };
  ProductionRhsRuleType type;
};

struct LanguageBinding {
  Id id;
  LanguageExpression* languageExpression;
};

struct LanguageExpression {
  union {
    Id languageId;
    Id grammarId;
    LanguageExpression* unaryExpression;
    struct {
      LanguageExpression* leftExpression;
      LanguageExpression* rightExpression;
    };
  };
  LanguageExpressionType type;
};

/**
 * Node recursive destructors.
 */
// void releaseConstant(Constant* constant);
// void releaseExpression(Expression* expression);
// void releaseFactor(Factor* factor);
void releaseProgram(Program* program);

#endif
