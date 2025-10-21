#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX 50

struct _person{
	char name[MAX];
	char surname[MAX];
	int birthYear;
	
	struct _person* next;

}typedef Person;

Person* Delete_by_surname(Person* head, char* s);
void Find_by_surname(Person* head, char* s);
Person* Insert_at_end(Person* head);
void printList(Person* head);
Person* Insert_at_start(Person* head);
void InsertData(Person* new);

int main() {

	Person* head = NULL;      //Krecem s praznom listom
	int num,casenum;
	char help[MAX];

	printf("Unesi broj clanova za pocetnu listu: ");
	scanf("%d", &num);

	for (int i = 0;i < num;i++) {
		head = Insert_at_end(head);
	}

	printList(head);     //Ispis pocetne liste

	do {
		printf(
			"\nUnesi broj uz radnju koju zelis napraviti:\n"
			"1.Unos na pocetku liste\n"
			"2.Ispis Liste\n"
			"3.Unos na kraju liste\n"
			"4.Pronaci element po prezimenu\n"
			"5.Izbrisati odredeni element\n"
			"6.Zavrsiti s uredivanjem liste\n\n"  //Ponavlja se dok korisnik ne unese 6
		);

		scanf("%d", &casenum);

		switch (casenum) {
			case 1:
				printf("Koliko elemenata zelis unijeti na pocetak: ");
				scanf("%d", &num);
				for (int i = 0;i < num;i++) {
					head = Insert_at_start(head);
				}
				break;

			case 2:
				printList(head);
				break;

			case 3:
				printf("Koliko elemenata zelis unijeti na kraj: ");
				scanf("%d", &num);
				for (int i = 0;i < num;i++) {
					head = Insert_at_end(head);
				}
				break;

			case 4:
				printf("Unesi prezime koje zelis pronaci: ");
				scanf("%s", help);
				Find_by_surname(head, help);
				break;

			case 5:
				printf("Unesi prezime koje zelis izbrisat: ");
				scanf("%s", help);
				head = Delete_by_surname(head, help);
				break;

			case 6:
				printf("\nZavrseno s radom.");
				break;

			default:
				printf("\nNepostojeca opcija");
				break;
		}
	} while (casenum != 6);


	return 0;
}

Person* Insert_at_start(Person * head) {

	Person* new = (Person*)malloc(sizeof(Person));
	if (new == NULL) {
		printf("Greska u alokaciji memorije!\n");
		return head;
	}

	InsertData(new);
	new->next = head;    //Novi element pokazuje na head koji pokazuje na ostatak liste
	
	return new;
}

void InsertData(Person * new) {

	printf("Unesi ime, prezime i godinu rodenja: ");
	scanf("%s %s %d", new->name, new->surname, &new->birthYear);
}

void printList(Person*head) {

	Person* tmp = head;
	int i = 1;
	while (tmp != NULL) {
		printf("%d. %s %s %d\n",i, tmp->name, tmp->surname, tmp->birthYear);
		tmp = tmp->next;
		i++;
	}
	printf("\n");
}

Person* Insert_at_end(Person* head) {

	Person* new = (Person*)malloc(sizeof(Person));
	if (new == NULL) {
		printf("Greska u alokaciji memorije!\n");
		return head;
	}
	InsertData(new);

	new->next = NULL;
	Person* tmp = head;
	if (head == NULL)  //Ako je prazna lista onda je novi element prvi
		return new;
	while (tmp->next != NULL) {  //Ako lista ima elemente, idemo do zadnjeg elementa koji pokazuje na NULL
		tmp = tmp->next;
	}
	tmp->next = new;

	return head;
}

void Find_by_surname(Person* head, char* s) {

	Person* tmp = head;
	int position= 1;
	int found = 0;
	
	while (tmp != NULL) {
		if (strcmp(tmp->surname, s) == 0) {
			printf("Osoba se nalazi na mjestu %d: %s %s %d\n", position, tmp->name, tmp->surname, tmp->birthYear);
			found = 1;
		}
		tmp = tmp->next;
		position++;
	}
	if (found == 0)
	printf("Osoba %s nije pronadena na listi.\n",s);
}

Person* Delete_by_surname(Person* head, char* s) { 

	Person* tmp = head;
	Person* prev = NULL;

	while (tmp != NULL) {
		if (strcmp(tmp->surname, s) == 0) {
			
			if (prev == NULL) { //ako brisem prvi element
				head = tmp->next;
				printf("Izbrisali ste prezime: %s\n", tmp->surname);
			}
			else {
				prev->next = tmp->next;
				printf("Izbrisali ste prezime: %s\n", tmp->surname);
			}
			free(tmp);
			return head;
		}
		prev = tmp;
		tmp = tmp->next;
	}

	printf("Osoba %s nije pronađena na listi.\n", s);
	return head;
}
