#ifndef TOKEN_H
#define TOKEN_H

typedef enum __tokentype {
	OPEN_BRACKET,
	CLOSE_BRACKET,
	PLUS,
	MINUS,
	MOVE_RIGHT,
	MOVE_LEFT,
	PRINT,
	READ,
} TokenType;

typedef struct __token {
	TokenType type;
	int times;
} Token;

typedef struct __token_list_node {
	Token token;
	struct __token_list_node *loop_body;
	struct __token_list_node *next;
	struct __token_list_node *prev;
} TokenListNode;

TokenListNode *new_token(TokenType type, int times);
TokenListNode *insert_token(TokenListNode *head, TokenListNode *new_node);
TokenListNode *delete_last_token(TokenListNode *head);
void move_token_to_loop(TokenListNode *node_with_loop_body, TokenListNode *target);
TokenListNode *clear_tokens(TokenListNode *head);

#endif // ifndef TOKEN_H
