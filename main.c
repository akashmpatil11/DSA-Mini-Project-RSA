#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limit.h>
#include "list.h"
#define MAX_DIGITS 100
#define ACCURACY 10
#define RANDOM_SIZE 8


/*Addition of two bignum*/
void Addition_bignum(bignum *m, bignum *n, bignum *result) {
	int carry = 0, sum = 0;
	char str[4];
	node *p, *q;
	if(m->len == n->len) {
		p = m->tail;
		q = n->tail;
		while(p) {
			sum = atoi(p->str) + atoi(q->str) + carry;
			carry = sum / 10000;
			sum = sum % 10000;
			sprintf(str,"%d", sum);
			num(str);
			insert(result, str, 0);
			q = q->prev;
			p = p->prev;
		}
		if(carry) {
			sprintf(str,"%d", carry);
			num(str);
			insert(result, str, 0);	
		}
	}
	else {
		if(m->len > n->len) {
			p = m->tail;
			q = n->tail;
		}
		else {
			q = m->tail;
			p = n->tail;
		}
		while(q) {
			sum = atoi(p->str) + atoi(q->str) + carry;
			carry = sum / 10000;
			sum = sum % 10000;
			sprintf(str,"%d", sum);
			num(str);
			insert(result, str, 0);
			q = q->prev;
			p = p->prev;
		}
		while(p) {
			sum = atoi(p->str) + carry;
			carry = sum / 10000;
			sum = sum % 10000;
			sprintf(str,"%d", sum);
			num(str);
			insert(result, str, 0);
			p = p->prev;
		}
	}
	return;
}

/*multiply two bignums*/
void Multiplication_bignum(bignum *m, bignum *n, bignum *result) {
	
	int carry = 0, mul, count = 0, i;
	char str[4];	
	node *p, *q;
	bignum tmp1, tmp2, tmpresult;
	init(&tmp1);
	init(&tmp2);
	p = m->tail;
	q = n->tail;
	while(q) {
		init(&tmp2);
		carry = 0;
		if(!count) {
			while(p) {
				mul = atoi(p->str) * atoi(q->str) + carry;
				carry = mul / 10000;
				mul = mul % 10000;
				sprintf(str,"%d", mul);
				num(str);
				insert(&tmp1, str, 0);
				p = p->prev;
			}
			if(carry) {
				sprintf(str,"%d", carry);
				num(str);
				insert(&tmp1, str, 0);
			}
		}
		else {
			init(&tmpresult);
			for(i = 0; i < count; i++) {
				strcpy(str,"0000");
				insert(&tmp2, str, tmp2.len);
			}
			while(p) {
				mul = atoi(p->str) * atoi(q->str) + carry;
				carry = mul / 10000;
				mul = mul % 10000;
				sprintf(str,"%d", mul);
				num(str);
				insert(&tmp2, str, 0);
				p = p->prev;
			}
			if(carry) {
				sprintf(str,"%d", carry);
				num(str);
				insert(&tmp2, str, 0);
			}
			Addition_bignum(&tmp1, &tmp2, &tmpresult);
			init(&tmp1);
			Copy_bignum(&tmpresult, &tmp1);
		}
		p = m->tail;
		q = q->prev;
		count++;
	}
	Copy_bignum(&tmp1, result);
}

/*bignum(n) divide by 2*/
void Divide2_bignum(bignum *n) {
	int m, rem = 0, l;
	char str[4];
	node *p = n->head;
	while(p) {	
		m = atoi(p->str);
		if(rem)
			m = m + rem * 10000;
		rem = m % 2;
		m = m / 2;
		sprintf(str,"%d", m);
		l = strlen(str);
		num(str, l);
		strcpy(p->str, str);
		p = p->next;
	}
	return;
}

/*checks bignum(n) modulas 2*/
int Modulus2_bignum(bignum *n) {
	if(!(n->tail->str)[3] % 2)
		return 1;
	else
		return 0;
}

/*Substracting 1 from bignum*/
void Substract1_bignum(bignum *n) {
	int a;
	a = atoi((n->tail->str));
	a--;
	sprintf((n->tail->str), "%d", a);
	return;
}

/*coping one bignum(n) to other bignum(b)*/
void Copy_bignum(bignum *a, bignum *b) {
	node *p;
	p = a->head;
	while(p != NULL) {
		insert(b, p->str, b->len);
		p = p->next;
	}
}

int MillerRabinTest(bignum *d, bignum *n)
{
	bignum a;
	init(&a);
	int m, n, l;
	/*Pick random number*/
	/*First digit should be non-zero*/
	do {
		m = random() % 10;
	} while(!m);
	n = m;
	for(i = 0; i < 3; i++) {
		m = random() % 10;
		n = n * 10 + m;	
	}
	sprintf(str,"%d", n);
	l = strlen(str);
	checknum(str, l);
	insert(&a, str, a.len);

	n = random() % 10000;
	sprintf(str,"%d", n);
	l = strlen(str);
	checknum(str, l);
	insert(&a, str, a.len);
	/*Compute a^d % n*/
}

/*checking bignum is prime or not using Miller-Rabin primilarly test*/
int IsPrime(bignum *n) {
	bignum d;
	init(&d);
	int i;
	Copy_bignum(n, &d);
	Substract1_bignum(&d);
	while(!Modulus2_bignum(&d)) {
		Divide2_bignum(&d);
	}

	for(i = 0; i < ACCURACY; i++) {
		if(MillerRabinTest(&d, n) == 0)
			return 0;
	}
	return 1;
}

void checknum(char *str) {
	int l;
	if(l == 3){
		str[3] = str[2];
		str[2] = str[1];
		str[1] = str[0];
		str[0] = '0';
	}
	else if(l == 2){
		str[3] = str[1];
		str[2] = str[0];
		str[1] = '0';
		str[0] = '0';
	}
	else if(l == 1){
		str[3] = str[0];
		str[2] = '0';
		str[1] = '0';
		str[0] = '0';
	}
	else if(l == 0){
		str[3] = '0';
		str[2] = '0';
		str[1] = '0';
		str[0] = '0';
	}
	else
		return;
}

bignum GeneratePrime(int max, bignum *p) {
	int i, n = 0, m;
	char str[4];
	while(1) {
		/*create random 100 digit num*/
		/*First digit should be non-zero*/
		do {
			m = random() % 10;
		} while(!m);
		n = m;
		for(i = 0; i < 3; i++) {
			m = random() % 10;
			n = n * 10 + m;	
		}
		sprintf(str,"%d", n);
		checknum(str);
		insert(&p, str, p.len);
	
		for(i = 0; i < (100 / 4)- 2; i++) {
			n = random() % 10000;
			sprintf(str,"%d", n);
			checknum(str);
			insert(&p, str, p.len);
		}
		for(i = 0, n =0; i < 3; i++) {
			m = random() % 10;
			n = n * 10 + m;	
		}
		/*Last digit should be odd*/
		do {
			m = random() % 10;
		} while(!(m % 2));
		n = n * 10 + m;
		sprintf(str,"%d", n);
		checknum(str);
		insert(&p, str, p.len);
		/*bignum created*/

		if(IsPrime(&p))
			return p;
	}
}

int main() {
	/*variable decleration*/
	time_t tt;
	bignum p, q;
	srandom(time(&tt));
	init(&p);
	init(&q);
	GeneratePrime(MAX_DIGITS, p);
	GeneratePrime(MAX_DIGITS, q);
	printf("prime generated ")







}
