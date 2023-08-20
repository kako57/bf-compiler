#include <stdio.h>
#include "lexer.h"
#include "util.h"

TokenListNode *tokenize(const char *filename)
{
	FILE *fp = fopen(filename, "r");
	char c;

	TokenListNode *head = NULL;
	int open_bracket_count = 0;

	while ((c = fgetc(fp)) != EOF) {
		switch (c) {
		case '[':
			head = insert_token(head, new_token(OPEN_BRACKET, 0));
			open_bracket_count++;
			break;
		case ']':
			if (head == NULL || open_bracket_count <= 0) {
				report_error("unmatched ]");
			}
			open_bracket_count--;
			head = insert_token(head, new_token(CLOSE_BRACKET, 0));
			break;
		case '+':
			if (head == NULL) {
				head = insert_token(head, new_token(PLUS, 1));
			} else if (head->prev->token.type == PLUS) {
				head->prev->token.times++;
				if (head->prev->token.times == 256) {
					delete_last_token(head);
				}
			} else if (head->prev->token.type == MINUS) {
				head->prev->token.times--;
				if (head->prev->token.times == 0) {
					head = delete_last_token(head);
				} else if (head->prev->token.times == -1) {
					head->prev->token.type = PLUS;
					head->prev->token.times = 1;
				}
			} else {
				head = insert_token(head, new_token(PLUS, 1));
			}
			break;
		case '-':
			if (head == NULL) {
				head = insert_token(head, new_token(MINUS, 1));
			} else if (head->prev->token.type == MINUS) {
				head->prev->token.times++;
				if (head->prev->token.times == 256) {
					delete_last_token(head);
				}
			} else if (head->prev->token.type == PLUS) {
				head->prev->token.times--;
				if (head->prev->token.times == 0) {
					head = delete_last_token(head);
				} else if (head->prev->token.times == -1) {
					head->prev->token.type = MINUS;
					head->prev->token.times = 1;
				}
			} else {
				head = insert_token(head, new_token(MINUS, 1));
			}
			break;
		case '>':
			if (head == NULL) {
				head = insert_token(head,
						    new_token(MOVE_RIGHT, 1));
			} else if (head->prev->token.type == MOVE_RIGHT) {
				head->prev->token.times++;
			} else if (head->prev->token.type == MOVE_LEFT) {
				head->prev->token.times--;
				if (head->prev->token.times == -1) {
					head->prev->token.type = MOVE_RIGHT;
					head->prev->token.times = 1;
				}
			} else {
				head = insert_token(head,
						    new_token(MOVE_RIGHT, 1));
			}
			break;
		case '<':
			if (head == NULL) {
				head = insert_token(head,
						    new_token(MOVE_LEFT, 1));
			} else if (head->prev->token.type == MOVE_LEFT) {
				head->prev->token.times++;
			} else if (head->prev->token.type == MOVE_RIGHT) {
				head->prev->token.times--;
				if (head->prev->token.times == -1) {
					head->prev->token.type = MOVE_LEFT;
					head->prev->token.times = 1;
				}
			} else {
				head = insert_token(head,
						    new_token(MOVE_LEFT, 1));
			}
			break;
		case '.':
			if (head == NULL) {
				head = insert_token(head, new_token(PRINT, 1));
			} else if (head->prev->token.type == PRINT) {
				head->prev->token.times++;
			} else {
				head = insert_token(head, new_token(PRINT, 1));
			}
			break;
		case ',':
			if (head == NULL) {
				head = insert_token(head, new_token(READ, 1));
			} else if (head->prev->token.type == READ) {
				head->prev->token.times++;
			} else {
				head = insert_token(head, new_token(READ, 1));
			}
			break;
		default:
			break;
		}
	}
	fclose(fp);

	if (open_bracket_count < 0) {
		report_error("unmatched ]");
	}

	if (open_bracket_count > 0) {
		fprintf(stderr, "%d unmatched [", open_bracket_count);
		report_error("unmatched [");
	}

	return head;
}

void print_prog(TokenListNode *head)
{
	if (head == NULL) {
		return;
	}

	TokenListNode *node = head;
	do {
		switch (node->token.type) {
		case OPEN_BRACKET:
			printf("[");
			print_prog(node->loop_body);
			break;
		case CLOSE_BRACKET:
			printf("]");
			break;
		case PLUS:
			for (int i = 0; i < node->token.times; i++)
				printf("+");
			break;
		case MINUS:
			for (int i = 0; i < node->token.times; i++)
				printf("-");
			break;
		case MOVE_RIGHT:
			for (int i = 0; i < node->token.times; i++)
				printf(">");
			break;
		case MOVE_LEFT:
			for (int i = 0; i < node->token.times; i++)
				printf("<");
			break;
		case PRINT:
			for (int i = 0; i < node->token.times; i++)
				printf(".");
			break;
		case READ:
			for (int i = 0; i < node->token.times; i++)
				printf(",");
			break;
		default:
			break;
		}

		node = node->next;
	} while (node != head);
}
