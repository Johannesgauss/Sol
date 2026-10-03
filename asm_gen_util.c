#include "asm_gen_util.h"
#include "ast.h"
#include "semantic_analysis_util.h"
#include <stdio.h>
//#define RTABLE_SIZE 7 #define RTABLE_INIT {0,0,0,0, 0,0,0}

FILE *file = NULL;
int rtable = 0;

int scratch_alloc()
{
	int i; for (i = 1; rtable & i; i=i<<1);
	if (i > R15) return -1;
	rtable |= i;
	return i;
}

inline void scratch_free(int reg)
{
	rtable &= ~reg;
}

const char *scratch_name(int reg)
{
        switch (reg) {              
        case RBX: return "%rbx";  
        case R10: return "%r10";  
        case R11: return "%r11";  
        case R12: return "%r12";  
        case R13: return "%r13";  
        case R14: return "%r14";  
        case R15: return "%r15";  
        default:  return "ERROR!";         
        }
}

ullint label_counter = 0;
inline ullint label_create()
{
	return label_counter++;
}

const char *label_name(int label)
{
	// It has a better way, don't forget it!
	static char buffer[32];
	snprintf(buffer, sizeof(buffer), ".L%d", label);
	return buffer;
}

const char *symbol_codegen(Symbol *symbol)
{
	if (symbol->kind == SYMBOL_GLOBAL || symbol->kind == SYMBOL_EXTERN) {
		static char buffer[128];
		snprintf(buffer, sizeof(buffer), "%.*s", (int)symbol->name.size, symbol->name.data);
		return buffer;
	}
	if (symbol->kind == SYMBOL_PARAM) {
		switch (symbol->which) {
		case 0: return "%rdi";
		case 1: return "%rsi";
		case 2: return "%rdx";
		case 3: return "%rcx";
		case 4: return "%r8";
		case 5: return "%r9";
		default: return "ERROR!";
		}
	}	
	static char buffer[sizeof(symbol->which)*8];
	snprintf(buffer, sizeof(buffer), "-%d(%%rbp)", symbol->which*8);

	return buffer;
}

void emit(const char *fmt, ...)
{

	if (!file) file = stdout;

	va_list args;
	va_start(args, fmt);

	vfprintf(file, fmt, args);
	fprintf(file, "\n");

	va_end(args);
}

//void emit_label(const char *label) { if (!file) return; fprintf(file, "%s:\n", label); }
