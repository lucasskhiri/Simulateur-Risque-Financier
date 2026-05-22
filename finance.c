#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "finance.h"

// Ajoute un prêt à la liste chaînée
Pret* ajouter_pret(Pret* tete, int id, double cap, double taux, int duree, int var) {
    Pret* nouveau = (Pret*)malloc(sizeof(Pret));
    if (nouveau == NULL) {
        printf("Erreur d'allocation memoire.\n");
        return tete;
    }
    nouveau->id = id;
    nouveau->capital = cap;
    nouveau->taux_base = taux;
    nouveau->duree_mois = duree;
    nouveau->est_taux_variable = var;
    nouveau->suivant = tete;
    return nouveau;
}

// Affiche tous les prêts enregistrés
void afficher_portefeuille(Pret* tete) {
    Pret* actuel = tete;
    if (actuel == NULL) {
        printf("Le portefeuille est vide.\n");
        return;
    }
    printf("\n--- VOTRE PORTEFEUILLE DE PRETS ---\n");
    while (actuel != NULL) {
        printf("ID: %d | Capital: %.2lf EUR | Taux: %.2lf%% | Duree: %d mois | Type: %s\n",
               actuel->id, actuel->capital, actuel->taux_base, actuel->duree_mois,
               actuel->est_taux_variable ? "Variable" : "Fixe");
        actuel = actuel->suivant;
    }
}

// Libère la mémoire à la fin du programme
void liberer_portefeuille(Pret* tete) {
    Pret* actuel = tete;
    while (actuel != NULL) {
        Pret* temp = actuel;
        actuel = actuel->suivant;
        free(temp);
    }
}

// Génère une fluctuation aléatoire du taux
double generer_variation_taux() {
    double u1 = (double)rand() / RAND_MAX;
    double u2 = (double)rand() / RAND_MAX;
    if (u1 == 0.0) u1 = 0.0001; // eviter le log(0)
    double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    return z0 * 0.05; // Volatilité de 5%
}

// L'algorithme de Monte-Carlo
void simuler_monte_carlo(Pret* p, int nb_simulations) {
    if (p->est_taux_variable == 0) {
        printf("\nLe pret ID %d est a taux fixe. Simulation Monte-Carlo inutile.\n", p->id);
        return;
    }

    double cout_total_moyen = 0.0;
    double pire_cas = 0.0;
    double meilleur_cas = -1.0;

    for (int i = 0; i < nb_simulations; i++) {
        double taux_actuel = p->taux_base;
        double cout_simulation = 0.0;
        double capital_restant = p->capital;

        for (int mois = 0; mois < p->duree_mois; mois++) {
            taux_actuel += generer_variation_taux();
            if (taux_actuel < 0.1) taux_actuel = 0.1;

            double taux_m = taux_actuel / 100.0 / 12.0;
            double interet_mois = capital_restant * taux_m;
            double mensualite = (capital_restant * taux_m) / (1.0 - pow(1.0 + taux_m, -(p->duree_mois - mois)));

            cout_simulation += mensualite;
            capital_restant -= (mensualite - interet_mois);
        }

        cout_total_moyen += cout_simulation;
        if (cout_simulation > pire_cas) pire_cas = cout_simulation;
        if (meilleur_cas < 0 || cout_simulation < meilleur_cas) meilleur_cas = cout_simulation;
    }

    cout_total_moyen /= nb_simulations;

    printf("\n--- RESULTATS MONTE-CARLO PRENT ID %d (%d iterations) ---\n", p->id, nb_simulations);
    printf("Cout total moyen espere   : %.2lf Euros\n", cout_total_moyen);
    printf("Meilleur scenario (baisse): %.2lf Euros\n", meilleur_cas);
    printf("Pire scenario (hausse)    : %.2lf Euros\n", pire_cas);
}
