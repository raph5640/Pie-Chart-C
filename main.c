#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gd.h>
#include <math.h>
#include <time.h>
/* @auteur Raphael De Oliveira
 * PROJET PIE CHART réalisé par @auteur
 */

//Déclaration des fonctions ici
void initialise_tableau(char* argv[], int argc, char* tab[], int tab_size);
void initialise_image(gdImagePtr *img);
void dessine(gdImagePtr *img, char* tab[], int tab_size);
void telecharge_image(gdImagePtr img);
void affiche_tab(char* tab[], int tab_size);
void clean(char* tab[],int size);
//voir la déclaration et la documentations de ces fonction a partir de la ligne 44

const char *separator = "=:";                           //les séparateurs possibles pour les arguments lors du lancement du programme
const char *titre = "/home/raphael/pie/pie.png";        //Chemin de l'image ou l'on souhaite qu'elle soit crée

int main(int argc, char* argv[]) {

    char* tab[(argc * 2)-2]; // Notre tableau de valeurs
    printf("taille tab=%d\n",(argc * 2)-2);
    gdImagePtr image;    // Notre image

    initialise_tableau(argv, argc, tab, sizeof(tab) / sizeof(tab[0]));
    initialise_image(&image);

    affiche_tab(tab, sizeof(tab) / sizeof(tab[0]));
    dessine(&image, tab, sizeof(tab)/sizeof(tab[0]));

    // Téléchargement de l'image et libération de la mémoire (image/tableaux/pointeurs)
    telecharge_image(image);
    clean(tab, sizeof(tab) / sizeof(tab[0]));
    return 0;
}

void dessine(gdImagePtr *img, char* tab[], int tab_size){
    srand(time(NULL));
    int couleur_random = gdImageColorAllocate(*img, rand() % 256, rand() % 256, rand() % 256);

    int size_data = tab_size / 2;
    printf("data_size=%d\n",size_data);
    int data_pourcentage[size_data]; //Tableau data_pourcentage contient tout les pourcentages type (int)
    char* data_pays[size_data];      //Tableau data_pays contient tout les noms de pays type (char*)
    int k = 0;
    int l = 0;
    //Initialisation du tableau data a l'aide du tableau de base exemple: de tab[] = {"Paris","50","Londre","12","Tokyo","30"} vers data[]={50,12,30}
    for (int i = 0; i < tab_size && k < size_data; i++) {
        if (atoi(tab[i]) != 0) {
            data_pourcentage[k] = atoi(tab[i]);
            printf("data_pourcentage[%d]=%d\n", k, data_pourcentage[k]);
            k++;
        }else{
            data_pays[l] = tab[i];
            printf("data_pays[%d]=%s\n", l, data_pays[l]);
            l++;
        }
    }
    gdImageFilledArc(*img, 500,500, 200,200, 0,250, couleur_random,gdArc);
    //Fin de l'Initialisation du tableau data a l'aide du tableau de base
}

/* Initialise le tableau (tab) avec les données entrée en arguments lors du lancement du programme avec une commande exemple :tab[] = {"Paris","50","Londre","12","Tokyo","30"}
 * elle initialise les valeurs de tab[]
 * return : rien
 * param : char* argv[], int argc, char* tab[], int tab_size
 */
void initialise_tableau(char* argv[], int argc, char* tab[], int tab_size) {
    for (int i = 0; i < tab_size; i++) {
        tab[i] = NULL;
    }

    int k = 0;
    for (int i = 1; i < argc && k < tab_size; i++) {
        char* strtoken = strtok(argv[i], separator);
        while (strtoken != NULL && k < tab_size) {
            tab[k] = strdup(strtoken);      //Alloue la mémoire pour les chaines de caracteres a initialiser dans le tableau de valeur et initialise la valeur
            k++;
            strtoken = strtok(NULL, separator);
        }
    }
}

/* Fonction initialise_image
 * Elle crée l'image défini ca taille en pixel et met un fond blanc
 * return : rien
 * param : gdImagePtr
 */
void initialise_image(gdImagePtr *img){

    *img = gdImageCreate(1000, 1000);
    //definition des couleurs
    int white = gdImageColorAllocate(*img,255,255,255);
    //création de formes
    gdImageFilledRectangle(*img, 0,0, 999 ,999,white);  //Met le fond en blanc

}

/* Fonction telecharge_image
 * Elle télécharge l'image au format .png
 * return : rien
 * param : gdImagePtr
 */
void telecharge_image(gdImagePtr img){
    FILE *fichier;
    fichier = fopen(titre, "wb");
    gdImagePng(img, fichier);
    fclose(fichier);
    gdImageDestroy(img); //Destruction de l'image ici (liberation de la memoire)
}

void affiche_tab(char* tab[], int tab_size) {
    for (int i = 0; i < tab_size && tab[i] != NULL; i++) {
        printf("tab[%d]=%s\n",i, tab[i]);
    }
}

/* Fonction clean
 * Elle libère la mémoire alloué par le tableau de valeurs qui initialise les arguments données en entrée lors de l'éxécution du programme
 * return : rien
 * param : char* tab[], int tab_size
 */
void clean(char* tab[], int tab_size){
    for(int i=0; i<tab_size && tab[i]!=NULL;i++){
        free(tab[i]);
    }
    return;
}
