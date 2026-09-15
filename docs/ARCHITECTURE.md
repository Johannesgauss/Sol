# Sol Architecture & Codebase Overview

Sol is a C-like compiled programming language designed and implemented in C. Its compiler architecture is structured as a classical front-end pipeline: **Lexer $\rightarrow$ Parser Utilities $\rightarrow$ Recursive Descent Parser $\rightarrow$ Abstract Syntax Tree (AST)**.

---

## 1. Pipeline Overview

```
Source Code (.sol / .c)
      │
      ▼
   [lexer.h]          Hand-written inlined lexer / tokenizer (get_token)
      │
      ▼
 [parser_util.c]      Parser cursor management (peek, scan, consume, putback, expect)
      │
      ▼
[parser_grammar.c]    Recursive descent syntax parser (decls, functions, stmts, exprs)
      │
      ▼
 [ast.h / ast.c]      Abstract Syntax Tree nodes, constructors, destructors
```

---

## 2. Modules & File Responsibilities

### `lexer.h` (Lexer / Tokenizer)
- **Token Type Enum**: `enum token_type` prefixed with `TOKEN_` (e.g., `TOKEN_INT`, `TOKEN_PLUS`, `TOKEN_IF`, `TOKEN_VOID`, `TOKEN_EOF`).
- **Token Struct**:
  ```c
  typedef struct Token {
      token_type type;
      char *begin, *end;
  } Token;
  ```
  Tokens do not copy strings into individual buffers; they point directly into the source text (`begin` to `end`).
- **Tokenizer Function**: `static inline Token get_token(char **src_ptr)`:
  - Skips whitespace and line comments (`//`).
  - Scans operators (`+`, `-`, `*`, `/`, `%`, `==`, `!=`, `<`, `<=`, `>`, `>=`, `=`, `!`, `&`, `#`, `;`, `:`, `(`, `)`, `{`, `}`, `[`, `]`, `,`).
  - Scans numeric literals (`isdigit`).
  - Scans keywords and identifiers (`if`, `else`, `while`, `for`, `return`, `continue`, `break`, `goto`, `char`, `tiny`, `int`, `long`, `tux`, `void`, and `TOKEN_NON_PROTECTED_WORD`).
  - Advances caller's `*src_ptr` to the end of the token.

### `parser_util.h` & `parser_util.c` (Parser State & Lookahead)
Maintains the parser's cursor and backtrack buffer:
```c
typedef struct parser {
    Token head;    // Current active token
    Token backup;  // Backup token for 1-token putback / backtracking
} Parser;
extern Parser parser;
```
- **`parser_init(char *source)`**: Initializes parser with the source string buffer.
- **`peek_token()`**: Inspects the next token without advancing the parser cursor (uses a temporary pointer copy to `get_token`).
- **`scan_token()`**: Saves current state to `parser.backup`, scans the next token, updates `parser.head`, and returns it.
- **`consume_token()`**: Advances the token stream by scanning one token.
- **`putback_token()`**: Restores `parser.head` from `parser.backup` (1-token backtrack).
- **`expect_token(token_type type)`**: Scans a token; if matching, returns `true`; if mismatched, calls `putback_token()` and returns `false`.

### `parser_grammar.c` (Grammar & Recursive Descent)
Contains syntax analysis and AST construction:
- **`parser_decl(Token token)`**: Parses variable declarations and delegates function headers to `parser_function`.
- **`parser_function(char *name, type_kind kind)`**: Parses function parameter list `(...)`, creates `TYPE_FUNCTION`, checks for `{ body }` (definition) or `;` (prototype), and returns `decl__create_code(...)`.
- **`parser_param_list()`**: Parses comma-separated parameter declarations, allocating linked `Param_list` nodes dynamically and linking via `next`.
- **Statements**:
  - `parser_if_else(Token token)`
  - `parser_for(Token token)`
  - `parser_while(Token token)`
  - `parser_return(Token token)`
  - `parser_break(Token token)`
  - `parser_continue(Token token)`
  - `parser_goto(Token token)`
  - `parser_statement(Token token)`
  - `parser_body(Token token)`
- **In Progress / Stubs**:
  - `parser_expr()`: Expression parser (currently being implemented).
  - `parser_program()`: Top-level file parser (author implementing).

### `ast.h` & `ast.c` (Abstract Syntax Tree)
Heavily based on Douglas Thain's B-minor compiler AST design:
- **Node Kinds**:
  - `Decl`: `name`, `type`, union `{ Expr *value; Stmt *code; }`, `next`.
  - `Stmt`: `kind` (`STMT_DECL`, `STMT_EXPR`, `STMT_IF_ELSE`, `STMT_FOR`, `STMT_WHILE`, `STMT_PRINT`, `STMT_RETURN`, `STMT_BLOCK`, `STMT_BREAK`, `STMT_CONTINUE`, `STMT_GOTO`, `STMT_LABEL`), body unions, `next`.
  - `Expr`: `kind`, `left`, `right`, literal unions (`name`, `int_value`, `str_literal`, `bool_value`, `char_value`).
  - `Type`: `kind`, `subtype`, `params` (`Param_list *`).
  - `Param_list`: `name`, `type`, `next`.
- **Constructors**: Prefixed with type and double-underscore (e.g. `decl__create_value`, `decl__create_code`, `stmt__create_*`, `expr__create_*`, `type__create`, `param_list__create`).
- **Destructors**: Complete deep-freeing destructors (`decl__destroy`, `stmt__destroy`, `expr__destroy`, `type__destroy`, `param_list__destroy`).
