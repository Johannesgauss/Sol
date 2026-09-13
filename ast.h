#ifndef AST_H
#define AST_H
struct Expr;
struct Stmt;
struct Type;
struct Param_list;
struct Decl;

typedef struct Expr Expr;
typedef struct Stmt Stmt;
typedef struct Decl Decl;
typedef struct Type Type;
typedef struct Param_list Param_list;
typedef struct Stmt_for_block Stmt_for_block;
typedef struct Stmt_if_else_block Stmt_if_else_block;
typedef struct Stmt_while_block Stmt_while_block;

void decl__destroy(Decl *self);
void stmt__destroy(Stmt *self);
void expr__destroy(Expr *self);
void type__destroy(Type *self);
void param_list__destroy(Param_list *self);

typedef enum {
        TYPE_VOID,
        TYPE_BOOLEAN,
        TYPE_CHARACTER,
        TYPE_TINY,
        TYPE_INTEGER,
        TYPE_LONG,
        TYPE_TUX,
        TYPE_STRING,
        TYPE_ARRAY,
        TYPE_POINTER,
        TYPE_FUNCTION
} type_kind;

struct Type {
        type_kind kind;
        Type *subtype;
        Param_list *params;
};

struct Param_list {
        char *name;
        Type *type;
        Param_list *next;
};

struct Stmt_for_block {
        Expr *init_expr;
        Expr *expr;
        Expr *next_expr;
        Stmt *body;
};

struct Stmt_if_else_block {
        Expr *expr;
        Stmt *body;
        Stmt *else_body;
};

struct Stmt_while_block {
        Expr *expr;
        Stmt *body;
};

typedef enum {
        STMT_DECL,
        STMT_EXPR,
        STMT_IF_ELSE,
        STMT_FOR,
        STMT_WHILE,
        STMT_PRINT,
        STMT_RETURN,
        STMT_BLOCK,
        STMT_BREAK,
        STMT_CONTINUE,
        STMT_GOTO,
        STMT_LABEL
} stmt_kind;

typedef enum {
        EXPR_ADD,
        EXPR_SUB,
        EXPR_MUL,
        EXPR_DIV,
        EXPR_MOD,
        EXPR_NEG,
        EXPR_NOT,
        EXPR_EQ,
        EXPR_NEQ,
        EXPR_LT,
        EXPR_GT,
        EXPR_LE,
        EXPR_GE,
        EXPR_AND,
        EXPR_OR,
        EXPR_ASSIGN,
        EXPR_COMMA,
        EXPR_CALL,
        EXPR_NAME,
        EXPR_INTEGER_LITERAL,
        EXPR_STRING_LITERAL,
        EXPR_BOOLEAN_LITERAL,
        EXPR_CHAR_LITERAL,
        EXPR_DEREF,
        EXPR_ADDR
} expr_kind;

struct Expr {
        expr_kind kind;
        Expr *left, *right;
        
        union {
                const char *name;
                int int_value;
                const char *str_literal;
                int bool_value;
                char char_value;
        };
};

struct Stmt {
        stmt_kind kind;
        union {
                Decl *decl;
                Stmt_for_block for_block;
                Stmt_if_else_block if_else_block;
                Stmt_while_block while_block;
                Expr *expr;
                Stmt *body;
                const char *label;
        };

        Stmt *next;
};

struct Decl {
        char *name;
        Type *type;
        union {
                Expr *value;
                Stmt *code;
        };
        Decl *next;
};

// Constructors
Decl *decl__create_value(char *name, Type *type, Expr *value, Decl *next);
Decl *decl__create_code(char *name, Type *type, Stmt *code, Decl *next);

Stmt *stmt__create_decl(Decl *decl, Stmt *next);
Stmt *stmt__create_expr(Expr *expr, Stmt *next);
Stmt *stmt__create_if_else(Expr *expr, Stmt *body, Stmt *else_body, Stmt *next);
Stmt *stmt__create_for(Expr *init_expr, Expr *expr, Expr *next_expr, Stmt *body, Stmt *next);
Stmt *stmt__create_while(Expr *expr, Stmt *body, Stmt *next);
Stmt *stmt__create_print(Expr *expr, Stmt *next);
Stmt *stmt__create_return(Expr *expr, Stmt *next);
Stmt *stmt__create_block(Stmt *body, Stmt *next);
Stmt *stmt__create_break(Stmt *next);
Stmt *stmt__create_continue(Stmt *next);
Stmt *stmt__create_goto(const char *label, Stmt *next);
Stmt *stmt__create_label(const char *label, Stmt *next);

Expr *expr__create(expr_kind kind, Expr *left, Expr *right);
Expr *expr__create_unary(expr_kind kind, Expr *expr);
Expr *expr__create_name(const char *name);
Expr *expr__create_integer_literal(int int_value);
Expr *expr__create_string_literal(const char *str_literal);
Expr *expr__create_boolean_literal(int bool_value);
Expr *expr__create_char_literal(char char_value);
Expr *expr__create_deref(Expr *expr);
Expr *expr__create_addr(Expr *expr);
Expr *expr__create_comma(Expr *left, Expr *right);

Type *type__create(type_kind kind, Type *subtype, Param_list *params);
Param_list *param_list__create(char *name, Type *type, Param_list *next);

#endif // AST_H
