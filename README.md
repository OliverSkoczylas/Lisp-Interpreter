Oliver Skoczylas
CWID: 12281473
Lisp Interpreter
Sprints 1-8 Complete Submission

================================================================================
README
================================================================================

Code Organization:
The code is organized into multiple files:
- lisp_interpreter.h: Header file with all type definitions and function declarations
- lisp_interpreter.c: Implementation file with all functionality
- test_suite.c: Comprehensive test suite covering all sprints
- repl.c: Optional interactive Read-Eval-Print Loop (REPL)

The implementation file is organized into clearly marked sections:
- Global constants and memory management
- Sprint 1: Constructors (nil, make_number, make_symbol, make_string, cons, make_lambda, make_primitive)
- Sprint 2: Predicates and accessors (isNil, isNumber, car, cdr, eq, etc.)
- Sprint 3: Arithmetic and comparison functions (+, -, *, /, %, <, >, <=, >=, not)
- Sprint 5: Environment management and eval function
- Sprint 6: Conditional special forms (if, and, or, cond)
- Sprint 7: User-defined functions (define)
- Sprint 8: Lambda functions and closures
- Helper functions and parser
- Printing functions

Build Process:
To compile the test suite on Windows with MinGW:
C:\MinGW\bin\gcc.exe -o test_lisp.exe lisp_interpreter.c test_suite.c -lm

To run the test suite:
.\test_lisp.exe

Optional REPL:
To compile the interactive REPL:
C:\MinGW\bin\gcc.exe -o lisp_repl.exe lisp_interpreter.c repl.c -lm

To run the REPL:
.\lisp_repl.exe

Main Location:
The main() function for testing is in test_suite.c. It initializes the global environment and runs all test suites automatically.
The main() function for the REPL is in repl.c. It provides an interactive environment for exploring the interpreter.

================================================================================
FEATURES IMPLEMENTED
================================================================================

Sprint 1 - Data Structures:
- S-expression types (numbers, symbols, strings, cons cells, nil)
- Constructor functions for all types
- Basic predicates (isNil, isNumber, isSymbol, isList, isTrueSexp)

Sprint 2 - Predicates & Accessors:
- List accessors (car, cdr, cadr, caddr, cadddr)
- Equality testing (eq)
- Null checking

Sprint 3 - Arithmetic & Comparison:
- Arithmetic operations (+, -, *, /, %)
- Comparison operations (<, >, <=, >=)
- Logical not operation
- Error handling for division by zero and type errors

Sprint 5 - Evaluation:
- Environment data structure with symbol binding
- Eval function for interpreting S-expressions
- Quote special form
- Set special form for variable assignment
- Function application
- Primitive function wrappers

Sprint 6 - Conditionals:
- If expression with true/false branches
- And expression with short-circuit evaluation
- Or expression with short-circuit evaluation
- Cond expression with multiple clauses

Sprint 7 - User-Defined Functions:
- Define special form for naming functions
- Function definitions with parameters
- Recursive functions (demonstrated with factorial)
- Lexical scoping

Sprint 8 - Lambda Functions:
- Lambda expressions creating anonymous functions
- First-class functions (can be assigned to variables)
- Closures (lambdas capture their environment)
- Higher-order functions (functions returning functions)

Additional Features:
- Parser for reading S-expressions from strings
- Quote syntax support with ' prefix
- Comprehensive error messages
- Pretty printing of S-expressions
- Global environment with built-in primitives
- Interactive REPL for exploring the interpreter

================================================================================
TEST PLAN
================================================================================

Testing Methodology:
A comprehensive test suite in test_suite.c runs automatically when the program executes. Each sprint has its own test section with multiple test cases.

Sprint 1 Tests:
- Type predicates with various S-expression types
- Cons cell construction
- List building and manipulation
- String recognition

Sprint 2 Tests:
- Car/cdr operations on lists
- Equality testing with numbers and symbols
- Null checking on empty and non-empty lists
- Nested list access with cadr, caddr

Sprint 3 Tests:
- All arithmetic operations with positive results
- Division by zero error handling
- All comparison operations
- Not operation on nil and non-nil values
- Type error handling for non-numeric operations

Sprint 5 Tests:
- Eval with self-evaluating expressions (nil, numbers, strings)
- Quote special form
- Basic arithmetic through eval
- Nested arithmetic expressions
- Set and variable lookup
- Primitive function calls

Sprint 6 Tests:
- If expressions with both branches
- And with short-circuit evaluation (avoids divide-by-zero)
- Or with short-circuit evaluation
- Cond with multiple clauses

Sprint 7 Tests:
- Simple function definition (square)
- Multi-parameter functions (add3)
- Recursive functions (factorial of 5 = 120)
- Function calls with expressions as arguments

Sprint 8 Tests:
- Anonymous lambda functions
- Multi-parameter lambdas
- Lambdas assigned to variables
- Higher-order functions (make-adder)
- Closures capturing environment

Integration Tests:
- Complex nested expressions combining multiple features
- Function composition (double of add1)
- Mixed use of conditionals, arithmetic, and functions

Parser Tests:
- Parsing simple expressions
- Parsing nested expressions
- Parsing quoted lists with ' syntax
- Parsing and evaluating lambda expressions

All tests display expected output and actual output for verification.

================================================================================
REPL USAGE
================================================================================

The REPL (Read-Eval-Print Loop) provides an interactive environment for testing expressions.

Commands:
- help: Display example expressions
- exit or quit: Exit the REPL

Multi-line Input:
The REPL supports multi-line expressions. If parentheses are unbalanced, it will continue reading input on subsequent lines.

Example Session:
lisp> (+ 2 3)
5

lisp> (define square (x)
...     (* x x))
#<lambda>

lisp> (square 7)
49

lisp> (define fib (n)
...     (if (<= n 1)
...       n
...       (+ (fib (- n 1)) (fib (- n 2)))))
#<lambda>

lisp> (fib 10)
55

The REPL uses a fully functional parser that handles complex nested expressions and can be used to interactively explore all features of the interpreter.

================================================================================
KNOWN ISSUES
================================================================================

Symbol Resolution:
- In Sprint 6 tests, symbols like "yes", "no", "first", "second", "third" return UNDEFINED because they're not bound in the environment. This is expected behavior - the interpreter correctly identifies undefined symbols.

================================================================================
DOCUMENTATION
================================================================================

Design Decisions:

1. Environment Representation:
   Environments are represented as ((symbols . values) . parent) using cons cells.
   This allows for efficient lookup with lexical scoping.

2. Error Handling:
   Errors return special symbols like "ERROR:DIVISION_BY_ZERO" or "UNDEFINED"
   rather than crashing. This makes the interpreter more robust.

3. NIL Singleton:
   NIL is implemented as a global singleton to ensure all nil references
   point to the same object, making equality checks reliable.

4. True Values:
   Any non-nil value is considered true. The symbol "T" is used to represent
   explicit true values in comparisons and logical operations.

5. Lambda Closures:
   Lambdas capture their defining environment, enabling proper lexical scoping
   and allowing for higher-order functions and closures.

6. Primitive Functions:
   Built-in operations are wrapped in primitive functions that follow the same
   calling convention as user-defined functions, providing uniformity.

7. REPL Implementation:
   The REPL is implemented as a separate executable that shares the core
   interpreter code. This provides an interactive environment while keeping
   the test suite independent.

8. Parser Implementation:
   The parser uses an iterative approach for reading lists to avoid recursion
   issues. It properly handles nested expressions, quoted lists, and dotted pairs.

Non-Standard Choices:
- Using symbol "T" for true instead of a dedicated boolean type
- Returning error symbols instead of using exception handling
- Global environment initialization required before use
- Separate executables for testing and interactive use

Strengths:
- Complete implementation of all required features through Sprint 8
- Support for recursion, closures, and higher-order functions
- Comprehensive test coverage
- Clean separation of concerns with header/implementation files
- Proper lexical scoping with environment chains
- Interactive REPL for exploration and demonstration
- Fully functional parser for reading S-expressions from strings

Limitations:
- No garbage collection (memory grows with use)
- Limited error recovery

================================================================================
TESTING RESULTS
================================================================================

All core interpreter functionality passes tests:
- Data structure creation and manipulation
- Arithmetic and comparison operations
- Evaluation and environment management
- Conditional expressions with short-circuit evaluation
- User-defined functions with recursion
- Lambda expressions and closures
- Higher-order functions
- Parser correctly handles simple and complex expressions

Test results are included in Test_suite_output(FOR_GRADING).txt

================================================================================
CONCLUSION
================================================================================

This LISP interpreter successfully implements all requirements from Sprints 1-8,
including data structures, predicates, arithmetic, evaluation, conditionals,
user-defined functions, and lambda expressions. The interpreter supports
advanced features like recursion, closures, and higher-order functions,
demonstrating a complete and functional implementation of core LISP semantics.

The addition of an interactive REPL with a fully functional parser provides
a user-friendly way to explore the interpreter's capabilities. The programmatic
test suite remains the authoritative demonstration of correctness, with all
required functionality tested and verified.
