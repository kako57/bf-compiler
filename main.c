#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "util.h"
#include "lexer.h"
#include "parser.h"
#include "optimum_pride.h"

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		return 1;
	}

	TokenListNode *head = tokenize(argv[1]);
	// loopify(head);

	// print_prog(head);
	// puts("");

	assemble(head, "output.asm");

	head = clear_tokens(head);

	system("nasm -f elf64 output.asm -o output.o");
	system("ld output.o -o output");

	system("rm output.asm output.o");

	return 0;
}
