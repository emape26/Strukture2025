#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX 50


typedef struct article {

	char name[MAX];
	int amount;
	double price;
	struct article* next;
}Article;

typedef struct bill {
	int year, month, day;
	Article* articles;
	struct bill* next;

}Bill;

//funkcije
void printBillsByTotalRange(Bill* headBill);
void historyForArticle(Bill* headBill);
void mostExpensiveArticle(Bill* headBill);
void printSummary(Bill* headBill);
Bill* findMostExpensiveBill(Bill* headBill);
void totalPerDay(Bill* headBill);
void printAllBills(Bill* head);
void PriceOnArticlesYMD(Bill* headBill);
void printBillsInRange(Bill* headBill);


void runMenu(Bill* headBill);
double billTotal(Bill* b);
void printOneBill(Bill* b);
int compareYMD(int y1, int m1, int d1, int y2, int m2, int d2);
void loadAllBill(Bill** headBill, const char* listFilename);
int compareDates(Bill* a, Bill* b);
Bill* loadOneBill(const char* filename);
void addArticleSorted(Article** headAr, Article* newAr);
Bill* createBill(int year, int month, int day);
Article* createArticle(char* name, int amount, double price);


int main() {

	Bill* headBill = NULL;

	loadAllBill(&headBill, "Racuni.txt");

	runMenu(headBill);

	return 0;
}



Article* createArticle(char* name,int amount,double price) {

	Article* newAr = (Article*)malloc(sizeof(Article));
	if (newAr == NULL) {
		printf("Greska u alokaciji memorije!");
		return NULL;
	}

	strcpy(newAr->name, name);  
	newAr->amount = amount;
	newAr->price = price;
	newAr->next = NULL;

	return newAr;
}

Bill* createBill(int year,int month, int day) {

	Bill* newBill = (Bill*)malloc(sizeof(Bill));
	if (newBill == NULL) {
		printf("Greska u alokaciji memorije!");
		return NULL;
	}
	newBill->year = year;
	newBill->month = month;
	newBill->day = day;

	newBill->articles = NULL;
	newBill->next = NULL;

	return newBill;
}

void addArticleSorted(Article **headAr,Article *newAr) {

	if (*headAr == NULL || strcmp(newAr->name, (*headAr)->name) < 0) {
		newAr->next = *headAr;
		*headAr = newAr;
		return;
	}

	Article* tmp = *headAr;
	while (tmp->next != NULL && strcmp(tmp->next->name, newAr->name) <= 0) {
		tmp = tmp->next;
	}
	newAr->next = tmp->next;
	tmp->next = newAr;
}

Bill* loadOneBill(const char* filename) {

	FILE* f = fopen(filename, "r");
	if (f == NULL) {
		printf("Greska u otvaranju file-a");
		return NULL;
	}

	int year, month, day;

	fscanf(f, "%d-%d-%d", &year, &month, &day); 

	Bill* nowBill = createBill(year, month, day);



	Article* headArticle = NULL;
	char name[MAX];
	int amount;
	double price;

	while ( fscanf(f, "%s %d %lf", name, &amount, &price)==3 ) {

		Article* nowAr = createArticle(name, amount, price);
		addArticleSorted(&headArticle, nowAr);

	}
	nowBill->articles = headArticle;
	fclose(f);

	return nowBill;
}
int compareDates(Bill*a,Bill*b) {
	
	if (a->year > b->year) return 1;
	if (a->year < b->year) return -1;

	if (a->month > b->month) return 1;
	if (a->month < b->month) return -1;

	if (a->day > b->day) return 1;
	if (a->day < b->day) return -1;

	return 0;
}
void AddSortBill(Bill**headBill,Bill* newBill) {

	if (*headBill == NULL) {  //prazna lista
		*headBill = newBill;
		return;
	}

	if (compareDates(newBill, *headBill) > 0) {  //veci ide ispred
		newBill->next = *headBill;
		*headBill = newBill;
		return;
	}

	Bill* tmp = *headBill;  // dodajem izad
	while (tmp->next != NULL && compareDates(tmp->next, newBill) >= 0) {
		tmp = tmp->next;
	}

	newBill->next = tmp->next;
	tmp->next = newBill;

}
void loadAllBill(Bill** headBill, const char* listFilename)
{
	FILE* f = fopen(listFilename, "r");
	if (!f) {
		printf("Greska pri otvaranju datoteke");
		return;
	}

	char filename[256];

	
	while (fscanf(f, "%255s", filename) == 1) {

		Bill* newBill = loadOneBill(filename);
		if (newBill != NULL) {
			AddSortBill(headBill, newBill);
		}
	}

	fclose(f);
}

//funkcije s listom

int compareYMD(int y1, int m1, int d1, int y2, int m2, int d2) {
	if (y1 > y2) return 1;
	if (y1 < y2) return -1;

	if (m1 > m2) return 1;
	if (m1 < m2) return -1;

	if (d1 > d2) return 1;
	if (d1 < d2) return -1;

	return 0;
}

void PriceOnArticlesYMD(Bill* headBill) {

	int fromY, fromM, fromD;  //biranje datuma od - do 
	int toY, toM, toD;
	char wantedName[MAX];

	printf("Unesi pocetni datum (YYYY-MM-DD): ");
	scanf("%d-%d-%d", &fromY, &fromM, &fromD);

	printf("Unesi zavrsni datum (YYYY-MM-DD): ");
	scanf("%d-%d-%d", &toY, &toM, &toD);

	printf("Unesi naziv artikla: ");
	scanf("%s", wantedName);      


	int totalAmount = 0;
	double totalMoney = 0.0;

	Bill* tmpB = headBill;

	while (tmpB != NULL) {

		
		if (compareYMD(tmpB->year, tmpB->month, tmpB->day,fromY, fromM, fromD) >= 0 &&   
			compareYMD(tmpB->year, tmpB->month, tmpB->day,toY, toM, toD) <= 0) {      

		
			Article* a = tmpB->articles;
			while (a != NULL) {
				if (strcmp(a->name, wantedName) == 0) {
					totalAmount += a->amount;
					totalMoney += a->amount * a->price;
				}
				a = a->next;
			}
		}

		tmpB = tmpB->next;
	}

	if (totalAmount == 0) {
		printf("Artikl '%s' nije kupljen u zadanom razdoblju.\n", wantedName);
	}
	else {
		printf("Artikl '%s' u razdoblju %04d-%02d-%02d do %04d-%02d-%02d:\n",
			wantedName, fromY, fromM, fromD, toY, toM, toD);
		printf("  Ukupna kolicina: %d\n", totalAmount);
		printf("  Ukupno potroseno: %.2lf\n", totalMoney);
	}
}
void printOneBill(Bill* b) {
	if (b == NULL) return;

	printf("Racun: %04d-%02d-%02d\n", b->year, b->month, b->day);
	Article* a = b->articles;
	while (a != NULL) {
		printf("  %-10s x%d  %.2lf\n", a->name, a->amount, a->price);
		a = a->next;
	}
}
void printAllBills(Bill* head) {
	Bill* tmp = head;
	while (tmp != NULL) {
		printOneBill(tmp);
		printf("\n");
		tmp = tmp->next;
	}
}
void printBillsInRange(Bill* headBill) {

	int fromY, fromM, fromD;
	int toY, toM, toD;

	printf("Unesi pocetni datum (YYYY-MM-DD): ");
	scanf("%d-%d-%d", &fromY, &fromM, &fromD);

	printf("Unesi zavrsni datum (YYYY-MM-DD): ");
	scanf("%d-%d-%d", &toY, &toM, &toD);


	if (compareYMD(fromY, fromM, fromD, toY, toM, toD) > 0) {
		int ty = fromY, tm = fromM, td = fromD;
		fromY = toY; fromM = toM; fromD = toD;
		toY = ty;  toM = tm;  toD = td;
	}

	Bill* tmp = headBill;
	int found = 0;

	while (tmp != NULL) {

		
		if (compareYMD(tmp->year, tmp->month, tmp->day,
			fromY, fromM, fromD) >= 0 &&
			compareYMD(tmp->year, tmp->month, tmp->day,
				toY, toM, toD) <= 0) {

			printOneBill(tmp);
			printf("\n");
			found = 1;
		}

		tmp = tmp->next;
	}

	if (!found) {
		printf("Nema racuna u razdoblju %04d-%02d-%02d do %04d-%02d-%02d.\n",
			fromY, fromM, fromD, toY, toM, toD);
	}
}
void totalPerDay(Bill* headBill) {

	int fromY, fromM, fromD;
	int toY, toM, toD;

	printf("Unesi pocetni datum (YYYY-MM-DD): ");
	scanf("%d-%d-%d", &fromY, &fromM, &fromD);

	printf("Unesi zavrsni datum (YYYY-MM-DD): ");
	scanf("%d-%d-%d", &toY, &toM, &toD);

	if (compareYMD(fromY, fromM, fromD, toY, toM, toD) > 0) {
		int ty = fromY, tm = fromM, td = fromD;
		fromY = toY; fromM = toM; fromD = toD;
		toY = ty;  toM = tm;  toD = td;
	}

	Bill* b = headBill;

	int currY = 0, currM = 0, currD = 0;
	double currSum = 0.0;
	int anyPrinted = 0;

	while (b != NULL) {

		
		if (compareYMD(b->year, b->month, b->day,
			fromY, fromM, fromD) >= 0 &&
			compareYMD(b->year, b->month, b->day,
				toY, toM, toD) <= 0) {

			
			if (currSum == 0.0 ||
				b->year != currY || b->month != currM || b->day != currD) {

				
				if (currSum > 0.0) {
					printf("%04d-%02d-%02d : %.2lf\n", currY, currM, currD, currSum);
					anyPrinted = 1;
				}

				currY = b->year;
				currM = b->month;
				currD = b->day;
				currSum = 0.0;
			}

			Article* a = b->articles;
			while (a != NULL) {
				currSum += a->amount * a->price;
				a = a->next;
			}
		}

		b = b->next;
	}
	if (currSum > 0.0) {
		printf("%04d-%02d-%02d : %.2lf\n", currY, currM, currD, currSum);
		anyPrinted = 1;
	}
	if (!anyPrinted) {
		printf("Nema racuna u razdoblju %04d-%02d-%02d do %04d-%02d-%02d.\n",
			fromY, fromM, fromD, toY, toM, toD);
	}
}
double billTotal(Bill* b) {
	
	if (b == NULL) return 0.0;

	double sum = 0.0;
	Article* a = b->articles;

	while (a != NULL) {
		sum += a->amount * a->price;
		a = a->next;
	}

	return sum;

}
Bill* findMostExpensiveBill(Bill* headBill) {
	
	if (headBill == NULL) return NULL;

	Bill* maxBill = headBill;
	double maxSum = billTotal(headBill);
	Bill* b = headBill->next;

	while (b != NULL) {
		double curr = billTotal(b);
		if (curr > maxSum) {
			maxSum = curr;
			maxBill = b;
		}
		b = b->next;
	}
	return maxBill;
}

void printSummary(Bill* headBill) {

	int billCount = 0;
	int articleCount = 0;
	double totalMoney = 0.0;

	Bill* b = headBill;

	while (b != NULL) {

		billCount++;

		
		Article* a = b->articles;
		while (a != NULL) {
			articleCount++;
			totalMoney += a->amount * a->price;
			a = a->next;
		}

		b = b->next;
	}

	printf("\n--- Statistika svih racuna ---\n");
	printf("Ukupan broj racuna: %d\n", billCount);
	printf("Ukupan broj artikala: %d\n", articleCount);
	printf("Ukupno potroseno (svi racuni): %.2lf\n", totalMoney);
}
void mostExpensiveArticle(Bill* headBill) {

	Bill* b = headBill;
	Article* maxArt = NULL;
	double maxPrice = 0.0;

	while (b != NULL) {
		Article* a = b->articles;
		while (a != NULL) {

			if (maxArt == NULL || a->price > maxPrice) {
				maxArt = a;
				maxPrice = a->price;
			}

			a = a->next;
		}
		b = b->next;
	}

	if (maxArt == NULL) {
		printf("Nema artikala (nema ucitanih racuna).\n");
	}
	else {
		printf("Najskuplji artikl (cijena jednog proizvoda):\n");
		printf("  Naziv: %s\n", maxArt->name);
		printf("  Cijena: %.2lf\n", maxPrice);
	}
}
void historyForArticle(Bill* headBill) {

	char wantedName[MAX];
	printf("Unesi naziv artikla: ");
	scanf("%s", wantedName);

	Bill* b = headBill;
	int found = 0;
	int totalAmount = 0;
	double totalMoney = 0.0;

	printf("\nPovijest artikla '%s':\n", wantedName);

	while (b != NULL) {
		Article* a = b->articles;
		while (a != NULL) {

			if (strcmp(a->name, wantedName) == 0) {
				double lineTotal = a->amount * a->price;
				printf("  %04d-%02d-%02d : %d x %.2lf = %.2lf\n",
					b->year, b->month, b->day,
					a->amount, a->price, lineTotal);

				totalAmount += a->amount;
				totalMoney += lineTotal;
				found = 1;
			}

			a = a->next;
		}
		b = b->next;
	}

	if (!found) {
		printf("Artikl '%s' se ne pojavljuje ni na jednom racunu.\n", wantedName);
	}
	else {
		printf("\nUkupno:\n");
		printf("  Kolicina: %d kom\n", totalAmount);
		printf("  Potroseno: %.2lf\n", totalMoney);
	}
}
void printBillsByTotalRange(Bill* headBill) {

	double min, max;

	printf("Unesi minimalni iznos racuna: ");
	scanf("%lf", &min);

	printf("Unesi maksimalni iznos racuna: ");
	scanf("%lf", &max);

	// ako je korisnik obrnuo (min > max) - zamijenimo
	if (min > max) {
		double t = min;
		min = max;
		max = t;
	}

	Bill* b = headBill;
	int found = 0;

	while (b != NULL) {

		double total = billTotal(b);

		if (total >= min && total <= max) {
			printf("\nRacun (ukupno: %.2lf)\n", total);
			printOneBill(b);
			found = 1;
		}

		b = b->next;
	}

	if (!found) {
		printf("Nema racuna s ukupnim iznosom u rasponu [%.2lf, %.2lf].\n", min, max);
	}
}

void runMenu(Bill* headBill) {

	int choice;

	do {
		printf("\n--- IZBORNIK ---\n");
		printf("1 - Ispis svih racuna\n");
		printf("2 - Ispis racuna u vremenskom razdoblju\n");
		printf("3 - Upit za artikl u vremenskom razdoblju\n");
		printf("4 - Ispis troska u danu u odredenom vremenskom razdoblju\n");
		printf("5 - Najskuplji racun\n");
		printf("6 - Najskuplji artikl\n");
		printf("7 - Ukupni pregled podataka\n");
		printf("8 - Povijest jednog artikla (po datumima)\n");
		printf("9 - Ispis racuna prema rasponu ukupne cijene\n");
		printf("0 - Izlaz\n");
		printf("Odaberi opciju: ");
		scanf("%d", &choice);

		switch (choice) {
		case 1:
			printAllBills(headBill);
			break;

		case 2:
			printBillsInRange(headBill);
			break;

		case 3:
			PriceOnArticlesYMD(headBill);
			break;

		case 4:
			totalPerDay(headBill);
			break;

		case 5: {
			Bill* maxB = findMostExpensiveBill(headBill);
			if (maxB != NULL) {
				printf("Najskuplji racun:\n");
				printOneBill(maxB);
				printf("Ukupno: %.2lf\n", billTotal(maxB));
			}
			else {
				printf("Nema racuna.\n");
			}
			break;
		}
		
		case 7:
			printSummary(headBill);
			break;

		case 6:
			mostExpensiveArticle(headBill);
			break;

		case 8:
			historyForArticle(headBill);
			break;

		case 9:
			printBillsByTotalRange(headBill);
			break;

		case 0:
			printf("Izlaz iz programa.\n");
			break;

		default:
			printf("Nepostojeca opcija, pokusaj ponovno.\n");
		}

	} while (choice != 0);

}