#ifndef SOL_ASM_GEN_UTIL_H
#define SOL_ASM_GEN_UTIL_H

#include "alias.h"
#include "semantic_analysis_util.h"
#include <stdarg.h>
#include <stdio.h>

typedef enum {
	RBX = 0b00000001,
	R10 = 0b00000010,
	R11 = 0b00000100,
	R12 = 0b00001000,
	R13 = 0b00010000,
	R14 = 0b00100000,
	R15 = 0b01000000,
} registers;

int scratch_alloc(void);
void scratch_free(int reg);
const char *scratch_name(int reg);

ullint label_create(void);
const char *label_name(int label);

const char *symbol_codegen(Symbol *symbol);

void emit(const char *fmt, ...);

//void emit_label(const char *label);

#endif // SOL_ASM_GEN_UTIL_H
