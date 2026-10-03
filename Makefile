CC ?= gcc
CFLAGS ?= -Wall -Wextra -g -O0

SRCS = sol.c \
       parser_grammar.c \
       parser_util.c \
       ast.c \
       hash_table.c \
       semantic_analysis_util.c \
       name_resolution.c \
       asm_gen_util.c \
       asm_gen.c

OBJS = $(SRCS:.c=.o)
TARGET = solc

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c $(wildcard *.h)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
