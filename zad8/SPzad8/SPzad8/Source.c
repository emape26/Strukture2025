#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct cvorStabla;

typedef struct cvorStabla Cvor;
typedef struct cvorStabla* Stablo;
typedef struct cvorStabla* Pozicija;

struct cvorStabla {
    int El;
    Stablo L;
    Stablo D;
};

void Meni(Stablo* S);
void ObrisiSve(Stablo S);
void IspisLevelOrder(Stablo S);
void IspisRazina(Stablo S, int level);
int Visina(Stablo S);
void IspisPostorder(Stablo S);
void IspisPreorder(Stablo S);
void IspisInorder(Stablo S);
Stablo Brisi(int x, Stablo S);
Stablo Dodaj(int x, Stablo S);
Stablo NoviCvor(int x);
Pozicija Trazi(int x, Stablo S);
Pozicija TraziMin(Stablo S);
Pozicija TraziMax(Stablo S);
Stablo StvoriPrazno(Stablo S);


int main() {


    Stablo S = StvoriPrazno(NULL);
    Meni(&S);
    ObrisiSve(S);
   
    return 0;
}


Stablo StvoriPrazno(Stablo S) {
    (void)S;     
    return NULL;
}

Pozicija TraziMin(Stablo S) {
    if (S == NULL) return NULL;

    while (S->L != NULL)
        S = S->L;

    return S;
}

Pozicija TraziMax(Stablo S) {
    if (S == NULL) return NULL;

    while (S->D != NULL)
        S = S->D;

    return S;
}

Pozicija Trazi(int x, Stablo S) {
    while (S != NULL) {
        if (x == S->El) return S;
        if (x < S->El) S = S->L;
        else S = S->D;
    }
    return NULL;
}

Stablo NoviCvor(int x) {
    Stablo n = (Stablo)malloc(sizeof(Cvor));
    if (n == NULL) {
        printf("Greska u alociranju!");
   }
    n->El = x;
    n->L = n->D = NULL;
    return n;
}

Stablo Dodaj(int x, Stablo S) {
    if (S == NULL) {
        return NoviCvor(x);         
    }

    if (x < S->El) {
        S->L = Dodaj(x, S->L);   
    }
    else if (x > S->El) {
        S->D = Dodaj(x, S->D);      
    }
    

    return S;                        
}
Stablo Brisi(int x, Stablo S) {
    if (S == NULL) return NULL;             

    if (x < S->El) {
        S->L = Brisi(x, S->L);              
    }
    else if (x > S->El) {
        S->D = Brisi(x, S->D);               
    }
    else {
        

        if (S->L == NULL) {
            Stablo desno = S->D;
            free(S);
            return desno;
        }

        
        if (S->D == NULL) {
            Stablo lijevo = S->L;
            free(S);
            return lijevo;
        }

        
        Pozicija minD = TraziMin(S->D);
        S->El = minD->El;
        S->D = Brisi(minD->El, S->D);
    }

    return S;                                
}

void IspisInorder(Stablo S) {
    if (S == NULL) {
        return;
    }
    IspisInorder(S->L);        
    printf("%d ", S->El);      
    IspisInorder(S->D);       
}

void IspisPreorder(Stablo S) {
    if (S == NULL) {
        return;
    }

    printf("%d ", S->El);      
    IspisPreorder(S->L);       
    IspisPreorder(S->D);       
}

void IspisPostorder(Stablo S) {
    if (S == NULL) {
        return;
    }

    IspisPostorder(S->L);     
    IspisPostorder(S->D);      
    printf("%d ", S->El);      
}

int Visina(Stablo S) {
    if (S == NULL) {
        return 0;
    }

    if (Visina(S->L) > Visina(S->D)) {
        return Visina(S->L) + 1;
    }
    else {
        return Visina(S->D) + 1;
    }
}

void IspisRazina(Stablo S, int level) {
    if (S == NULL) {
        return;
    }

    if (level == 1) {
        printf("%d ", S->El);        
    }

    IspisRazina(S->L, level - 1);
    IspisRazina(S->D, level - 1);
}

void IspisLevelOrder(Stablo S) {
    int h = Visina(S);
    int i;
    for (i = 1; i <= h; i++) {
        IspisRazina(S, i);
    }
}

void ObrisiSve(Stablo S) {
    if (!S) return;
    ObrisiSve(S->L);
    ObrisiSve(S->D);
    free(S);
}

void Meni(Stablo* S) {
    int izbor, x;

    do {
        printf("\n---Izbornik ---\n");
        printf("1) Unos novog elementa\n");
        printf("2) Ispis inorder\n");
        printf("3) Ispis preorder\n");
        printf("4) Ispis postorder\n");
        printf("5) Ispis level order\n");
        printf("6) Pronadi element\n");
        printf("7) Brisi element\n");
        printf("0) Izlaz\n");
        printf("Odabir: ");

        if (scanf("%d", &izbor) != 1) return;

        switch (izbor) {
        case 1:
            printf("Unesi broj: ");
            if (scanf("%d", &x) == 1) *S = Dodaj(x, *S);
            break;

        case 2:
            printf("Inorder: ");
            IspisInorder(*S);
            printf("\n");
            break;

        case 3:
            printf("Preorder: ");
            IspisPreorder(*S);
            printf("\n");
            break;

        case 4:
            printf("Postorder: ");
            IspisPostorder(*S);
            printf("\n");
            break;

        case 5:
            printf("Level order: ");
            IspisLevelOrder(*S);
            printf("\n");
            break;

        case 6:
            printf("Trazi broj: ");
            if (scanf("%d", &x) == 1) {
                Pozicija p = Trazi(x, *S);
                printf(p ? "Pronaden.\n" : "Nije pronaden.\n");
            }
            break;

        case 7:
            printf("Brisi broj: ");
            if (scanf("%d", &x) == 1) *S = Brisi(x, *S);
            break;

        case 0:
            break;

        default:
            printf("Nepoznat odabir.\n");
        }

    } while (izbor != 0);
}