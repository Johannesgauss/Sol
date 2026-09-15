# Agent Git & Commit Workflow Guidelines

This document defines the strict workflow that any AI agent working on this repository must follow regarding staging and committing changes.

## 1. Manual Staging
- **The user will manually stage all changes** (e.g., using `git add`).
- **The AI MUST NOT run `git add`** or stage files automatically under any circumstance unless explicitly instructed by the user.

## 2. Commit Confirmation Gate
- When changes are ready or requested to be committed, the AI **MUST explicitly ask the user for permission** to proceed with committing.
- The AI must wait for the user to say "yes" (or give explicit consent) before proceeding toward any commit step.
- The AI **MUST NEVER execute `git commit` automatically**.

## 3. Commit Message Suggestion
- After the user confirms they want to proceed with committing, the AI must **suggest a clear and descriptive commit message** (following conventional commits matching the repository's history, e.g., `feat(...)`, `fix(...)`, `refactor(...)`).
- The user may agree with the suggested message, request adjustments, or provide their own commit message.
- The commit is only executed (or provided to the user) once the user agrees on the final message.

## 4. Interactive Menus
- Confirmation gates and commit message selection should be presented via interactive selection menus (using `ask_question`) whenever prompting the user.

