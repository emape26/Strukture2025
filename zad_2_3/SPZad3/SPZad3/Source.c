#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX 50

struct _person {
	char name[MAX];
	char surname[MAX];
	int birthYear;

	struct _person* next;

}typedef Person;

Person* sortList(Person* head);
Person* ReadListFile(void);
void ListToFile(Person* head);
Person* add_in_Front(Person* head, char* s);
Person* Add_behind(Person* head, char* s);
Person* Delete_by_surname(Person* head, char* s);
void Find_by_surname(Person* head, char* s);
Person* Insert_at_end(Person* head);
void printList(Person* head);
Person* Insert_at_start(Person* head);
void InsertData(Person* new);

int main() {

	Person* head = NULL;      //Krecem s praznom listom
	int num, casenum;
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
				"6.Dodat element izad odabranog prezimena\n"
				"7.Dodat element ispred odabranog prezimena\n"
				"8.Sortirati listu\n"
				"9.Zavrsiti s uredivanjem liste i spremanje u datoteku\n\n"  //Ponavlja se dok korisnik ne unese 8
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
			
			case 6 :
				printf("Izad kojeg prezimena zelis dodati element? ");
				scanf("%s", help);
				head = Add_behind(head, help);
				break;

			case 7:
				printf("Unesi prezime ispred kojeg zelis dodat element: ");
				scanf("%s", help);
				head = add_in_Front(head, help);
				break;

			case 8:
				head = sortList(head);
				printList(head);
				break;

			case 9:
				printf("\nZavrseno s radom.");
				break;

			default:
				printf("\nNepostojeca opcija");
				break;
			}
		} while (casenum != 9);
		
		ListToFile(head);

		//head = ReadListFile();
		//printList(head);
		
		printf("Zelite li ucitati listu iz datoteke?( Upisat broj uz zeljenu akciju)\n"
			   "1. Da\n"
			   "2. Ne\n"
		);
		scanf("%d", &casenum);

		switch (casenum) {

			case 1:
				head = ReadListFile();
				printList(head);
				break;
			case 2:
				printf("Gotovo s radom!");
				break;
		}

	
		return 0;
}

Person* Insert_at_start(Person* head) {

	Person* new = (Person*)malloc(sizeof(Person));
	if (new == NULL) {
		printf("Greska u alokaciji memorije!\n");
		return head;
	}

	InsertData(new);
	new->next = head;    //Novi element pokazuje na head koji pokazuje na ostatak liste

	return new;
}

void InsertData(Person* new) {

	printf("Unesi ime, prezime i godinu rodenja: ");
	scanf("%s %s %d", new->name, new->surname, &new->birthYear);
}

void printList(Person* head) {

	Person* tmp = head;
	int i = 1;
	while (tmp != NULL) {
		printf("%d. %s %s %d\n", i, tmp->name, tmp->surname, tmp->birthYear);
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
	int position = 1;
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
		printf("Osoba %s nije pronadena na listi.\n", s);
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

Person* Add_behind(Person* head, char* s) { //Dodaje element izad odabarnog

	Person* new = (Person*)malloc(sizeof(Person));
	if (new == NULL) {
		printf("Greska u alokaciji memorije!\n");
		return head;
	}
	Person* tmp = head;
	InsertData(new);

	while (tmp != NULL) {

		if (strcmp(tmp->surname, s) == 0) {
			new->next = tmp->next;  //prvo moram zapantit stari next pa onda novi
			tmp->next = new;

			return head;
		}
		tmp = tmp->next;
	}

	printf("Nije pronadeno prezime!");
	return head;
}

Person* add_in_Front(Person* head,char *s) {

	Person* new = (Person*)malloc(sizeof(Person));
	if (new == NULL) {
		printf("Greska u alokaciji memorije!");
		return head;
	}
	InsertData(new);

	Person* tmp = head;
	Person* prev = NULL;

	while (tmp != NULL) {
		if (strcmp(tmp->surname, s) == 0) {

			if (prev == NULL) {           //ako je prezime na pocetku onda new postaje prvi element
				head = new;
				new->next = tmp;
				return head;
			}
			else {                         
				new->next = tmp;
				prev->next = new;
				return head;
			}
		}
		prev = tmp;
		tmp = tmp->next;
	}

	printf("Nije pronadeno prezime!");
	return head;
} 
	
void ListToFile(Person* head) {

	FILE* f = fopen("GotovaLista.txt", "w");
	if (f == NULL) {
		printf("Greska pri otvaranju datoteke!");
		return;
	}

	Person* tmp = head;
	while (tmp != NULL) {
		fprintf(f, "%s %s %d\n", tmp->name, tmp->surname, tmp->birthYear);
		tmp = tmp->next;
	}
	fclose(f);
	printf("Lista je spremljena u datoteku!\n");

}
Person* ReadListFile(void) {

	FILE* f = fopen("GotovaLista.txt", "r");
	if (f == NULL) {
		printf("Greska pri otvaranju datoteke!");
		return;
	}

	Person* head = NULL;
	Person* prev = NULL;

	while (1) {  //citanje sve dok ima podataka u datoteci

		Person* new = (Person*)malloc(sizeof(Person));
		if (new == NULL) {
			printf("Greska u alokaciji memorije!");
			fclose(f);
			return head;
		}

		if (fscanf(f, "%s %s %d", new->name, new->surname, &new->birthYear)==3) {
			new->next = NULL;

			if (head == NULL) {  //ako je lista prazna onda novi element postaje head
				head = new;
				prev = new;
			}
			else { 
				prev->next = new;       //ako imamo elemente new ide izad starog
				prev = new;
			}
		}
		else {                            //ako fscanf nije procitao 3 vrijednosti izlazi iz petlje
			free(new);
			break;
		}
	}
	fclose(f);
	printf("Lista je ucitana!\n");
	return head;
}

Person * sortList(Person* head) {

	if (head == NULL || head->next == NULL ) {  //Provjera liste
		printf("Lista je prekratka za sortiranje.\n");
		return head;
	}


	Person* prev, * now, * next,*end = NULL;
	

	while (head->next != end) {  //Nakon kraja petlje najveci element dode na kraj
		prev = NULL;
		now = head;              //Krecem od prvog elementa
		next = now->next;

		while (next != end) {
			if (strcmp(now->surname, next->surname) > 0) {
				
				if (prev != NULL)  
					prev->next = next;

				else 
					head = next;

				now->next = next->next;
				next->next = now;
		
				prev = next;
				next = now->next;
			}
			else {                 //ako netriba zamjena onda se pomaknem za jedan u nizu sa prev,now i next
				prev = now;
				now = next;
				next = next->next;
			}
		}
		end = now;                //now mi ostane najveci element i onda je end jednak njemu kako nebi vise mijenjali zadnji element
	}
	printf("Lista je sortirana po prezimenima!\n");
	return head;
}

