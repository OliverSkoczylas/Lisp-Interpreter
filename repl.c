// repl.c
// REPL (Read-Eval-Print Loop) for LISP Interpreter
// Based on Sprint 5 requirements

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lisp_interpreter.h"

#define MAX_INPUT 2048

void print_banner() {
    printf("=====================================\n");
    printf("  LISP Interpreter REPL\n");
    printf("  Type 'exit' or 'quit' to exit\n");
    printf("  Type 'help' for examples\n");
    printf("=====================================\n\n");
}

void print_help() {
    printf("\nExample expressions:\n");
    printf("Numbers and symbols:\n");
    printf("  42                                   ; Self-evaluating number\n");
    printf("  (set x 10)                           ; Bind variable\n");
    printf("  x                                    ; Lookup variable\n\n");
    
    printf("Arithmetic (Sprint 3):\n");
    printf("  (+ 2 3)                              ; 5\n");
    printf("  (* (+ 1 2) (- 5 2))                  ; 9\n");
    printf("  (/ 10 3)                             ; 3.333...\n");
    printf("  (% 17 5)                             ; 2\n\n");
    
    printf("Comparison (Sprint 3):\n");
    printf("  (< 5 10)                             ; T\n");
    printf("  (>= 10 10)                           ; T\n");
    printf("  (eq 7 7)                             ; T\n\n");
    
    printf("Quote (Sprint 5):\n");
    printf("  (quote (a b c))                      ; (a b c) - unevaluated\n");
    printf("  '(a b c)                             ; Same as above\n\n");
    
    printf("Conditionals (Sprint 6):\n");
    printf("  (if (> 5 3) 'yes 'no)                ; yes\n");
    printf("  (and (> 5 3) (< 2 4))                ; T - short-circuits\n");
    printf("  (or (< 5 3) (> 8 6))                 ; T - short-circuits\n");
    printf("  (cond ((< 5 3) 'first)\n");
    printf("        ((> 5 3) 'second)\n");
    printf("        (T 'third))                    ; second\n\n");
    
    printf("User-Defined Functions (Sprint 7):\n");
    printf("  (define square (x) (* x x))          ; Define function\n");
    printf("  (square 7)                           ; 49\n");
    printf("  (define fact (n)\n");
    printf("    (if (<= n 1) 1\n");
    printf("      (* n (fact (- n 1)))))           ; Recursive factorial\n");
    printf("  (fact 5)                             ; 120\n\n");
    
    printf("Lambda Functions (Sprint 8):\n");
    printf("  ((lambda (x) (* x 2)) 5)             ; 10\n");
    printf("  (set double (lambda (x) (* x 2)))    ; Assign lambda\n");
    printf("  (double 8)                           ; 16\n\n");
    
    printf("List operations:\n");
    printf("  (cons 1 '(2 3))                      ; (1 2 3)\n");
    printf("  (car '(a b c))                       ; a\n");
    printf("  (cdr '(a b c))                       ; (b c)\n\n");
}

// Check if parentheses are balanced
int is_balanced(const char* str) {
    int depth = 0;
    int in_string = 0;
    
    for (int i = 0; str[i]; i++) {
        if (str[i] == '"' && (i == 0 || str[i-1] != '\\')) {
            in_string = !in_string;
        }
        if (!in_string) {
            if (str[i] == '(') depth++;
            if (str[i] == ')') depth--;
            if (depth < 0) return 0; // Too many closing parens
        }
    }
    
    return depth == 0;
}

// Check if input is just whitespace
int is_empty(const char* str) {
    while (*str) {
        if (!isspace(*str)) return 0;
        str++;
    }
    return 1;
}

// Read potentially multi-line input until balanced
void read_expression(char* buffer, size_t max_size) {
    char line[256];
    buffer[0] = '\0';
    int first_line = 1;
    
    while (1) {
        if (first_line) {
            printf("lisp> ");
            first_line = 0;
        } else {
            printf("...   ");
        }
        fflush(stdout);
        
        if (!fgets(line, sizeof(line), stdin)) {
            strcpy(buffer, "exit");
            return;
        }
        
        // Check for commands on first line
        if (buffer[0] == '\0') {
            if (strncmp(line, "exit", 4) == 0 || strncmp(line, "quit", 4) == 0) {
                strcpy(buffer, "exit");
                return;
            }
            if (strncmp(line, "help", 4) == 0) {
                strcpy(buffer, "help");
                return;
            }
        }
        
        // Append line to buffer
        if (strlen(buffer) + strlen(line) < max_size - 1) {
            strcat(buffer, line);
        } else {
            printf("Error: Input too long\n");
            buffer[0] = '\0';
            return;
        }
        
        // Check if complete
        if (is_balanced(buffer)) {
            return;
        }
    }
}

void repl() {
    char input[MAX_INPUT];
    
    print_banner();
    
    while (1) {
        // Read - potentially multi-line
        read_expression(input, MAX_INPUT);
        
        // Handle special commands
        if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
            printf("Goodbye!\n");
            break;
        }
        
        if (strcmp(input, "help") == 0) {
            print_help();
            continue;
        }
        
        // Skip empty input
        if (is_empty(input)) {
            continue;
        }
        
        // Parse the input string into S-expression
        Sexp* expr = parse(input);
        
        // Check for parse error
        if (isNil(expr) && !is_empty(input)) {
            printf("Parse error: Could not read expression\n\n");
            continue;
        }
        
        // Eval - evaluate the S-expression in global environment
        Sexp* result = eval(expr, GLOBAL_ENV);
        
        // Print - display the result
        if (result) {
            print_sexp(result);
            printf("\n\n");
        } else {
            printf("Error: eval returned NULL\n\n");
        }
    }
}

int main() {
    // Initialize the interpreter as per Sprint 5
    nil();                  // Initialize NIL
    init_global_env();      // Initialize global environment with primitives
    
    // Start the REPL
    repl();
    
    return 0;
}