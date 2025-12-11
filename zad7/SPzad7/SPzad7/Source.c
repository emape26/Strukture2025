#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50

typedef struct Dir {
    char name[MAX];
    struct Dir* parent;
    struct Dir* child;
    struct Dir* sibling;
} Dir;

Dir* createDir(const char* name, Dir* parent);
Dir* findChild(Dir* current, const char* name);
void printDir(Dir* current);
void deleteTree(Dir* root);
void printPath(Dir* current);
void Meni(Dir* root);

int main() {

    Dir* root = createDir("C:", NULL);

    Meni(root);

    deleteTree(root);

    return 0;
}

Dir* createDir(const char* name, Dir* parent) {
    Dir* novi = (Dir*)malloc(sizeof(Dir));
    if (!novi) {
        printf("Greska pri alokaciji memorije!\n");
        exit(1);
    }
    strcpy(novi->name, name);
    novi->parent = parent;
    novi->child = NULL;
    novi->sibling = NULL;

    if (parent != NULL) {

        novi->sibling = parent->child;
        parent->child = novi;
    }

    return novi;
}


Dir* findChild(Dir* current, const char* name) {
    Dir* p = current->child;
    while (p != NULL) {
        if (strcmp(p->name, name) == 0)
            return p;
        p = p->sibling;
    }
    return NULL;
}


void printDir(Dir* current) {
    Dir* p = current->child;
    if (p == NULL) {
        printf("Direktorij je prazan.\n");
        return;
    }

    printf("Sadrzaj direktorija %s:\n", current->name);
    while (p != NULL) {
        printf("  %s\n", p->name);
        p = p->sibling;
    }
}


void deleteTree(Dir* root) {
    if (root == NULL) return;

    Dir* child = root->child;
    while (child != NULL) {
        Dir* next = child->sibling;
        deleteTree(child);
        child = next;
    }
    free(root);

}


void printPath(Dir* current) {
    if (current->parent != NULL) {
        printPath(current->parent);
        printf("\\%s", current->name);
    }
    else {
        printf("%s", current->name);
    }
}


void Meni(Dir* root) {

    int izbor;
    char ime[MAX];
    Dir* current = root;
    do {
        printf("\n-------------------------------\n");
        printf("Trenutni direktorij: ");
        printPath(current);
        printf("\n");
        printf("1 - md      (napravi direktorij)\n");
        printf("2 - cd dir  (udi u pod-direktorij)\n");
        printf("3 - cd   (povratak u prethodni)\n");
        printf("4 - dir     (ispis sadrzaja)\n");
        printf("5 - izlaz\n");
        printf("Odabir: ");

        if (scanf("%d", &izbor) != 1) {
            printf("Pogresan unos!\n");
            return;
        }

        switch (izbor) {
        case 1: // md
            printf("Unesi ime novog direktorija: ");
            scanf("%s", ime);

            if (findChild(current, ime) != NULL) {
                printf("Direktorij s tim imenom vec postoji!\n");
            }
            else {
                createDir(ime, current);
                printf("Direktorij '%s' je kreiran.\n", ime);
            }
            break;

        case 2: // cd dir
        {
            printf("Unesi ime direktorija u koji zelis uci: ");
            scanf("%s", ime);

            Dir* p = findChild(current, ime);
            if (p == NULL) {
                printf("Direktorij ne postoji.\n");
            }
            else {
                current = p;
            }
        }
        break;

        case 3: // cd
            if (current->parent == NULL) {
                printf("Vec si u korijenskom direktoriju.\n");
            }
            else {
                current = current->parent;
            }
            break;

        case 4: // dir
            printDir(current);
            break;

        case 5:
            printf("Izlaz iz programa\n");
            break;

        default:
            printf("Nepoznata opcija!\n");
        }

    } while (izbor != 5);
}