/*  
  ==========================================================
  ||  Jeu du Nim - SAE 1.02 - Ahmet BASBUNAR - 2024-2025  ||
  ==========================================================

  // Voici un exemple de code en C pour jouer au jeu du Nim. 
  \\ Liste des fonctions disponibles pour le jeu du Nim :  
  
\\      < Lire_Entier >          - Permet de lire un entier et de le valider entre deux bornes.
 \\     < Parametres >           - Permet de lire les paramètres du jeu.
  \\    < Voisines >             - Construit la liste des cases voisines d'une case donnée.
   \\   < Hasard >               - Génère un déplacement au hasard.
    ||  < Nimber >               - Calcule le nimber d'une case donnée.
   //   < Coup_Joueur >          - Permet au joueur humain de choisir une destination parmi les voisines possibles.
  //    < Coup_Ordi_Hasard >     - Permet à l'ordinateur de choisir un coup au hasard.
 //     < Coup_Ordi_Gagnant >    - Permet à l'ordinateur de jouer un coup gagnant.
//      < main >                 - Assemble l’ensemble et exécute la partie.
*/


#include <stdio.h>                                   // Inclue les bibliothèques standards
#include <stdlib.h>                                 // 
#include <time.h>                                  // 

#define VMIN 5                                      // Defini le Nombre minimum de lignes/colonnes stocké dans VMIN
#define VMAX 30                                     // Defini le Nombre maximum de lignes/colonnes stocké dans VMAX

/*
  ===========================
   1. Déclarations de types
  ===========================
*/

typedef struct {                       // Structure qui Représente une case de la grille : (lig, col)
    int lig;                          //ligne
    int col;                         //colonne
} T_Case;                           // Stocke les coordonnées ligne/colonne

typedef struct {                       // Structure qui représente un tableau de cases voisines
    T_Case tab[4];                    // Tableau de 4 cases voisines
    int nb;                          // Nombre de voisins
} T_Tab_Case;                       // Tableau de cases voisines



/*
  =====================================
   2. Lecture et vérification d'entier
  =====================================
*/

// Cette fonction permet de lire un entier et de vérifier qu’il est compris entre deux bornes definies (un minimum "VMIN" et un maximum "VMAX").
// On affiche à l’utilisateur le message (par exemple : "nombre de lignes : ") et on lui demande de saisir un entier.

int Lire_Entier(const char* message, int min, int max) {
    int valeur, retour;
    do {
        printf("%s", message);                                              // Affiche le message personnalisé (ex: "nombre de lignes : ")
        retour = scanf("%d", &valeur);                                  
        if (retour != 1) {                               
            while (getchar() != '\n');
            printf("Erreur de saisie. Veuillez entrer un entier.\n");
        } else if (valeur < min || valeur > max) {
            printf("Valeur hors bornes [%d..%d]. Réessayez.\n", min, max);
        }
    } while (retour != 1 || valeur < min || valeur > max);
    return valeur;
}


/*
  ================================
   3. Paramètres de la partie
  ================================
*/

void Parametres(int* nlig, int* ncol, int* niveau, int* next) {             // Saisie des paramètres du jeu
    printf("Paramètres du jeu :\n");                                         // Affiche les paramètres du jeu
    *nlig = Lire_Entier("nombre de lignes : ", VMIN, VMAX);                  // Saisie du nombre de lignes
    *ncol = Lire_Entier("nombre de colonnes : ", VMIN, VMAX);                // Saisie du nombre de colonnes
    *niveau = Lire_Entier("niveau de 1 à 4 : ", 1, 4);                       // Saisie du niveau de difficulté (1..4)
    printf("qui commence ?\n");
    *next = Lire_Entier("l’ordinateur (1) ou le joueur (2) : ", 1, 2);       // Saisie du niveau de difficulté (1..4)
}


/*
  ============================
   4. Calcul des voisins 
  ============================
*/

// Construit la liste des cases voisines du pion : 
// - (lig, col+1), (lig, col+2)
// - (lig+1, col), (lig+2, col)
// si elles sont dans la grille.
// Fonction qui construit la liste des cases voisines d'une case donnée (pion).
// Les voisins potentiels sont :
// - La case à droite (lig, col + 1)
// - La case à deux colonnes à droite (lig, col + 2)
// - La case en dessous (lig + 1, col)
// - La case à deux lignes en dessous (lig + 2, col)
// Seules les cases qui restent dans les limites de la grille (nlig x ncol) sont ajoutées à la liste.

void Voisines(T_Case pion, int nlig, int ncol, T_Tab_Case* vois) {                                 // Construit la liste des voisins du pion
    vois->nb = 0;                                                                                   // Initialisation : Aucun voisin n'a encore été trouvé.

                                                          // Vérification et ajout de la case à droite (lig, col + 1)
    if (pion.col + 1 <= ncol) {                                     // Vérifie si la case est dans la grille
        vois->tab[vois->nb].lig = pion.lig;                         // La ligne reste inchangée
        vois->tab[vois->nb].col = pion.col + 1;                     // Colonne décalée de +1
        vois->nb++;                                                 // Incrémente le compteur de voisins trouvés de +1
    }

                                                        // Vérification et ajout de la case à deux colonnes à droite (lig, col + 2)
    if (pion.col + 2 <= ncol) {                                     // Vérifie si la case est dans la grille
        vois->tab[vois->nb].lig = pion.lig;                         // La ligne reste inchangée
        vois->tab[vois->nb].col = pion.col + 2;                     // Colonne décalée de +2
        vois->nb++;                                                 // Incrémente le compteur de voisins trouvés de +1
    }

                                                        // Vérification et ajout de la case en dessous (lig + 1, col)
    if (pion.lig + 1 <= nlig) {                                     // Vérifie si la case est dans la grille
        vois->tab[vois->nb].lig = pion.lig + 1;                     // Ligne décalée de +1
        vois->tab[vois->nb].col = pion.col;                         // Colonne reste inchangée
        vois->nb++;                                                 // Incrémente le compteur de voisins trouvés de +1
    }

                                                        // Vérification et ajout de la case à deux lignes en dessous (lig + 2, col)
    if (pion.lig + 2 <= nlig) {                                     // Vérifie si la case est dans la grille
        vois->tab[vois->nb].lig = pion.lig + 2;                     // Ligne décalée de +2
        vois->tab[vois->nb].col = pion.col;                         // Colonne reste inchangée
        vois->nb++;                                                 // Incrémente le compteur de voisins trouvés de +1
    }
}

/*
  ===================================
   5. Calcul du nimber d'une case
  ===================================
*/
// On stocke les nimbers dans un tableau global 2D (index 1..nlig / 1..ncol)
// nimber[i][j] = 0 ou 1
static int nimber[VMAX+1][VMAX+1];

// Calcule l'intégralité des nimbers par la méthode du sujet:
//  - nimber(nlig, ncol) = 0 (puits)
//  - en remontant (col. par col. et lig. par lig.), nimber(c) = 0 si TOUTES ses voisines sont 1, sinon 1
void ConstruireNimbers(int nlig, int ncol) {
    // Initialisation (optionnelle)
    for (int i = 1; i <= nlig; i++) {
        for (int j = 1; j <= ncol; j++) {
            nimber[i][j] = -1; 
        }
    }
    // On remplit de la dernière colonne à la première
    for (int col = ncol; col >= 1; col--) {
        // pour chaque colonne, on part du bas vers le haut
        for (int lig = nlig; lig >= 1; lig--) {
            if (lig == nlig && col == ncol) {
                // puits
                nimber[lig][col] = 0;
            } else {
                // regarder les voisines de (lig, col)
                T_Case cCour = {lig, col};
                T_Tab_Case v;
                Voisines(cCour, nlig, ncol, &v);
                // Si TOUTES les voisines => nimber=1, alors (lig,col) => 0
                int toutesV1 = 1;
                for (int k = 0; k < v.nb; k++) {
                    int nl = v.tab[k].lig;
                    int nc = v.tab[k].col;
                    if (nimber[nl][nc] == 0) {
                        toutesV1 = 0;
                        break;
                    }
                }
                if (toutesV1 == 1) nimber[lig][col] = 0;
                else nimber[lig][col] = 1;
            }
        }
    }
}

// Fonction utilitaire pour lire le nimber d'une case
int NimberCase(int lig, int col) {
    return nimber[lig][col];
}

/*
  ===================================
   6. Affichage de la grille en texte
  ===================================
*/

void Affiche_Grille(T_Case pion, int nlig, int ncol) {
    printf("\n   ");
    for (int j = 1; j <= ncol; j++) {
        printf("%2d ", j);
    }
    printf("\n");
    for (int i = 1; i <= nlig; i++) {
        printf("%2d ", i);
        for (int j = 1; j <= ncol; j++) {
            if (i == pion.lig && j == pion.col) {
                printf(" O ");
            } else {
                printf(" - ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

/*
  ========================================
   7. Coups de l'ordinateur et du joueur
  ========================================
*/

// 7.1: Le joueur choisit un coup parmi les voisins possibles
void Coup_Joueur(T_Case* pion, int nlig, int ncol) {
    T_Tab_Case v;
    Voisines(*pion, nlig, ncol, &v);

    // On affiche la grille pour voir où on en est
    Affiche_Grille(*pion, nlig, ncol);

    // Affiche la liste des coups possibles
    printf("A toi de jouer ! Choisis ta destination :\n");
    for (int i = 0; i < v.nb; i++) {
        printf("%d:(%d,%d) ", i+1, v.tab[i].lig, v.tab[i].col);
    }
    printf("\n---> ");

    // Lecture du choix
    int choix;
    while (1) {
        int ret = scanf("%d", &choix);
        if (ret == 1 && choix >= 1 && choix <= v.nb) {
            // correct
            *pion = v.tab[choix-1];
            break;
        } else {
            printf("erreur !\n---> ");
            while(getchar()!='\n');
        }
    }
}

// 7.2: L'ordinateur choisit un coup au hasard
// parmi les voisins possibles
int Hasard(int min, int max) {
    // renvoie un entier entre min et max inclus
    return rand() % (max - min + 1) + min;
}

void Coup_Ordi_Hasard(T_Case* pion, int nlig, int ncol) {
    T_Tab_Case v;
    Voisines(*pion, nlig, ncol, &v);
    int r = Hasard(0, v.nb-1);
    *pion = v.tab[r];
    printf("L'ordinateur (HASARD) deplace le pion en (%d,%d)\n", pion->lig, pion->col);
}

// 7.3: L'ordinateur cherche un coup gagnant
// => une voisine où nimber=0
// sinon, joue au hasard

void Coup_Ordi_Gagnant(T_Case* pion, int nlig, int ncol) {                    // fonction pour trouver un coup gagnant
    T_Tab_Case v;                                                            // tableau de voisins
    Voisines(*pion, nlig, ncol, &v);                                        // construit la liste des voisins

    for (int i = 0; i < v.nb; i++) {                                         // parcourt la liste des voisins
        int n = NimberCase(v.tab[i].lig, v.tab[i].col);                     //  appel a la fonction NimberCase pour obtenir le nimber de la case
        if (n == 0) {                                                                                // si le nimber de la case est 0
            *pion = v.tab[i];                                                                       // on met la case dans le pion
            printf("L'ordinateur (GAGNANT) deplace le pion en (%d,%d)\n", pion->lig, pion->col);   // on affiche le message
            return;                                                                                             // on retourne
        }
    }

    int r = Hasard(0, v.nb-1);                                                                                // sinon, on choisit un coup au hasard
    *pion = v.tab[r];                                                                                        // on met la case dans le pion
    printf("L'ordinateur (GAGNANT->pas possible) joue au hasard en (%d,%d)\n", pion->lig, pion->col);       // on affiche le message
}

/*
  ===============================
   8. Fonction principale (main)
  ===============================
*/

int main(void){
    srand((unsigned int)time(NULL));

    int nlig, ncol, niveau, next;                                                       // Parametres du jeu
    Parametres(&nlig, &ncol, &niveau, &next);                                          // Saisie des paramètres du jeu

    ConstruireNimbers(nlig, ncol);                                                   // Construction du tableau des nimbers

    T_Case pion;                                                                   // Position du pion
    pion.lig = 1;                                                                 // Position ligne du pion
    pion.col = 1;                                                                // Position colonne du pion

    printf("\n=== Debut de la partie ! ===\n");                                // Debut de la partie
    Affiche_Grille(pion, nlig, ncol);                                         // Affichage de la grille

    int fini = 0;                                                             // Variable pour la fin de la partie
    int joueurActuel = next; // 1=ordi, 2=joueur                             // Variable pour le joueur actuel

    while (!fini) {                                                           // Boucle de jeu tant que la partie n'est pas finie

        if (pion.lig == nlig && pion.col == ncol) {                             // Si le pion est dans le puits
            fini = 1;                                                          // La partie est finie
            break;                                                            // On sort
        }

        if (joueurActuel == 2) {
            // Coup du joueur
            Coup_Joueur(&pion, nlig, ncol);
        } else {
            // Coup de l'ordinateur
            double r = rand() / (double)RAND_MAX;
            // On applique les probabilités selon le niveau
            switch(niveau) {
                case 1:                                                        //Niveau 1 : toujours hasard : appel a Coup_Ordi_Hasard
                    Coup_Ordi_Hasard(&pion, nlig, ncol);
                    break;
                case 2:
                    if (r < 2.0/3.0) {                                       //Niveau 2 : 2/3 hasard, 1/3 gagnant : appel a Coup_Ordi_Hasard
                        Coup_Ordi_Hasard(&pion, nlig, ncol);                 //Niveau 2 : 1/3 gagnant: appel a Coup_Ordi_Gagnant    
                    } else {
                        Coup_Ordi_Gagnant(&pion, nlig, ncol);
                    }
                    break;
                case 3:
                    if (r < 1.0/3.0) {                                      //Niveau 3 : 1/3 hasard, appel a Coup_Ordi_Hasard
                        Coup_Ordi_Hasard(&pion, nlig, ncol);                //Niveau 3 : 2/3 gagnant, appel a Coup_Ordi_Gagnant
                    } else {
                        Coup_Ordi_Gagnant(&pion, nlig, ncol);
                    }
                    break;
                case 4:
                default:
                    Coup_Ordi_Gagnant(&pion, nlig, ncol);                    //Niveau 4 : toujours gagnant : appel a Coup_Ordi_Gagnant
                    break;
            }
        }
        if (joueurActuel == 1) joueurActuel = 2;                               // Changement de Joueurs
        else                  joueurActuel = 1;
    }

    // Fin de partie, celui qui a fait tomber le pion (joueurActuel) a gagné
    if (joueurActuel == 1) {
        printf("\nC'est termine. L'ordinateur a gagne.\n");
    } else {
        printf("\nC'est termine. Bravo, tu as gagne !\n");
    }

    return 0;
}

