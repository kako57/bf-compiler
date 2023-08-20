#include <stdio.h>
#include <stdlib.h>
#include "optimum_pride.h"
#include "util.h"

typedef struct __loop_stack {
	int loopnum;
	struct __loop_stack *next;
} LoopStack;

LoopStack *push_stack(LoopStack *head, int num) {
	LoopStack *new_node = malloc(sizeof(LoopStack));
	if (new_node == NULL)
		report_error("malloc failed");
	new_node->loopnum = num;
	new_node->next = head;

	return new_node;
}

LoopStack *pop_stack(LoopStack *head) {
	if (head == NULL)
		return NULL;
	LoopStack *next = head->next;
	free(head);
	return next;
}


void assemble(TokenListNode *head, const char *filename)
{
	LoopStack *loop_stack = NULL;
	freopen(filename, "w", stdout);

	printf("section .data\n");
	printf("\tbuf: times 30000 db 0\n");
	printf("section .text\n");
	printf("global _start\n");

	printf("_start:\n");
	// initialize r8
	// so r8 will point to the first cell of buf
	printf("\tmov r8, buf\n");

	int loop_num = 0, tmp;


	TokenListNode *cur = head;

	do {
		switch (cur->token.type) {
		case PLUS:
			printf("\tadd byte [r8], %d\n", cur->token.times);
			break;
		case MINUS:
			printf("\tsub byte [r8], %d\n", cur->token.times);
			break;
		case OPEN_BRACKET:
			printf("loop%d:\n", loop_num);
			printf("\tcmp byte [r8], 0\n");
			printf("\tje endloop%d\n", loop_num);
			loop_stack = push_stack(loop_stack, loop_num++);
			break;
		case CLOSE_BRACKET:
			tmp = loop_stack->loopnum;
			loop_stack = pop_stack(loop_stack);
			printf("\tjmp loop%d\n", tmp);
			printf("endloop%d:\n", tmp);
			break;
		case MOVE_LEFT:
			printf("\tsub r8, %d\n", cur->token.times);
			break;
		case MOVE_RIGHT:
			printf("\tadd r8, %d\n", cur->token.times);
			break;
		case READ:
			printf("\tmov rdi, 0\n");
			printf("\tmov rsi, r8\n");
			printf("\tmov rdx, 1\n");
			for (int i = 0; i < cur->token.times; i++) {
				printf("\tmov rax, 0\n"); // sys_read
				printf("\tsyscall\n");
			}
			break;
		case PRINT:
			printf("\tmov rdi, 1\n");
			printf("\tmov rsi, r8\n");
			printf("\tmov rdx, 1\n");
			for (int i = 0; i < cur->token.times; i++) {
				printf("\tmov rax, 1\n"); // sys_write
				printf("\tsyscall\n");
			}
			break;
		default:
			break;
		}
		cur = cur->next;
	} while (cur != head);

	printf("\tmov rax, 60\n"); // sys_exit
	printf("\tmov rdi, 0\n");
	printf("\tsyscall\n");

	fclose(stdout);
}
