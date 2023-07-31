#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gd.h>
#include <math.h>
/* @auteur = Raphael De Oliveira
 * PROJET PIE CHART
 */

const char *separator = "=:";

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

void initialise_image(gdImagePtr *img){

    *img = gdImageCreate(1000, 1000);
    //definition des couleurs
    int white = gdImageColorAllocate(*img,255,255,255);
    //création de formes
    gdImageFilledRectangle(*img, 0,0, 999 ,999,white);  //Met le fond en blanc

}

void telecharge_image(gdImagePtr img){
    FILE *fichier;
    fichier = fopen("/home/raphael/pie/pie.png", "wb");
    gdImagePng(img, fichier);
    fclose(fichier);
    gdImageDestroy(img); //Destruction de l'image ici (liberation de la memoire)
}

void clean(char* tab[]){
    for(int i=0; i<strlen(*tab) && tab[i]!=NULL;i++){
        free(tab[i]);
    }
}

void affiche_tab(char* tab[]){
    for(int i=0;i<strlen(*tab) && tab[i]!=NULL;i++){
        printf("%s\n",tab[i]);
    }
}

void dessine(gdImagePtr *img, char* tab[]){
    srand( time( NULL ) );
    int couleur_random = gdImageColorAllocate(*img, rand()%256, rand()%256, rand()%256);
    gdImageFilledArc(*img, 500, 500, 500,500,0,200,couleur_random, gdArc);
}

int main(int argc, char* argv[]) {
    const int tab_size= strlen(*argv)*2;
    char* tab[tab_size];

    initialise_tableau(argv, argc, tab, tab_size);

    gdImagePtr image;
    initialise_image(&image);

    dessine(&image,tab);
    affiche_tab(tab);

    //Telechargement de l'image et libération de la mémoire (image/tableaux/pointeurs)
    telecharge_image(image);
    clean(tab);
    return 0;
}
