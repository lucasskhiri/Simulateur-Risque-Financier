#ifndef FINANCE_H
#define FINANCE_H

// Structure pour un élément de la liste chaînée (le portefeuille de prêts)
typedef struct Pret {
    int id;
    double capital;
    double taux_base;
    int duree_mois;
    int est_taux_variable;
    struct Pret* suivant;
} Pret;

// Prototypes des fonctions
Pret* ajouter_pret(Pret* tete, int id, double cap, double taux, int duree, int var);
void afficher_portefeuille(Pret* tete);
void liberer_portefeuille(Pret* tete);
double generer_variation_taux();
void simuler_monte_carlo(Pret* p, int nb_simulations);

#endif
