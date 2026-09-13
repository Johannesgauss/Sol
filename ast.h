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
        EXPR_SUBSCRIPT,
        EXPR_CALL,
        EXPR_ARG,
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
#endif // AST_H
