# AI Agent Working Guidelines for Sol

**ATTENTION FUTURE AI AGENTS**: Read this file and `AGENTS.md` in the repository root before making any suggestions or modifying any code.

---

## 1. Student Learning Philosophy (Crucial Rule)

- The repository author is a **student learning compiler construction and language design**.
- **DO NOT rewrite or replace the author's code with standard, library, or idiomatic boilerplate** from other compilers.
- **Always preserve the user's architectural approach, variable names (`first_token`, `second_token`, etc.), control flow, and comments.**
- Keep changes surgical and line-level: fix only the exact lines causing bugs, compiler errors, or missing logic, leaving everything else intact.
- Explain the "why" clearly so the author learns the compiler concepts behind every modification.

---

## 2. Git & Commit Workflow Rules (Strict Requirement)

These rules are enforced by `AGENTS.md` in the repository root:
1. **Manual Staging**: The author will manually stage files using `git add`. **The AI MUST NEVER run `git add`** or stage files automatically.
2. **Commit Confirmation Gate**: When work is ready to commit, the AI **MUST explicitly ask the user for permission** to commit and wait for an explicit "yes". **The AI MUST NEVER execute `git commit` automatically**.
3. **Commit Message Suggestion**: Suggest a descriptive Conventional Commit message (e.g. `feat(...)`, `fix(...)`, `refactor(...)`). The user may accept or modify it.
4. **Interactive Menus**: Use interactive prompts (such as `ask_question`) whenever asking the user for confirmation.

---

## 3. Reference Context (Douglas Thain's B-minor)

- The AST design in `ast.h` and `ast.c` (`decl__create_*`, `stmt__create_*`, `expr__create_*`, `type__create`, `param_list__create`, `*__destroy`) is inspired by **Prof. Douglas Thain's B-minor compiler** (*Introduction to Compilers and Language Design*, University of Notre Dame).
- When looking for how AST nodes are wired up, check:
  - `ast.h` for struct definitions and constructor declarations.
  - `ast.c` for constructor implementations and destructor freeing logic.
  - Note: In `ast.c`, `Decl` nodes distinguish function vs variable by checking `self->type && self->type->kind == TYPE_FUNCTION`. Functions use `self->code` (`Stmt *`), variables use `self->value` (`Expr *`).

---

## 4. Current Work Focus

- When resuming work, check `docs/CURRENT_STATUS.md` first.
- The immediate priority in the codebase is completing the expression parser [`parser_expr()`](../parser_grammar.c) in `parser_grammar.c`.
