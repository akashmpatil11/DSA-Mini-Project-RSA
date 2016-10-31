#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include <time.h>
#define ACCURACY 10
#define PRIME_DIGITS 100
#define EXP_SIZE 8

/*Constant declaration*/
mpz_t zero, one, two;

/*Function For Encryption*/
void Cipher(FILE *fp1, FILE *fp2, mpz_t e, mpz_t n) {
	char ch;
	mpz_t a;
	while(fscanf(fp1, "%c", &ch) != EOF) {
		mpz_init(a);
		mpz_set_ui(a, ch);
		mpz_powm(a, a, e, n);
		/*Prints encrcrypted message in file*/
		gmp_fprintf(fp2, "%Zd ", a);
		mpz_clear(a);
	}
}

/*Function to Decryption*/
void AntiCipher(FILE *fp1, FILE *fp2, mpz_t d, mpz_t n) {
	mpz_t a;
	mpz_init(a);
	char *ch;
	int m;
	ch = (char*)malloc(sizeof(char) * 4);
	while(gmp_fscanf(fp1, "%Zd", a) != EOF) {
		mpz_powm(a, a, d, n);
		mpz_get_str (ch, 10, a);
		m = atoi(ch);
		/*Prints decrypted message in file*/
		fprintf(fp2, "%c", m);
		mpz_clear(a);
		mpz_init(a);
	}
	free(ch);
}

/*Function to generate Public Key (e)*/
void PublicExponent(mpz_t phi, mpz_t e) {
	int i;
	mpz_t n, gcd;
	mpz_init(n);
	char *str;
	str = (char*)malloc(sizeof(char) * (EXP_SIZE + 1));
	str[0] = (rand() % 9) + '0';
	for(i = 1; i < EXP_SIZE - 1; i++)
		str[i] = (rand() % 10) + '0';
	str[EXP_SIZE - 1] = ((rand() % 5) * 2) + 1 + '0';
	str[EXP_SIZE] = '\0';
	mpz_set_str(n, str, 10);
	while(1) {
		mpz_init(gcd);
		mpz_gcd(gcd, phi, n);
		if(!mpz_cmp(gcd, one)) {
			mpz_set(e, n);
			mpz_clear(n);
			mpz_clear(gcd);
			return;
		}
		mpz_add(n, n, two);
	}
}

/*Miller_Rabin Function*
 *Returns 1 if test is true
 *else returns 0
 */
int MillerRabinTest(mpz_t d, mpz_t n)
{
	int i;
	char *str;
	str = (char*)malloc(sizeof(char) * 9);
	mpz_t a, x, n1;
	mpz_init(a);
	mpz_init(x);
	mpz_init(n1);

	str[0] = (rand() % 9) + '0';
	for(i = 1; i < 7; i++)
		str[i] = (rand() % 10) + '0';
	str[8] = '\0';
	mpz_set_str(a, str, 10);

	mpz_powm (x, a, d, n);
	mpz_sub(n1, n, one);

	if(!(mpz_cmp(x, one)) || !(mpz_cmp(x, n1))) {
		mpz_clear(a);
		mpz_clear(x);
		mpz_clear(n1);
		return 1;
	}

	while(mpz_cmp(d, n1) != 0) {
		mpz_powm(x, x, two, n);
		mpz_mul(d, d, two);
		if(mpz_cmp(x, one) == 0) {
			mpz_clear(a);
			mpz_clear(x);
			mpz_clear(n1);
			return 0;
		}
		if(mpz_cmp(x, n1) == 0) {
			mpz_clear(a);
			mpz_clear(x);
			mpz_clear(n1);
			return 1;
		}	
	}
	return 0;
}

/*checking bignum is prime or not using Miller-Rabin primilarly test*/
int IsPrime(mpz_t n) {
	mpz_t d;
	mpz_init(d);
	int i;
	mpz_sub(d, n, one);  /*d = n - 1*/
	while(mpz_even_p(d))
		mpz_div(d, d, two);

	for(i = 0; i < ACCURACY; i++) {
		if(!(MillerRabinTest(d, n))) {
			mpz_clear(d);
			return 0;
		}
	}
	mpz_clear(d);
	return 1;
}

/*Function to generate Prime number
 *Prime number is of 100 digits
 */
void GeneratePrime(mpz_t p) {
	srand(time(NULL));
	int i;
	char *str;
	str = (char*)malloc(sizeof(char) * (PRIME_DIGITS + 1));

	str[0] = (rand() % 9) + '1';
	for(i = 1; i < (PRIME_DIGITS - 1); i++)
		str[i] = rand() % 10 + '0';
	str[PRIME_DIGITS - 1] = (rand() % 5) * 2 + '1';
	str[PRIME_DIGITS] = '\0';
	mpz_set_str(p, str, 10);
	
	while(1) {
		if(IsPrime(p)) {
			free(str);
			return;
		}
		mpz_add(p, p, two);
	}
}

int main(int argc, char *argv[]) {

	FILE *fp1, *fp2, *fp3;

	if((argc != 5) || (argc == 2 && !strcmp(argv[1], "-h"))) {
		printf("for help: ./program -h\n");
		printf("for encryption: usage: ./program -e <filename1> <filename2> <filename3>\n");
		printf("filename1: file to be encrypted\nfilename2: encrypted file\nfilename3: To save Private key(d) and n\n");
		printf("for decryption: usage: ./program -d <filename1> <filename2> <filename3>\n");
		printf("filename1: file to be decrypted\nfilename2: decrypted file\nfilename3: To get Private key(d) and n\n");
		return 0;
	}

	if(!strcmp(argv[1], "-e")) {
		fp1 = fopen(argv[2], "r");
		if(!fp1) {
			printf("Open: %s failed", argv[1]);
			return 0;
		}
		fp2 = fopen(argv[3], "w");
		fp3 = fopen(argv[4], "w");
		srand(time(NULL));
		/*variable declaration*/
		mpz_t p, q, n, phi, p1, q1, e, d;
		mpz_init(zero);
		mpz_init(one);
		mpz_init(two);
		mpz_init(p);
		mpz_init(q);
		mpz_init(n);
		mpz_init(phi);
		mpz_init(p1);
		mpz_init(q1);
		mpz_init(e);
		mpz_init(d);
		mpz_set_ui(zero, 0);
		mpz_set_ui(one, 1);
		mpz_set_ui(two, 2);
		/*Generate first prime number*/
		GeneratePrime(p);
		/*
		 *Genarate second prime number
		 *Note:Primes should be distict 	
		 */
		do{
			GeneratePrime(q);
		} while(mpz_cmp(p, q) == 0);
		/*
		 *n = p * q
		 */
		mpz_mul(n, p, q);
		mpz_sub(p1, p , one);
		mpz_sub(q1, q, one);
		/*
		 * Totient phi(n) = (p - 1) * (q - 1)
		 */
		mpz_mul(phi, p1, q1);
		/*
		 * Public Key(e, n) generated
		 */
		PublicExponent(phi, e);
		/*
		 * Private Key(e, n) generated
		 */
		mpz_invert (d, e, phi);
		/*Private key saved in filename3*/
		gmp_fprintf(fp3, "%Zd ", d);
		gmp_fprintf(fp3, "%Zd ", n);
		Cipher(fp1, fp2, e, n);
		mpz_clear(p);
		mpz_clear(q);
		mpz_clear(n);
		mpz_clear(phi);
		mpz_clear(p1);
		mpz_clear(q1);
		mpz_clear(e);
		mpz_clear(d);
		fclose(fp1);
		fclose(fp2);
		fclose(fp3);
		}
	else if(!strcmp(argv[1], "-d")) {
		mpz_t d, n;
		mpz_init(d);
		mpz_init(n);
		fp1 = fopen(argv[2], "r");
		if(!fp1) {
			printf("Open: %s failed", argv[1]);
			return 0;
		}
		fp2 = fopen(argv[3], "w");
		fp3 = fopen(argv[4], "r");
		if(!fp1) {
			printf("Open: %s failed", argv[3]);
			return 0;
		}
		gmp_fscanf(fp3, "%Zd", &d);
		gmp_fscanf(fp3, "%Zd", &n);
		AntiCipher(fp1, fp2, d, n);
		mpz_clear(d);
		mpz_clear(n);
	}
	return 0;
}
