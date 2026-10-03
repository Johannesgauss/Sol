#include "ast.h"
#include "asm_gen_util.h"
#include "parser_grammar.h"
#include "name_resolution.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	if (argc < 2) return -1;

	FILE *src_file = fopen(argv[1], "r");
	fseek(src_file, 0, SEEK_END);
	unsigned long long int file_size = ftell(src_file);
	char *src = malloc(file_size+1);
	rewind(src_file);
	fread(src, 1, file_size, src_file);
	src[file_size] = '\0';
	fclose(src_file);

	Decl *root = parser_program(src);
	resolve(root);

	char asm_file[256];
	char exe_file[256];
	char cmd[1024];

	char *dot = strrchr(argv[1], '.');
	int base_len = dot ? (int)(dot - argv[1]) : (int)strlen(argv[1]);

	snprintf(asm_file, sizeof(asm_file), "%.*s.s", base_len, argv[1]);
	snprintf(exe_file, sizeof(exe_file), "%.*s", base_len, argv[1]);

	file = fopen(asm_file, "w");
	if (!file) {
		fprintf(stderr, "Error creating output file %s\n", asm_file);
		return -1;
	}
	decl_codegen(root);
	fclose(file);

	snprintf(cmd, sizeof(cmd), "gcc %s -o %s", asm_file, exe_file);
	system(cmd);

	return 0;
}
