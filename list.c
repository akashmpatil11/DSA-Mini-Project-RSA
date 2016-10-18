#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"list.h"

void init(bignum *l){
	l->tail = l->head = NULL;
	l->len = 0;
	return;
}

int length(bignum *l){
	return l->len;
}

void insert(bignum *l, char *str, int pos) {
	node *tmp, *p;
	int i;	
	if(pos < 0 || pos > l->len)
		return;	
	tmp = (node *)malloc(sizeof(node));
	tmp->str = (char*)malloc(4);
	strcpy(tmp->str, str);
	tmp->next = tmp->prev = NULL;

	if(l->head == NULL) {
		tmp->next = tmp->prev = NULL;
		l->head = l->tail = tmp;
		l->len++;
		return;		
	}

	p = l->head;
	if(pos == 0) {
		tmp->next = p;
		p->prev = tmp;	
		l->head = tmp;
	}
	
	else {
		for(i = 0; i < pos - 1; i++)
			p = p->next;
		if(pos == l->len) {
			tmp->next = p->next;
			tmp->prev = p;
			p->next = tmp;
			l->tail = tmp;
		}
		else {
			tmp->next = p->next;
			tmp->prev = p;
			p->next->prev = tmp;
			p->next = tmp;
		}
	}
	l->len++;
}

void traverse(bignum *l){
	node *p;
	p = l->head;
	do{
		printf("%s", p->str);
		p = p->next;
	} while(p != NULL);
	return;
}

