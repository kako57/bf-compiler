#include <stdlib.h>
#include "token.h"
#include "util.h"

TokenListNode *new_token(TokenType type, int times)
{
	TokenListNode *new_node =
		(TokenListNode *)malloc(sizeof(TokenListNode));

	if (new_node == NULL) {
		report_error("malloc failed");
	}

	new_node->token.type = type;
	new_node->token.times = times;
	new_node->next = new_node;
	new_node->prev = new_node;
	new_node->loop_body = NULL;
	return new_node;
}

TokenListNode *insert_token(TokenListNode *head, TokenListNode *new_node)
{
	if (new_node == NULL) {
		// why the fuck would you do this
		return head;
	}

	if (head == NULL) {
		new_node->next = new_node;
		new_node->prev = new_node;
		return new_node;
	}

	new_node->next = head;
	new_node->prev = head->prev;
	head->prev->next = new_node;
	head->prev = new_node;
	return head;
}

TokenListNode *delete_last_token(TokenListNode *head)
{
	if (head == NULL) {
		return NULL;
	}

	if (head->next == head) {
		free(head);
		return NULL;
	}

	TokenListNode *last = head->prev;
	last->prev->next = head;
	head->prev = last->prev;
	free(last);
	return head;
}

void move_token_to_loop(TokenListNode *node_with_loop_body,
			TokenListNode *target)
{
	if (node_with_loop_body == NULL) {
		report_error("node_with_loop_body is NULL");
	}

	if (target == NULL) {
		report_error("target is NULL");
	}

	// disconnect the node from the list
	target->prev->next = target->next;
	target->next->prev = target->prev;

	// insert the node into the loop
	node_with_loop_body->loop_body =
		insert_token(node_with_loop_body->loop_body, target);
}

TokenListNode *clear_tokens(TokenListNode *head)
{
	if (head == NULL) {
		return NULL;
	}

	TokenListNode *node = head;
	do {
		TokenListNode *next_node = node->next;
		node->loop_body = clear_tokens(node->loop_body);
		free(node);
		node = next_node;
	} while (node != head);

	return NULL;
}
