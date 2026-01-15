#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 64
#define MAXFILE 128
#define TABLE_SIZE 11

typedef struct CityNode {
    char name[MAX];
    int pop;
    struct CityNode* left;
    struct CityNode* right;
} CityNode;

typedef struct CountryNode {
    char name[MAX];
    char filename[MAXFILE];
    CityNode* cities;            
    struct CountryNode* next;      
} CountryNode;

typedef struct HashTable {
    int size; 
    CountryNode* buckets[TABLE_SIZE];
} HashTable;


int hashKey(char* countryName, int size);
void initHash(HashTable* ht);
int compareCity(int pop1, char* name1, int pop2, char* name2);
CityNode* createCity(char* name, int pop);
CityNode* insertCity(CityNode* root, char* name, int pop);
void printCityBST(CityNode* root);
void printCityAbove(CityNode* root, int limit);
void freeCityBST(CityNode* root);
CountryNode* createCountry(char* name, char* filename);
void insertCountrySorted(CountryNode** head, CountryNode* newNode);
CountryNode* findCountry(HashTable* ht, char* countryName);
void freeCountryList(CountryNode* head);
void loadCities(CityNode** root, char* filename);
void loadCountries(HashTable* ht, char* drzaveFile);
void printAll(HashTable* ht);
void menu(HashTable* ht);

int main() {
    HashTable ht;
    int i;

    initHash(&ht);
    loadCountries(&ht, "drzave.txt");
    menu(&ht);

  
    for (i = 0; i < ht.size; i++) {
        freeCountryList(ht.buckets[i]);
    }

    return 0;
}
int hashKey(char* countryName, int size) {
    int sum = 0;
    for (int i = 0; i < 5 && countryName[i] != '\0'; i++) {
        sum += countryName[i];
    }
    return sum % size;
}


void initHash(HashTable* ht) {
    int i;
    ht->size = TABLE_SIZE;

    for (i = 0; i < ht->size; i++) {
        ht->buckets[i] = NULL;
    }
}


int compareCity(int pop1, char* name1, int pop2, char* name2) {
    if (pop1 < pop2) return -1;
    if (pop1 > pop2) return 1;
    return strcmp(name1, name2);
}



CityNode* createCity(char* name, int pop) {
    CityNode* n = (CityNode*)malloc(sizeof(CityNode));
    if (!n) return NULL;

    strcpy(n->name, name);
    n->pop = pop;
    n->left = NULL;
    n->right = NULL;

    return n;
}


CityNode* insertCity(CityNode* root, char* name, int pop) {
    if (root == NULL) {
        return createCity(name, pop);
    }

    if (compareCity(pop, name, root->pop, root->name) < 0) {
        root->left = insertCity(root->left, name, pop);
    }
    else if (compareCity(pop, name, root->pop, root->name) > 0) {
        root->right = insertCity(root->right, name, pop);
    }
    

    return root;
}

void printCityBST(CityNode* root) {
    if (root == NULL) return;

    printCityBST(root->left);
    printf("%s %d\n", root->name, root->pop);
    printCityBST(root->right);
}

void printCityAbove(CityNode* root, int limit) {
    if (root == NULL) return;

    if (root->pop <= limit) {
       
        printCityAbove(root->right, limit);
    }
    else {
       
        printCityAbove(root->left, limit);
        printf("%s %d\n\n", root->name, root->pop);
        printCityAbove(root->right, limit);
    }
}

void freeCityBST(CityNode* root) {
    if (root == NULL) return;

    freeCityBST(root->left);
    freeCityBST(root->right);
    free(root);
}

CountryNode* createCountry(char* name, char* filename) {
    CountryNode* c = (CountryNode*)malloc(sizeof(CountryNode));
    if (!c) return NULL;

    strcpy(c->name, name);
    strcpy(c->filename, filename);
    c->cities = NULL;
    c->next = NULL;

    return c;
}

void insertCountrySorted(CountryNode** head, CountryNode* newNode) {
    CountryNode* cur;

    if (*head == NULL || strcmp(newNode->name, (*head)->name) < 0) {
        newNode->next = *head;
        *head = newNode;
        return;
    }

    cur = *head;
    while (cur->next != NULL && strcmp(cur->next->name, newNode->name) < 0) {
        cur = cur->next;
    }

   
    if (cur->next != NULL && strcmp(cur->next->name, newNode->name) == 0) {
        freeCityBST(newNode->cities);
        free(newNode);
        return;
    }

    newNode->next = cur->next;
    cur->next = newNode;
}

CountryNode* findCountry(HashTable* ht, char* countryName) {
    int index = hashKey(countryName, ht->size);
    CountryNode* cur = ht->buckets[index];

    while (cur != NULL) {
        int cmp = strcmp(cur->name, countryName);

        if (cmp == 0) return cur;
        if (cmp > 0) return NULL;

        cur = cur->next;
    }

    return NULL;
}

void freeCountryList(CountryNode* head) {
    CountryNode* next;

    while (head != NULL) {
        next = head->next;
        freeCityBST(head->cities);
        free(head);
        head = next;
    }
}
void loadCities(CityNode** root, char* filename) {
    FILE* f = fopen(filename, "r");
    char city[MAX];
    int pop;

    if (!f) {
        printf("Ne mogu otvoriti %s\n", filename);
        return;
    }

    while (fscanf(f, "%s %d", city, &pop) == 2)
        *root = insertCity(*root, city, pop);

    fclose(f);
}
void loadCountries(HashTable* ht, char* drzaveFile) {
    FILE* f = fopen(drzaveFile, "r");
    char cname[MAX];
    char cfile[MAXFILE];

    if (!f) {
        printf("Ne mogu otvoriti %s\n", drzaveFile);
        return;
    }

    while (fscanf(f, "%63s %127s", cname, cfile) == 2) {
        CountryNode* c = createCountry(cname, cfile);
        int idx;

       

        loadCities(&c->cities, cfile);

        idx = hashKey(cname, ht->size);
        insertCountrySorted(&ht->buckets[idx], c);
    }

    fclose(f);
}
void printAll(HashTable* ht) {
    int i;
    printf("\n=== ISPIS HASH TABLICE ===\n");

    for (i = 0; i < ht->size; i++) {
        CountryNode* cur = ht->buckets[i];

        if (cur == NULL) continue;

        printf("\nBucket %d:\n", i);
        while (cur != NULL) {
            printf("Drzava: %s\n", cur->name);
            printCityBST(cur->cities);
            cur = cur->next;
            printf("\n");
        }
    }
}

void menu(HashTable* ht) {
    int choice;
    char country[MAX];
    int limit;
    CountryNode* c;

    do {
        printf("\n--- MENU (Hash) ---\n");
        printf("1 - Ispis svih drzava i gradova\n");
        printf("2 - Pretraga gradova (pop > X) za drzavu\n");
        printf("0 - Izlaz\n");
        printf("Odabir: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printAll(ht);
        }
        else if (choice == 2) {
            printf("Unesi drzavu: ");
            scanf("%63s", country);
            printf("Unesi prag X: ");
            scanf("%d", &limit);

            c = findCountry(ht, country);
            if (c == NULL) {
                printf("Nema drzave '%s'\n", country);
            }
            else {
                printf("\nGradovi u %s s pop > %d:\n", c->name, limit);
                printCityAbove(c->cities, limit);
            }
        }
        else if (choice == 0) {
            printf("Izlaz.\n");
        }
        else {
            printf("Pogresan odabir.\n");
        }

    } while (choice != 0);
}