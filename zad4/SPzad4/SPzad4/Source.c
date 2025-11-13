#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>

typedef struct _Polinom* listPoli;
typedef struct _Polinom {
	int koef;
	int pot;
	listPoli next;
}Polinom;

int loadPolinom(listPoli head, int lineNumber);
void eraseAll(listPoli head);
int produkt(listPoli p1, listPoli p2, listPoli p);
int suma(listPoli p1, listPoli p2, listPoli s);
int print(char* name, listPoli P);
int Insert(listPoli head, int koef, int potencija);
int main() {


	Polinom headA = { 0,0,NULL };
	Polinom headB = { 0,0,NULL };
	Polinom headC = { 0,0,NULL };
	Polinom headProdukt = { 0,0,NULL };

	loadPolinom(&headA, 1);
	loadPolinom(&headB, 2);


	print("A", headA.next);
	print("B", headB.next);

	suma(headA.next, headB.next, &headC);
	print("A+B", headC.next);

	produkt(headA.next, headB.next, &headProdukt);
	print("A*B", headProdukt.next);

	eraseAll(&headA);
	eraseAll(&headB);
	eraseAll(&headC);
	eraseAll(&headProdukt);

}
int Insert(listPoli head, int koef, int potencija) {

	listPoli prev = head;
	listPoli q = head->next;

	while (q != NULL && q->pot > potencija) { //trazim mjesto di umetnut potencijiu po opadajucim vrijednostima
		prev = q;
		q = q->next;
	}

	if (q != NULL && q->pot == potencija) {   //aok smo naisli na istu potenciju onda ih samo zbrojimo
		q->koef += koef;

		if (q->koef == 0) { //ako je zboj 0 brisemo tu potenciju
			prev->next = q->next;
			free(q);
		}
		return 0;
	}

	listPoli new = (listPoli)malloc(sizeof(Polinom));
	if (new == NULL) {
		printf("Greska pri alokaciji memorije!\n");
		return EXIT_FAILURE;
	}

	new->koef = koef;   
	new->pot = potencija;
	new->next = q;
	prev->next = new;

	return 0;
}

int print(char* name, listPoli P) {
	
	printf("%s(x) = ", name);

	if (P == NULL) {
		printf("0\n");
		return 0;
	}
	int first = 1;           //jos nije ispisan prvi clan
	while (P != NULL) {
		int k = P->koef;
			
		if (!first) {         //ako nije prvi clan polinoma
			if (k >= 0)
				printf(" + ");
			else {
				printf(" - ");
				k = -k;
			}
		}
		else {                   //ako je prvi clan polinoma
			if (k < 0) {
				printf("-");
				k = -k;
			}
			first = 0;
		}
	
		printf("%d", k);
		if (P->pot != 0)
			printf("x^%d", P->pot);
	
		P = P->next;
	}
	printf("\n");
	return 0;

}
int suma(listPoli p1, listPoli p2, listPoli s) {

	listPoli tmp;

	while (p1 != NULL && p2 != NULL) {
		if (p1->pot == p2->pot) {
			Insert(s, p1->koef + p2->koef, p1->pot);
			p1 = p1->next;
			p2 = p2->next;
		}
		else if (p1->pot > p2->pot) {
			Insert(s, p1->koef, p1->pot);
			p1 = p1->next;
		}
		else {
			Insert(s, p2->koef, p2->pot);
			p2 = p2->next;
		}
	}
	if (p1 != NULL) tmp = p1;
	else tmp = p2;

	while (tmp != NULL) {
		Insert(s, tmp->koef, tmp->pot);
		tmp = tmp->next;
	}

	return 0;

}
int produkt(listPoli p1, listPoli p2, listPoli p) {
	
	listPoli i, j;

	for (i = p1;i != NULL;i = i->next) {
		for (j = p2; j != NULL; j = j->next) {
			int Koef = i->koef * j->koef;
			int pot = i->pot + j->pot;
			Insert(p, Koef, pot);   
		}
	}
	return 0;
}

void eraseAll(listPoli head) { //brisanje liste

	listPoli p = head->next;
	while (p != NULL) {
		listPoli tmp = p;
		p = p->next;
		free(tmp);
	}
	head->next = NULL;

}
int loadPolinom(listPoli head,int lineNumber) {

	FILE* f = fopen("Polinomi.txt", "r");
	if (f == NULL) {
		printf("Greska u otvaranju datoteke!\n");
		return EXIT_FAILURE;
	}

	char buffer[256];
	int currentLine = 1;

	
	while (fgets(buffer, sizeof(buffer), f) != NULL) { //citam jedan red i spreman u buffer
		if (currentLine == lineNumber)
			break;
		currentLine++;  
	}

	fclose(f);

	
	char* p = buffer;
	int koef, pot, last;

	while (sscanf(p, "%d %d%n", &koef, &pot, &last) == 2) {
		Insert(head, koef, pot);
		p += last;    //pomaknem se za onoliok kolko je procitano znakova
	}

	return 0;
}
