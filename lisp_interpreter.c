// lisp_interpreter.c
// Oliver Skoczylas - CWID: 12281473
// Implementation of LISP interpreter - Sprints 1-8

#include "lisp_interpreter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ============================================================================
// GLOBAL CONSTANTS DEFINITION
// ============================================================================

Sexp* NIL = NULL;
Sexp* TRUE_SEXP = NULL;
Sexp* GLOBAL_ENV = NULL;

// ============================================================================
// MEMORY MANAGEMENT
// ============================================================================

Sexp* allocate_sexp() {
    Sexp* s = (Sexp*)malloc(sizeof(Sexp));
    if (!s) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    return s;
}

// ============================================================================
// SPRINT 1: CONSTRUCTORS
// ============================================================================

Sexp* nil() {
    if (!NIL) {
        NIL = allocate_sexp();
        NIL->type = NIL_TYPE;
    }
    return NIL;
}

Sexp* make_number(double value) {
    Sexp* s = allocate_sexp();
    s->type = ATOM_NUMBER;
    s->data.number = value;
    return s;
}

Sexp* make_symbol(const char* value) {
    Sexp* s = allocate_sexp();
    s->type = ATOM_SYMBOL;
    s->data.symbol = strdup(value);
    return s;
}

Sexp* make_string(const char* value) {
    Sexp* s = allocate_sexp();
    s->type = ATOM_STRING;
    s->data.string = strdup(value);
    return s;
}

Sexp* cons(Sexp* car, Sexp* cdr) {
    Sexp* s = allocate_sexp();
    s->type = CONS_CELL;
    s->data.cons.car = car;
    s->data.cons.cdr = cdr;
    return s;
}

Sexp* make_lambda(Sexp* params, Sexp* body, Sexp* env) {
    Sexp* s = allocate_sexp();
    s->type = LAMBDA_TYPE;
    s->data.lambda.params = params;
    s->data.lambda.body = body;
    s->data.lambda.env = env;
    return s;
}

Sexp* make_primitive(PrimitiveFunc func) {
    Sexp* s = allocate_sexp();
    s->type = PRIMITIVE_TYPE;
    s->data.primitive = func;
    return s;
}

// ============================================================================
// SPRINT 2: PREDICATES
// ============================================================================

bool isNil(Sexp* s) {
    return s == nil() || (s && s->type == NIL_TYPE);
}

bool isNumber(Sexp* s) {
    return s && s->type == ATOM_NUMBER;
}

bool isSymbol(Sexp* s) {
    return s && s->type == ATOM_SYMBOL;
}

bool isString(Sexp* s) {
    return s && s->type == ATOM_STRING;
}

bool isList(Sexp* s) {
    return isNil(s) || (s && s->type == CONS_CELL);
}

bool isTrueSexp(Sexp* s) {
    return !isNil(s);
}

bool isLambda(Sexp* s) {
    return s && s->type == LAMBDA_TYPE;
}

bool isPrimitive(Sexp* s) {
    return s && s->type == PRIMITIVE_TYPE;
}

// ============================================================================
// SPRINT 2: ACCESSORS
// ============================================================================

Sexp* car(Sexp* s) {
    if (!s || s->type != CONS_CELL) {
        fprintf(stderr, "Error: car called on non-cons cell\n");
        return nil();
    }
    return s->data.cons.car;
}

Sexp* cdr(Sexp* s) {
    if (!s || s->type != CONS_CELL) {
        fprintf(stderr, "Error: cdr called on non-cons cell\n");
        return nil();
    }
    return s->data.cons.cdr;
}

Sexp* cadr(Sexp* s) {
    return car(cdr(s));
}

Sexp* caddr(Sexp* s) {
    return car(cdr(cdr(s)));
}

Sexp* cadddr(Sexp* s) {
    return car(cdr(cdr(cdr(s))));
}

// ============================================================================
// SPRINT 2: ADDITIONAL FUNCTIONS
// ============================================================================

bool eq(Sexp* a, Sexp* b) {
    if (isNil(a) && isNil(b)) return true;
    if (isNil(a) || isNil(b)) return false;
    if (a->type != b->type) return false;
    
    switch (a->type) {
        case ATOM_NUMBER:
            return a->data.number == b->data.number;
        case ATOM_SYMBOL:
            return strcmp(a->data.symbol, b->data.symbol) == 0;
        case ATOM_STRING:
            return strcmp(a->data.string, b->data.string) == 0;
        case CONS_CELL:
            return a == b;
        default:
            return false;
    }
}

bool null(Sexp* s) {
    return isNil(s);
}

// ============================================================================
// SPRINT 3: ARITHMETIC FUNCTIONS
// ============================================================================

Sexp* add(Sexp* a, Sexp* b) {
    if (!isNumber(a) || !isNumber(b)) {
        return make_symbol("ERROR:NOT_A_NUMBER");
    }
    return make_number(a->data.number + b->data.number);
}

Sexp* sub(Sexp* a, Sexp* b) {
    if (!isNumber(a) || !isNumber(b)) {
        return make_symbol("ERROR:NOT_A_NUMBER");
    }
    return make_number(a->data.number - b->data.number);
}

Sexp* mul(Sexp* a, Sexp* b) {
    if (!isNumber(a) || !isNumber(b)) {
        return make_symbol("ERROR:NOT_A_NUMBER");
    }
    return make_number(a->data.number * b->data.number);
}

Sexp* divide(Sexp* a, Sexp* b) {
    if (!isNumber(a) || !isNumber(b)) {
        return make_symbol("ERROR:NOT_A_NUMBER");
    }
    if (b->data.number == 0) {
        return make_symbol("ERROR:DIVISION_BY_ZERO");
    }
    return make_number(a->data.number / b->data.number);
}

Sexp* mod(Sexp* a, Sexp* b) {
    if (!isNumber(a) || !isNumber(b)) {
        return make_symbol("ERROR:NOT_A_NUMBER");
    }
    if (b->data.number == 0) {
        return make_symbol("ERROR:DIVISION_BY_ZERO");
    }
    return make_number((int)a->data.number % (int)b->data.number);
}

// ============================================================================
// SPRINT 3: COMPARISON FUNCTIONS
// ============================================================================

Sexp* lt(Sexp* a, Sexp* b) {
    if (!isNumber(a) || !isNumber(b)) {
        return make_symbol("ERROR:NOT_A_NUMBER");
    }
    return (a->data.number < b->data.number) ? make_symbol("T") : nil();
}

Sexp* gt(Sexp* a, Sexp* b) {
    if (!isNumber(a) || !isNumber(b)) {
        return make_symbol("ERROR:NOT_A_NUMBER");
    }
    return (a->data.number > b->data.number) ? make_symbol("T") : nil();
}

Sexp* lte(Sexp* a, Sexp* b) {
    if (!isNumber(a) || !isNumber(b)) {
        return make_symbol("ERROR:NOT_A_NUMBER");
    }
    return (a->data.number <= b->data.number) ? make_symbol("T") : nil();
}

Sexp* gte(Sexp* a, Sexp* b) {
    if (!isNumber(a) || !isNumber(b)) {
        return make_symbol("ERROR:NOT_A_NUMBER");
    }
    return (a->data.number >= b->data.number) ? make_symbol("T") : nil();
}

Sexp* not(Sexp* s) {
    return isNil(s) ? make_symbol("T") : nil();
}

// ============================================================================
// SPRINT 5: ENVIRONMENT MANAGEMENT
// ============================================================================

Sexp* make_env(Sexp* symbols, Sexp* values, Sexp* parent) {
    return cons(cons(symbols, values), parent);
}

Sexp* env_symbols(Sexp* env) {
    if (isNil(env)) return nil();
    return car(car(env));
}

Sexp* env_values(Sexp* env) {
    if (isNil(env)) return nil();
    return cdr(car(env));
}

Sexp* env_parent(Sexp* env) {
    if (isNil(env)) return nil();
    return cdr(env);
}

Sexp* env_set(Sexp* env, Sexp* symbol, Sexp* value) {
    Sexp* symbols = env_symbols(env);
    Sexp* values = env_values(env);
    
    // Add to the front of both lists
    Sexp* new_symbols = cons(symbol, symbols);
    Sexp* new_values = cons(value, values);
    
    // Update the environment
    env->data.cons.car = cons(new_symbols, new_values);
    return value;
}

Sexp* env_lookup(Sexp* env, Sexp* symbol) {
    while (!isNil(env)) {
        Sexp* symbols = env_symbols(env);
        Sexp* values = env_values(env);
        
        while (!isNil(symbols)) {
            if (isSymbol(car(symbols)) && isSymbol(symbol)) {
                if (strcmp(car(symbols)->data.symbol, symbol->data.symbol) == 0) {
                    return car(values);
                }
            }
            symbols = cdr(symbols);
            values = cdr(values);
        }
        env = env_parent(env);
    }
    
    // Symbol not found - return undefined message
    return make_symbol("UNDEFINED");
}

// Primitive function wrappers for eval
Sexp* prim_add(Sexp* args, Sexp* env) {
    (void)env;
    return add(car(args), cadr(args));
}

Sexp* prim_sub(Sexp* args, Sexp* env) {
    (void)env;
    return sub(car(args), cadr(args));
}

Sexp* prim_mul(Sexp* args, Sexp* env) {
    (void)env;
    return mul(car(args), cadr(args));
}

Sexp* prim_div(Sexp* args, Sexp* env) {
    (void)env;
    return divide(car(args), cadr(args));
}

Sexp* prim_mod(Sexp* args, Sexp* env) {
    (void)env;
    return mod(car(args), cadr(args));
}

Sexp* prim_lt(Sexp* args, Sexp* env) {
    (void)env;
    return lt(car(args), cadr(args));
}

Sexp* prim_gt(Sexp* args, Sexp* env) {
    (void)env;
    return gt(car(args), cadr(args));
}

Sexp* prim_lte(Sexp* args, Sexp* env) {
    (void)env;
    return lte(car(args), cadr(args));
}

Sexp* prim_gte(Sexp* args, Sexp* env) {
    (void)env;
    return gte(car(args), cadr(args));
}

Sexp* prim_eq(Sexp* args, Sexp* env) {
    (void)env;
    return eq(car(args), cadr(args)) ? make_symbol("T") : nil();
}

Sexp* prim_not(Sexp* args, Sexp* env) {
    (void)env;
    return not(car(args));
}

Sexp* prim_cons(Sexp* args, Sexp* env) {
    (void)env;
    return cons(car(args), cadr(args));
}

Sexp* prim_car(Sexp* args, Sexp* env) {
    (void)env;
    return car(car(args));
}

Sexp* prim_cdr(Sexp* args, Sexp* env) {
    (void)env;
    return cdr(car(args));
}

void init_global_env() {
    GLOBAL_ENV = make_env(nil(), nil(), nil());
    
    // Add primitive functions
    env_set(GLOBAL_ENV, make_symbol("+"), make_primitive(prim_add));
    env_set(GLOBAL_ENV, make_symbol("-"), make_primitive(prim_sub));
    env_set(GLOBAL_ENV, make_symbol("*"), make_primitive(prim_mul));
    env_set(GLOBAL_ENV, make_symbol("/"), make_primitive(prim_div));
    env_set(GLOBAL_ENV, make_symbol("%"), make_primitive(prim_mod));
    env_set(GLOBAL_ENV, make_symbol("<"), make_primitive(prim_lt));
    env_set(GLOBAL_ENV, make_symbol(">"), make_primitive(prim_gt));
    env_set(GLOBAL_ENV, make_symbol("<="), make_primitive(prim_lte));
    env_set(GLOBAL_ENV, make_symbol(">="), make_primitive(prim_gte));
    env_set(GLOBAL_ENV, make_symbol("eq"), make_primitive(prim_eq));
    env_set(GLOBAL_ENV, make_symbol("not"), make_primitive(prim_not));
    env_set(GLOBAL_ENV, make_symbol("cons"), make_primitive(prim_cons));
    env_set(GLOBAL_ENV, make_symbol("car"), make_primitive(prim_car));
    env_set(GLOBAL_ENV, make_symbol("cdr"), make_primitive(prim_cdr));
    
    // Alternative names
    env_set(GLOBAL_ENV, make_symbol("add"), make_primitive(prim_add));
    env_set(GLOBAL_ENV, make_symbol("sub"), make_primitive(prim_sub));
    env_set(GLOBAL_ENV, make_symbol("mul"), make_primitive(prim_mul));
    env_set(GLOBAL_ENV, make_symbol("div"), make_primitive(prim_div));
    env_set(GLOBAL_ENV, make_symbol("mod"), make_primitive(prim_mod));
}

// ============================================================================
// SPRINT 5-8: EVAL FUNCTION
// ============================================================================

Sexp* eval_list(Sexp* list, Sexp* env) {
    if (isNil(list)) return nil();
    return cons(eval(car(list), env), eval_list(cdr(list), env));
}

Sexp* apply(Sexp* func, Sexp* args, Sexp* env) {
    if (isPrimitive(func)) {
        return func->data.primitive(args, env);
    } else if (isLambda(func)) {
        // Create new environment with parameters bound to arguments
        Sexp* new_env = make_env(func->data.lambda.params, args, func->data.lambda.env);
        return eval(func->data.lambda.body, new_env);
    }
    return make_symbol("ERROR:NOT_A_FUNCTION");
}

Sexp* eval(Sexp* sexp, Sexp* env) {
    // Handle nil
    if (isNil(sexp)) {
        return nil();
    }
    
    // Handle numbers and strings - self-evaluating
    if (isNumber(sexp) || isString(sexp)) {
        return sexp;
    }
    
    // Handle symbols - look up in environment
    if (isSymbol(sexp)) {
        return env_lookup(env, sexp);
    }
    
    // Handle lists - function calls and special forms
    if (isList(sexp)) {
        Sexp* first = car(sexp);
        
        // Special forms
        if (isSymbol(first)) {
            char* sym = first->data.symbol;
            
            // QUOTE
            if (strcmp(sym, "quote") == 0) {
                return cadr(sexp);
            }
            
            // SET
            if (strcmp(sym, "set") == 0) {
                Sexp* symbol = cadr(sexp);
                Sexp* value = eval(caddr(sexp), env);
                return env_set(env, symbol, value);
            }
            
            // DEFINE (Sprint 7)
            if (strcmp(sym, "define") == 0) {
                Sexp* name = cadr(sexp);
                Sexp* params = caddr(sexp);
                Sexp* body = cadddr(sexp);
                Sexp* lambda = make_lambda(params, body, env);
                return env_set(env, name, lambda);
            }
            
            // LAMBDA (Sprint 8)
            if (strcmp(sym, "lambda") == 0) {
                Sexp* params = cadr(sexp);
                Sexp* body = caddr(sexp);
                return make_lambda(params, body, env);
            }
            
            // IF (Sprint 6)
            if (strcmp(sym, "if") == 0) {
                Sexp* test = eval(cadr(sexp), env);
                if (isTrueSexp(test)) {
                    return eval(caddr(sexp), env);
                } else {
                    return eval(cadddr(sexp), env);
                }
            }
            
            // AND (Sprint 6)
            if (strcmp(sym, "and") == 0) {
                Sexp* e1 = eval(cadr(sexp), env);
                if (isNil(e1)) {
                    return nil();
                }
                return eval(caddr(sexp), env);
            }
            
            // OR (Sprint 6)
            if (strcmp(sym, "or") == 0) {
                Sexp* e1 = eval(cadr(sexp), env);
                if (!isNil(e1)) {
                    return make_symbol("T");
                }
                return eval(caddr(sexp), env);
            }
            
            // COND (Sprint 6)
            if (strcmp(sym, "cond") == 0) {
                Sexp* clauses = cdr(sexp);
                while (!isNil(clauses)) {
                    Sexp* clause = car(clauses);
                    Sexp* test = eval(car(clause), env);
                    if (isTrueSexp(test)) {
                        return eval(cadr(clause), env);
                    }
                    clauses = cdr(clauses);
                }
                return nil();  // No clause matched
            }
        }
        
        // Regular function call - evaluate function and arguments
        Sexp* func = eval(first, env);
        Sexp* args = eval_list(cdr(sexp), env);
        return apply(func, args, env);
    }
    
    return sexp;
}

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

Sexp* atom(const char* str) {
    // Check if it's a number
    char* endptr;
    double val = strtod(str, &endptr);
    if (*endptr == '\0' && str[0] != '\0') {
        return make_number(val);
    }
    
    // Check if it's a string (starts and ends with quotes)
    if (str[0] == '"' && str[strlen(str)-1] == '"') {
        char* content = strdup(str + 1);
        content[strlen(content)-1] = '\0';
        Sexp* s = make_string(content);
        free(content);
        return s;
    }
    
    // Otherwise it's a symbol
    return make_symbol(str);
}

Sexp* list() {
    return nil();
}

Sexp* list1(Sexp* a) {
    return cons(a, nil());
}

Sexp* list2(Sexp* a, Sexp* b) {
    return cons(a, cons(b, nil()));
}

Sexp* list3(Sexp* a, Sexp* b, Sexp* c) {
    return cons(a, cons(b, cons(c, nil())));
}

Sexp* list4(Sexp* a, Sexp* b, Sexp* c, Sexp* d) {
    return cons(a, cons(b, cons(c, cons(d, nil()))));
}

Sexp* append(Sexp* list1, Sexp* list2) {
    if (isNil(list1)) return list2;
    return cons(car(list1), append(cdr(list1), list2));
}

int length(Sexp* list) {
    int len = 0;
    while (!isNil(list)) {
        len++;
        list = cdr(list);
    }
    return len;
}

// ============================================================================
// SIMPLE PARSER
// ============================================================================

void skip_whitespace(const char** input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

Sexp* read_atom(const char** input) {
    char buffer[256];
    int i = 0;
    
    // Handle strings
    if (**input == '"') {
        buffer[i++] = *(*input)++;
        while (**input && **input != '"' && i < 255) {
            buffer[i++] = *(*input)++;
        }
        if (**input == '"') {
            buffer[i++] = *(*input)++;
        }
        buffer[i] = '\0';
        return atom(buffer);
    }
    
    // Handle symbols and numbers
    while (**input && !isspace(**input) && **input != '(' && **input != ')' && i < 255) {
        buffer[i++] = *(*input)++;
    }
    buffer[i] = '\0';
    
    if (i == 0) return nil();
    return atom(buffer);
}

Sexp* read_list(const char** input);

Sexp* read_sexp(const char** input) {
    skip_whitespace(input);
    
    if (**input == '\0') {
        return nil();
    }
    
    if (**input == '(') {
        return read_list(input);
    }
    
    if (**input == '\'') {
        (*input)++;  // Skip quote
        return list2(make_symbol("quote"), read_sexp(input));
    }
    
    return read_atom(input);
}

Sexp* read_list(const char** input) {
    (*input)++;  // Skip opening paren
    skip_whitespace(input);
    
    if (**input == ')') {
        (*input)++;  // Skip closing paren
        return nil();
    }
    
    // Build list iteratively to avoid recursion bug
    Sexp* head = nil();
    Sexp* tail = nil();
    
    while (**input != ')' && **input != '\0') {
        Sexp* elem = read_sexp(input);
        skip_whitespace(input);
        
        // Check for dotted pair
        if (**input == '.' && (*(*input + 1) == ' ' || *(*input + 1) == ')' || isspace(*(*input + 1)))) {
            (*input)++;  // Skip dot
            skip_whitespace(input);
            Sexp* rest = read_sexp(input);
            skip_whitespace(input);
            if (**input == ')') {
                (*input)++;
            }
            
            // Handle dotted pair
            if (isNil(head)) {
                return cons(elem, rest);
            } else {
                tail->data.cons.cdr = cons(elem, rest);
                return head;
            }
        }
        
        // Add element to list
        if (isNil(head)) {
            head = tail = cons(elem, nil());
        } else {
            tail->data.cons.cdr = cons(elem, nil());
            tail = tail->data.cons.cdr;
        }
        
        skip_whitespace(input);
    }
    
    if (**input == ')') {
        (*input)++;  // Skip closing paren
    }
    
    return head;
}

Sexp* parse(const char* input) {
    return read_sexp(&input);
}

// ============================================================================
// PRINTING FUNCTIONS
// ============================================================================

void print_sexp(Sexp* s) {
    if (isNil(s)) {
        printf("()");
        return;
    }
    
    switch (s->type) {
        case ATOM_NUMBER:
            if (s->data.number == (int)s->data.number) {
                printf("%d", (int)s->data.number);
            } else {
                printf("%g", s->data.number);
            }
            break;
            
        case ATOM_SYMBOL:
            printf("%s", s->data.symbol);
            break;
            
        case ATOM_STRING:
            printf("\"%s\"", s->data.string);
            break;
            
        case LAMBDA_TYPE:
            printf("#<lambda>");
            break;
            
        case PRIMITIVE_TYPE:
            printf("#<primitive>");
            break;
            
        case CONS_CELL:
            printf("(");
            Sexp* current = s;
            while (current && current->type == CONS_CELL) {
                print_sexp(current->data.cons.car);
                current = current->data.cons.cdr;
                if (current && !isNil(current)) {
                    if (current->type == CONS_CELL) {
                        printf(" ");
                    } else {
                        // Dotted pair
                        printf(" . ");
                        print_sexp(current);
                        break;
                    }
                }
            }
            printf(")");
            break;
            
        default:
            break;
    }
}

void println_sexp(Sexp* s) {
    print_sexp(s);
    printf("\n");
}