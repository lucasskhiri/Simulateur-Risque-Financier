#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "finance.h"

void vider_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    srand(time(NULL)); // Initialisation de l'aléatoire
    Pret* portefeuille = NULL;
    int choix, id_compteur = 1;

    printf("==================================================\n");
    printf("  MOTEUR DE SIMULATION FINANCIERE - MIASHS L1\n");
    printf("==================================================\n");

    do {
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("1. Ajouter un pret au portefeuille\n");
        printf("2. Afficher le portefeuille\n");
        printf("3. Lancer la simulation Monte-Carlo sur le dernier pret\n");
        printf("4. Quitter\n");
        printf("Votre choix : ");

        if (scanf("%d", &choix) != 1) {
            vider_buffer();
            choix = 0;
        }

        if (choix == 1) {
            double cap, taux;
            int duree, var;
            printf("\nCapital emprunte (Euros) : "); scanf("%lf", &cap);
            printf("Taux de base annuel (%%) : "); scanf("%lf", &taux);
            printf("Duree totale (en mois) : "); scanf("%d", &duree);
            printf("Taux variable ? (1 pour Oui, 0 pour Non) : "); scanf("%d", &var);

            portefeuille = ajouter_pret(portefeuille, id_compteur++, cap, taux, duree, var);
            printf("[+] Pret ajoute avec succes.\n");
        }
        else if (choix == 2) {
            afficher_portefeuille(portefeuille);
        }
        else if (choix == 3) {
            if (portefeuille == NULL) {
                printf("[!] Aucun pret dans le portefeuille.\n");
            } else {
                // On lance la simulation sur le prêt en tête de liste (le dernier ajouté)
                printf("Lancement de 10 000 simulations...\n");
                simuler_monte_carlo(portefeuille, 10000);
            }
        }

    } while (choix != 4);

    printf("\nLiberation de la memoire et fermeture...\n");
    liberer_portefeuille(portefeuille);
    return 0;
}
