# Current Implementation Status & Roadmap

This document outlines the exact state of the Sol compiler as of September 2026, summarizing completed components, verified behaviors, and ongoing tasks.

---

## 1. Verified & Working Components

| Component | File | Status | Notes |
|---|---|---|---|
| **Lexer Enums & Tokens** | `lexer.h` | Complete | All enums prefixed with `TOKEN_`. Supports keywords, types (`TOKEN_VOID` included), operators (`+`, `-`, `*`, `/`, `%`, `==`, `!=`, `<`, `<=`, `>`, `>=`, `=`, `!`, `&`, `:`, `;`, etc.), and brackets. |
| **Lexer Function** | `lexer.h` | Complete | `get_token()` handles comments, multi-character operators, numbers, and identifiers without consuming extra tokens. Compiles cleanly. |
| **Parser Cursor & Backtrack** | `parser_util.c` / `h` | Complete | `parser_init`, `peek_token` (non-advancing lookahead), `scan_token` (with `parser.head` synchronization and backup), `consume_token`, `putback_token`, and `expect_token` are implemented and verified. |
| **Parameter List Parsing** | `parser_grammar.c` | Complete | `parser_param_list()` dynamically allocates individual `Param_list` nodes and chains via `param->next`. Supports empty `()`, `(void)`, pointers (`*`), and base types. |
| **Declaration Parsing** | `parser_grammar.c` | Complete | `parser_decl()` parses base types and identifiers. If followed by `(`, it delegates to `parser_function()`. Disallows `void` variables. Parses optional `= expr` and requires `;`. |
| **Function Parsing** | `parser_grammar.c` | Complete | `parser_function()` parses parameter lists, matches `)`, builds `TYPE_FUNCTION` with return type as subtype, and returns `decl__create_code` with either parsed body (`parser_body`) or `NULL` for prototypes. |
| **Statements** | `parser_grammar.c` | Complete | `parser_if_else`, `parser_for`, `parser_while`, `parser_return`, `parser_break`, `parser_continue`, `parser_goto`, `parser_statement`, and `parser_body` compile. |
| **AST Definitions & Lifetime**| `ast.h` / `ast.c` | Complete | All constructors and deep-freeing destructors (`*__destroy`) are implemented. |

---

## 2. In-Progress & Remaining Work

### A. Expression Parser (`parser_expr()`)
- **Location**: `parser_grammar.c` (lines 198–229)
- **Current State**: Contains a preliminary stub that has syntax errors (`expr` undeclared, redefinition of `first_token`).
- **Goal**: Implement expression parsing supporting operator precedence:
  1. Primary expressions: numbers (`TOKEN_DIGIT`), variable names (`TOKEN_NON_PROTECTED_WORD`), and grouped expressions `( expr )`.
  2. Binary operations: `+`, `-`, `*`, `/`, `%`, `< `, `<=`, `>`, `>=`, `==`, `!=`.
  3. Unary operations: `-`, `!`, `*`, `&`.
  4. Assignment expressions: `name = expr`.
  5. Function calls: `name(arg1, arg2)`.

### B. Top-Level Program Parser (`parser_program()`)
- **Location**: `parser_grammar.c`
- **State**: Left intentionally for the author to implement.
- **Concept**: A loop that runs while `peek_token().type != TOKEN_EOF`, calling `parser_decl(scan_token())` and linking the declarations into a linked list of `Decl *`.

### C. Array Bracket Dimensions
- **Location**: `parser_grammar.c` (`parser_param_list` and `parser_decl`)
- **State**: Marked with `// yet to be implemented` for parsing explicit integer sizes inside `[NUM]` brackets.

---

## 3. How to Verify Compilation

Use `gcc -fsyntax-only` to verify syntax of modified files:
```bash
# Verify lexer
gcc -fsyntax-only lexer.h

# Verify parser utilities
gcc -fsyntax-only parser_util.c

# Verify grammar (note: parser_expr currently contains placeholder code)
gcc -fsyntax-only parser_grammar.c
```
