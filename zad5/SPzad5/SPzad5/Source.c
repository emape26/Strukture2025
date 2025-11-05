#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>


typedef struct postfix* Position;

typedef struct postfix {
	double number;
	struct postfix* next;

}postfix;

int Pop(Position head, char op);
void FreeList(Position head);
int Push(Position head, double num);
int main() {


	FILE* f = fopen("Postfix.txt", "r");
	if (f == NULL) {
		printf("Greska u otvaranju!");
		return EXIT_FAILURE;
	}

	Position head = (Position)malloc(sizeof(postfix));
	if (head == NULL) {
		printf("greska u alokaciji memorije!");
		fclose(f);
		return EXIT_FAILURE;
	}

	head->next = NULL;
	char help[64];

	while (fscanf(f, " %63s", help) == 1) {

		char* end;
		double value = strtod(help, &end);

		if (*end == '\0') {

			if (Push(head, value) != 0) {  //Push vrijednost u stog
				FreeList(head);
				free(head);
				fclose(f);
				return EXIT_FAILURE;
			}
		}
		else if (strlen(help) == 1 && strchr("+-*/", help[0]) != NULL) {
			if (Pop(head, help[0]) != 0) {
				FreeList(head);
				free(head);
				fclose(f);
				return EXIT_FAILURE;
			}
		}
		else {
			printf("Greska!");
			FreeList(head);
			free(head);
			fclose(f);
			return EXIT_FAILURE;
		}
	}	
		
		fclose(f);

		if (head->next == NULL || head->next->next != NULL) {
			printf("Nepoznat postfix!");
			FreeList(head);
			free(head);
			return EXIT_FAILURE;
		}
	
	
	double result = head->next->number;
	printf("Rezultat je: %.2f\n", result);


	FreeList(head);
	free(head);

	return 0;
}


int Push(Position head, double num) {

	Position newEl = (Position)malloc(sizeof(postfix));
	if (newEl == NULL) {
		printf("Greska u alookaciji memorije!\n");
		return EXIT_FAILURE;
	}
	
	newEl->number = num;
	newEl->next = NULL;

	Position tmp = head;
	newEl->next = tmp->next;
	tmp->next = newEl;

	return 0;
}

int Pop(Position head, char op) {

	Position tmp = head;


	Position first = tmp->next;
	if (first == NULL || first->next == NULL) {
		printf("Premalo elemenata u stogu!\n");
		return EXIT_FAILURE;
	}

	Position second = first->next;

	double op1 = second->number;
	double op2 = first->number;
	double result = 0.0;

	switch (op) {
	case '+':
		result = op1 + op2;
		break;
	case'-':
		result = op1 - op2;
		break;
	case'*':
		result = op1 * op2;
		break;
	case'/':
		result = op1 / op2;
		break;
	default:
		printf("Nepoznata operacija!");
		return EXIT_FAILURE;
	}
	
	tmp->next = second->next;   //micem ta dva sa stoga

	first->next = NULL;          //oslobadanje memorije
	second->next = NULL;
	free(first);
	free(second);

	Push(tmp, result);   //rezultat ubacujem u stog
		
	return 0;
}

void FreeList(Position head) {   //oslobadanje memorije
	Position p = head->next;
	Position tmp;
	while (p != NULL) {
		tmp = p->next;
		free(p);
		p = tmp;
	}
	head->next = NULL;
}
