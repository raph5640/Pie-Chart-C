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
void dessine(gdImagePtr *img, char* tab[]);
void telecharge_image(gdImagePtr img);
void affiche_tab(char* tab[]);
void clean(char* tab[]);
//voir la déclaration et la documentations de ces fonction a partir de la ligne 44

const char *separator = "=:";
const char *titre = "/home/raphael/pie/pie.png";

int main(int argc, char* argv[]) {

    char* tab[strlen(*argv)*2];//Notre tableau de valeurs
    gdImagePtr image;//Notre image

    initialise_tableau(argv, argc, tab, strlen(*argv)*2);
    initialise_image(&image);

    affiche_tab(tab);
    dessine(&image,tab);


    //Telechargement de l'image et libération de la mémoire (image/tableaux/pointeurs)
    telecharge_image(image);
    clean(tab);
    return 0;
}

void dessine(gdImagePtr *img, char* tab[]){
    srand( time( NULL ) );
    int couleur_random = gdImageColorAllocate(*img, rand()%256, rand()%256, rand()%256);
    gdImageFilledArc(*img, 500, 500, 500,500,100,200,couleur_random, gdArc);
}

/* Initialise le tableau de valeur (tab) avec les données entrée en arguments lors du lancement du programme avec une commande
 * elle initialise les valeurs de tab[]
 * return : rien
 * param : char* argv[], int argc, char* tab[], int tab_size
 */
void initialise_tableau(char* argv[], int argc, char* tab[], int tab_size){
    int k = 0;
    for (int i = 0; i < argc && k < tab_size; i++) {
        char* strtoken = strtok(argv[i], separator);
        while (strtoken != NULL && k < tab_size) {
            tab[k] = strdup(strtoken);      //Alloue la mémoire pour chaque case du tableau et insere sa valeur dedans
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

void affiche_tab(char* tab[]){
    for(int i=0;i<strlen(*tab) && tab[i]!=NULL;i++){
        printf("%s\n",tab[i]);
    }
}

/* Fonction clean
 * Elle libère la mémoire alloué par le tableau de valeurs qui initialise les arguments données en entrée lors de l'éxécution du programme
 * return : rien
 * param : aucun
 */
void clean(char* tab[]){
    for(int i=0; i<strlen(*tab) && tab[i]!=NULL;i++){
        free(tab[i]);
    }
}
