// currently unused
// but maybe useful in the future
// for compiler optimizations

#include "parser.h"
#include "util.h"

void loopify(TokenListNode *head)
{
	// after tokenize, we get a flat list of tokens
	// we need to convert it to a tree of tokens
	// where there may be nested loops
	if (head == NULL) {
		return;
	}

	TokenListNode *node = head;
	int open_bracket_count = 0;

	do {
		if (node->token.type == OPEN_BRACKET) {
			open_bracket_count++;

			TokenListNode *loop_instr = node->next;
			int depth = 1;
			while (1) {
				TokenListNode *next_node = loop_instr->next;

				if (loop_instr->token.type == OPEN_BRACKET) {
					depth++;
				} else if (loop_instr->token.type ==
					   CLOSE_BRACKET) {
					depth--;
				}

				if (depth == 0) {
					break;
				}

				move_token_to_loop(node, loop_instr);
				loop_instr = next_node;
				if (loop_instr == head) {
					report_error("unmatched '['");
				}
			}

			loopify(node->loop_body);
		} else if (node->token.type == CLOSE_BRACKET) {
			if (open_bracket_count == 0) {
				report_error("unmatched ']'");
			}
			open_bracket_count--;
		}

		node = node->next;
	} while (node != head);
}

