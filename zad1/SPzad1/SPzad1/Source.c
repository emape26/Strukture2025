#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#define MAX_POINTS 60


struct _students {
	char name[50];
	char surname[50];
	float points;

}typedef Students;


int countStudents();
Students* loadData(int);

int main() {

	int numStudents;
	numStudents = countStudents();
	//printf("Broj redaka je %d", numStudents);

	Students* s = loadData(numStudents);

	printf("Podaci o studentima: \n\n");
	for (int i = 0;i < numStudents;i++) {
		printf("Ime i prezime: %s %s\nBroj bodova: %.2f/%d \nPostotak: %.2f %%\n", s[i].name, s[i].surname, s[i].points, MAX_POINTS ,s[i].points / MAX_POINTS * 100);
		printf("\n");
	}
	return 0;
}

int countStudents(){

	int counter = 0;   //Broj redova u file-u tj. broj studenata.
	char buffer[256];

	FILE* f;
	f = fopen("Studenti.txt", "r");
	if (f == NULL) {
		printf("Greška pri otvaranju datoteke!");
		return 0;
	}

	while (fgets(buffer, sizeof(buffer), f) != NULL) {  //Kada dode do /n brojac se povecava.
		counter++;                                       
	}
	
	fclose(f);

	return counter;
}

Students* loadData(int numStudents) {    //Funkcija mi vraca pokazivac na strukturu pa mora biti tipa struct.

	FILE* f;
	f = fopen("Studenti.txt", "r");
	if (f == NULL) {
		printf("Greška pri otvaranju datoteke!");
		return 0;
	}
	 
	Students* s = (Students*)malloc(numStudents * sizeof(Students));

	for (int i = 0; i < numStudents;i++) {
		fscanf(f, "%s %s %f", s[i].name, s[i].surname, &s[i].points);
	}
	
	fclose(f);

	return s;
}