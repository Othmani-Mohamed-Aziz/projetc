#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include<windows.h>

// Structure pour une personne
typedef struct Personne {
    int id;
    char nom[50];
    char prenom[50];
    char qualification[100];
    struct Personne *suivant;
} Personne;

// Structure pour un nœud dans la pile
typedef struct Action {
    char description[100];
    int id;
    char type[10];
    struct Action *suivant;
} Action;

Personne *teteListe = NULL;
Action *sommetPile = NULL;
// Fonction pour enregistrement dans la base de donnés
void sauvegarderDansFichier() {
    FILE *fichier = fopen("employés.txt", "w");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    Personne *temp = teteListe;
    while (temp != NULL) {
        fprintf(fichier, "%d %s %s %s\n", temp->id, temp->nom, temp->prenom, temp->qualification);
        temp = temp->suivant;
    }

    fclose(fichier);
    printf("Données sauvegardées dans le fichier.\n");
}
// verification que l id n'éxiste pas
bool IDNonPresent(int tableau[], int taille, int element) {
    for (int i = 0; i < taille; i++) {
        if (tableau[i] == element) {
            return false;
        }
    }
    return true;
}
// Suppressoion de Id de tableau
bool supprimerID(int tableau[], int taille, int Id){
    for (int i=0;i<taille;i++){
        if (tableau[i]==Id){
            tableau[i]= tableau[taille-i];
            tableau[taille]=0;
            return true;
        }
    }
    return false;
}
// Fonction pour ajouter une personne
void ajouterPersonne(int id, char nom[], char prenom[], char qualification[]) {
    Personne *nouvellePersonne =malloc(sizeof(Personne));
    nouvellePersonne->id = id;
    strcpy(nouvellePersonne->nom, nom);
    strcpy(nouvellePersonne->prenom, prenom);
    strcpy(nouvellePersonne->qualification, qualification);
    nouvellePersonne->suivant = teteListe;
    teteListe = nouvellePersonne;

    // Ajouter l'action dans la pile
    Action *nouvelleAction =malloc(sizeof(Action));
    sprintf(nouvelleAction->description, "Ajout de la personne ID: %d", id);
    strcpy(nouvelleAction->type, "ajout");
    nouvelleAction->id = id;
    nouvelleAction->suivant = sommetPile;
    sommetPile = nouvelleAction;

    printf("Personne ajoutée avec succès.\n");
    // Sauvegarder dans le fichier
    sauvegarderDansFichier();
}

// Fonction pour afficher la liste des personnes
void afficherPersonnes() {
    if (teteListe == NULL) {
        printf("La liste des personnes est vide.\n");
        return;
    }

    Personne *temp = teteListe;
    printf("Liste des personnes:\n");
    while (temp != NULL) {
        printf("ID: %d, Nom: %s, Prénom: %s, qualification: %s\n",
               temp->id, temp->nom, temp->prenom, temp->qualification);
        temp = temp->suivant;
    }
}

// Fonction pour rechercher une personne par ID
Personne* rechercherPersonne(int id) {
    Personne *temp = teteListe;
    while (temp != NULL) {
        if (temp->id == id) {
            return temp;
        }
        temp = temp->suivant;
    }
    return NULL;
}
// fonction pour afficher un personne de la liste  par ID
void afficherpersonneID(int id){
    Personne *temp=teteListe;
    while (temp != NULL && temp->id != id) {
        temp = temp->suivant;
    }
    if (temp == NULL) {
        printf("Personne avec ID %d introuvable.\n", id);
        return;
    }else{
        printf("Personne avec ID %d trouvé avec succés.\n",id);
        printf("ID: %d, Nom: %s, Prénom: %s, qualification: %s\n",
               temp->id, temp->nom, temp->prenom, temp->qualification);
    }
}
// fonction pour afficher un personne de la liste  par qualification
void afficherqualification(char qualification[]){
    Personne *temp=teteListe;
    while(temp != NULL && strcmp(temp->qualification, qualification) != 0){
        temp=temp->suivant;
    }
    if (temp == NULL) {
        printf("Personnes avec qualification %s introuvable.\n", qualification);
        return;
    }else{
        printf("Personnes avec qualification %s trouvé avec succés.\n",qualification);
        printf("ID: %d, Nom: %s, Prénom: %s, qualification: %s\n",
               temp->id, temp->nom, temp->prenom, temp->qualification);
    }

}

// Fonction pour modifier une personne
void modifierPersonne(int id, char nom[], char prenom[], char qualification[]) {
    Personne *personne = rechercherPersonne(id);
    if (personne != NULL) {
        strcpy(personne->nom, nom);
        strcpy(personne->prenom, prenom);
        strcpy(personne->qualification, qualification);

        // Ajouter l'action dans la pile
        Action *nouvelleAction = malloc(sizeof(Action));
        sprintf(nouvelleAction->description, "Modification de la personne ID: %d", id);
        strcpy(nouvelleAction->type, "modification");
        nouvelleAction->id = id;
        nouvelleAction->suivant = sommetPile;
        sommetPile = nouvelleAction;

        printf("Personne modifiée avec succès.\n");
        // Sauvegarder dans le fichier
        sauvegarderDansFichier();
    } else {
        printf("Personne avec ID %d introuvable.\n", id);
    }
}

// Fonction pour supprimer une personne
void supprimerPersonne(int id) {
    Personne *temp = teteListe, *precedent = NULL;

    while (temp != NULL && temp->id != id) {
        precedent = temp;
        temp = temp->suivant;
    }

    if (temp == NULL) {
        printf("Personne avec ID %d introuvable.\n", id);
        return;
    }

    if (precedent == NULL) {
        teteListe = temp->suivant;
    } else {
        precedent->suivant = temp->suivant;
    }
    free(temp);

    // Ajouter l'action dans la pile
    Action *nouvelleAction =malloc(sizeof(Action));
    sprintf(nouvelleAction->description, "Suppression de la personne ID: %d", id);
    strcpy(nouvelleAction->type, "suppression");
    nouvelleAction->id = id;
    nouvelleAction->suivant = sommetPile;
    sommetPile = nouvelleAction;

    printf("Personne supprimée avec succès.\n");
    // Sauvegarder dans le fichier
    sauvegarderDansFichier();
}

// Fonction pour afficher la pile des actions
void afficherActions() {
    if (sommetPile == NULL) {
        printf("La pile des actions est vide.\n");
        return;
    }

    Action *temp = sommetPile;
    printf("Pile des actions:\n");
    while (temp != NULL) {
        printf("- %s\n", temp->description);
        temp = temp->suivant;
    }
}

// Fonction pour annuler la dernière action
void annulerDerniereAction() {
    if (sommetPile == NULL) {
        printf("Aucune action à annuler.\n");
        return;
    }

    Action *derniereAction = sommetPile;
    sommetPile = sommetPile->suivant;

    if (strcmp(derniereAction->type, "ajout") == 0) {
        supprimerPersonne(derniereAction->id);
    } else{
         printf("Impossible d'annuler l'action .\n");
    }

    free(derniereAction);
}

// Fonction principale
int main() {
    int choix, id;
    char nom[50], prenom[50], qualification[100];
    int tableauID[100];
    int i = 0;
    int valid;

    do {
        system("cls");
        printf("========================================================================================================================\n");
        printf("|\t\t\t\t\t***********\t\t\t\t\t\t\t\t\t|\n");

        printf("|\t\t\t\t\t| Bienvenue |\t\t\t\t\t\t\t\t\t|\n");
        printf("|\t\t\t\t\t***********\t\t\t\t\t\t\t\t\t|\n");
        printf("|\t\t\t\t1. Ajouter une personne\t\t\t\t\t\t\t\t\t\t|\n");
        printf("|\t\t\t\t2. Modifier une personne\t\t\t\t\t\t\t\t\t|\n");
        printf("|\t\t\t\t3. Supprimer une personne\t\t\t\t\t\t\t\t\t|\n");
        printf("|\t\t\t\t4. Afficher la liste des personnes\t\t\t\t\t\t\t|\n");
        printf("|\t\t\t\t5. Afficher la pile des actions\t\t\t\t\t\t\t\t\t|\n");
        printf("|\t\t\t\t6. Annuler la dernière action\t\t\t\t\t\t\t\t\t|\n");
        printf("|\t\t\t\t7. recherce d'un personne avec ID\t\t\t\t\t\t\t\t\t|\n");
        printf("|\t\t\t\t8. recherce d'un personne avec qualification spécifique\t\t\t\t\t|\n");
        printf("|\t\t\t\t0. Quitter\t\t\t\t\t\t\t\t\t\t\t|\n");
        printf("|\t\t\t\t\t\tEntrez votre choix: \t\t\t\t\t\t\t\t|\n");
        printf("========================================================================================================================\n");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                system("cls");
                do{
                    printf("Entrez l'ID: ");
                    scanf("%d", &id);
                }while(!(IDNonPresent(tableauID, 100, id)));
                tableauID[i]=id;
                i++;
                printf("Entrez le nom: ");
                scanf("%s", nom);
                printf("Entrez le prénom: ");
                scanf("%s", prenom);
                printf("Entrez la qualification: ");
                scanf("%s", qualification);
                ajouterPersonne(id, nom, prenom, qualification);
                sleep(3);
                break;

            case 2:
                system("cls");
                printf("Entrez l'ID de la personne à modifier: ");
                scanf("%d", &id);
                printf("Entrez le nouveau nom: ");
                scanf("%s", nom);
                printf("Entrez le nouveau prénom: ");
                scanf("%s", prenom);
                printf("Entrez la nouvelle qualification: ");
                scanf("%s", qualification);
                modifierPersonne(id, nom, prenom, qualification);
                sleep(3);
                break;

            case 3:
                system("cls");
                printf("Entrez l'ID de la personne à supprimer: ");
                scanf("%d", &id);
                supprimerPersonne(id);
                if(supprimerID(tableauID, 100, id)){
                    i--;
                };
                sleep(3);
                break;

            case 4:
                system("cls");
                afficherPersonnes();
                sleep(3);
                break;

            case 5:
                system("cls");
                afficherActions();
                sleep(3);
                break;

            case 6:
                system("cls");
                annulerDerniereAction();
                sleep(3);
                break;

            case 7:
                system("cls");
                printf("Entrez l'ID de la personne : ");
                scanf("%d",&id);
                afficherpersonneID(id);
                sleep(3);
                break;
            case 8:
                system("cls");
                printf("Entrez la qualificatin a rechercher ");
                scanf("%s",qualification);
                afficherqualification(qualification);
                sleep(3);
                break;
            case 0:
                printf("Au revoir!\n");
                sleep(3);
                break;

            default:
                printf("Choix invalide, veuillez réessayer.\n");
        }
    } while (choix != 0);

    return 0;
}
