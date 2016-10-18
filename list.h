#ifndef __LIST_H
#define __LIST_H
typedef struct node{
	char *str;
	struct node *prev, *next;
} node;

typedef struct bignum{
	int len;
	node *head, *tail;
} bignum;

void init(bignum *i);
int length(bignum *i);
void insert(bignum *i, char *str, int pos);
void traverse(bignum *i);
#endif
