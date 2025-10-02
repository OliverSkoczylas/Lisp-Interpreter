// lisp_interpreter.h
// Oliver Skoczylas - CWID: 12281473
// Header file for LISP interpreter - Sprints 1-8

#ifndef LISP_INTERPRETER_H
#define LISP_INTERPRETER_H

#include <stdbool.h>

// ============================================================================
// TYPE DEFINITIONS
// ============================================================================

typedef enum {
    ATOM_NUMBER,
    ATOM_SYMBOL,
    ATOM_STRING,
    CONS_CELL,
    NIL_TYPE,
    LAMBDA_TYPE,
    PRIMITIVE_TYPE
} SexpType;

typedef struct Sexp Sexp;
typedef Sexp* (*PrimitiveFunc)(Sexp*, Sexp*);

struct Sexp {
    SexpType type;
    union {
        double number;
        char* symbol;
        char* string;
        struct {
            Sexp* car;
            Sexp* cdr;
        } cons;
        struct {
            Sexp* params;
            Sexp* body;
            Sexp* env;
        } lambda;
        PrimitiveFunc primitive;
    } data;
};

// ============================================================================
// GLOBAL CONSTANTS
// ============================================================================

extern Sexp* NIL;
extern Sexp* TRUE_SEXP;
extern Sexp* GLOBAL_ENV;

// ============================================================================
// MEMORY MANAGEMENT
// ============================================================================

Sexp* allocate_sexp(void);

// ============================================================================
// CONSTRUCTORS
// ============================================================================

Sexp* nil(void);
Sexp* make_number(double value);
Sexp* make_symbol(const char* value);
Sexp* make_string(const char* value);
Sexp* cons(Sexp* car, Sexp* cdr);
Sexp* make_lambda(Sexp* params, Sexp* body, Sexp* env);
Sexp* make_primitive(PrimitiveFunc func);

// ============================================================================
// PREDICATES
// ============================================================================

bool isNil(Sexp* s);
bool isNumber(Sexp* s);
bool isSymbol(Sexp* s);
bool isString(Sexp* s);
bool isList(Sexp* s);
bool isTrueSexp(Sexp* s);
bool isLambda(Sexp* s);
bool isPrimitive(Sexp* s);

// ============================================================================
// ACCESSORS
// ============================================================================

Sexp* car(Sexp* s);
Sexp* cdr(Sexp* s);
Sexp* cadr(Sexp* s);
Sexp* caddr(Sexp* s);
Sexp* cadddr(Sexp* s);

// ============================================================================
// ADDITIONAL FUNCTIONS
// ============================================================================

bool eq(Sexp* a, Sexp* b);
bool null(Sexp* s);

// ============================================================================
// ARITHMETIC FUNCTIONS
// ============================================================================

Sexp* add(Sexp* a, Sexp* b);
Sexp* sub(Sexp* a, Sexp* b);
Sexp* mul(Sexp* a, Sexp* b);
Sexp* divide(Sexp* a, Sexp* b);
Sexp* mod(Sexp* a, Sexp* b);

// ============================================================================
// COMPARISON FUNCTIONS
// ============================================================================

Sexp* lt(Sexp* a, Sexp* b);
Sexp* gt(Sexp* a, Sexp* b);
Sexp* lte(Sexp* a, Sexp* b);
Sexp* gte(Sexp* a, Sexp* b);
Sexp* not(Sexp* s);

// ============================================================================
// ENVIRONMENT FUNCTIONS
// ============================================================================

Sexp* make_env(Sexp* symbols, Sexp* values, Sexp* parent);
Sexp* env_symbols(Sexp* env);
Sexp* env_values(Sexp* env);
Sexp* env_parent(Sexp* env);
Sexp* env_set(Sexp* env, Sexp* symbol, Sexp* value);
Sexp* env_lookup(Sexp* env, Sexp* symbol);
void init_global_env(void);

// ============================================================================
// EVAL FUNCTION
// ============================================================================

Sexp* eval(Sexp* sexp, Sexp* env);
Sexp* eval_list(Sexp* list, Sexp* env);
Sexp* apply(Sexp* func, Sexp* args, Sexp* env);

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

Sexp* atom(const char* str);
Sexp* list(void);
Sexp* list1(Sexp* a);
Sexp* list2(Sexp* a, Sexp* b);
Sexp* list3(Sexp* a, Sexp* b, Sexp* c);
Sexp* list4(Sexp* a, Sexp* b, Sexp* c, Sexp* d);
Sexp* append(Sexp* list1, Sexp* list2);
int length(Sexp* list);

// ============================================================================
// PRINTING FUNCTIONS
// ============================================================================

void print_sexp(Sexp* s);
void println_sexp(Sexp* s);

// ============================================================================
// PARSER FUNCTIONS
// ============================================================================

Sexp* parse(const char* input);
Sexp* read_sexp(const char** input);

#endif // LISP_INTERPRETER_H