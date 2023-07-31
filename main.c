#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gd.h>
/* @auteur = Raphael De Oliveira
 */

const char *separator = "=:";

void initialise(char* argv[], int argc, char* tab[], int tab_size){
    int k = 0;
    for (int i = 0; i < argc && k < tab_size; i++) {
        char* strtoken = strtok(argv[i], separator);
        while (strtoken != NULL && k < tab_size) {
            tab[k] = strdup(strtoken);
            k++;
            strtoken = strtok(NULL, separator);
        }
    }
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
void telecharge_image(gdImagePtr *image){
    FILE *fichier;
    fichier = fopen("/home/raphael/pie/pie.png", "wb");
    gdImagePng(image, fichier);
    fclose(fichier);
    gdImageDestroy(image);
}
int main(int argc, char* argv[]) {
    const int tab_size= strlen(*argv)*2;
    char* tab[tab_size];

    initialise(argv, argc, tab, tab_size);

    gdImagePtr image = gdImageCreate(1000, 1000);
    int red = gdImageColorAllocate(image, 255, 0, 0);
    int green = gdImageColorAllocate(image, 0, 255, 0);
    int blue = gdImageColorAllocate(image, 0, 0, 255);

    gdImageLine(image, 1000, 0, 0, 1000, blue);

    affiche_tab(tab);
    telecharge_image(image);
    clean(tab);
    return 0;
}
