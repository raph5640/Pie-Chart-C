#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gd.h>
#include <math.h>
#include <gdfontg.h>
#include <time.h>
/* @auteur Raphael De Oliveira
 * PROJET PIE CHART réalisé par @auteur
 */

//Déclaration des fonctions ici
void initialise_tableau(char* argv[], int argc, char* tab[], int tab_size);
void initialise_image(gdImagePtr *img);
void dessine(gdImagePtr *img, char* tab[], int tab_size);
void telecharge_image(gdImagePtr img, int wich);
void affiche_tab(char* tab[], int tab_size);
void clean(char* tab[], int tab_size, gdImagePtr img1,gdImagePtr img2);
void dessine_histogramme(gdImagePtr *img, char* tab[], int tab_size);
//voir la déclaration et la documentations de ces fonction a partir de la ligne 44

const char *separator = "=:";                                                //les séparateurs possibles pour les arguments lors du lancement du programme
const char *titre_pie = "./image_repository/pie.png";        //Chemin de l'image pie ou l'on souhaite qu'elle soit crée
const char *titre_histo = "./image_repository/histogram.png";//Chemin de l'image histogramme ou l'on souhaite qu'elle soit crée
int fond;   //fond = 1 si le fond d'écran est blanc / fond =2 si le fond d'écran est noir

int main(int argc, char* argv[]) {

    char* tab[(argc * 2)-2]; //Notre tableau de valeurs
    gdImagePtr image;       //Notre image pie
    gdImagePtr image_histo; //Notre image histogramme

    initialise_tableau(argv, argc, tab, sizeof(tab) / sizeof(tab[0]));
    affiche_tab(tab, sizeof(tab) / sizeof(tab[0]));

    initialise_image(&image);       //Crée l'objet image et met un fond blanc
    initialise_image(&image_histo); //Crée l'objet image histogramme et met un fond blanc

    dessine(&image, tab, sizeof(tab)/sizeof(tab[0]));   //Dessine le pie chart dans l'objet image
    dessine_histogramme(&image_histo, tab, sizeof(tab)/sizeof(tab[0]));

    // Téléchargement de l'image et libération de la mémoire (image/tableaux/pointeurs)
    telecharge_image(image,0);  //Télécharge l'image pie
    telecharge_image(image_histo,1); //Télécharge l'image histogramme
    clean(tab, sizeof(tab) / sizeof(tab[0]),image,image_histo); //Libération de mémoire pointeurs+images
    return 0;
}

/* Fonction dessine : sépare le tableau général en deux tableaux data_pourcentage[] et data_pays[] puis dessine les secteurs et les titres du camembert
 * elle convertit tab[]---> data_pourcentage[]={50,12,30} et data_pays={"Paris","Londre","Tokyo"} et dessine
 * return : rien
 * param : gdImagePtr *img, char* tab[], int tab_size
 */
void dessine(gdImagePtr *img, char* tab[], int tab_size){

    int size_data = tab_size / 2;

    int data_pourcentage[size_data]; // Tableau data_pourcentage contient tous les pourcentages (type int)
    char* data_pays[size_data];      // Tableau data_pays contient tous les noms de pays (type char*)

    for(int i=0; i<size_data; i++){
        data_pays[i] = strdup("");
    }
    // Initialisation des tableaux data à l'aide du tableau de base exemple:
    // de tab[] = {"Paris","50","Londre","12","Tokyo","30"} vers data_pourcentage[]={50,12,30} et data_pays={"Paris","Londre","Tokyo"}

    int k = 0;  //Incrémenteur pour le tableau data_pourcentage
    int l = 0;  //Incrémenteur pour le tableau data_pays
    printf("------------------------------------------------\n");
    for (int i = 0; i < tab_size && k < size_data; i++) {
        if (atoi(tab[i]) != 0) {
            data_pourcentage[k] = atoi(tab[i]);
            printf("data_pourcentage[%d]=%d\n", k, data_pourcentage[k]);
            k++;
        }else{
            data_pays[l]=strdup(tab[i]);
            printf("data_pays[%d]=%s\n", l, data_pays[l]);
            l++;
        }
    }

    //Vérification des conditions sur les pourcentages pour éviter que les pourcentages soient supérieur strict a 100
    int total_pourcentage=0;
    for(int i=0; i<size_data; i++){
        total_pourcentage+=data_pourcentage[i];
    }

    if(total_pourcentage>100){  //Si la somme des pourcentages est supérieur strict a 100 on ne crée pas d'image et on exit avec une erreur 1
        printf("WARNING : La somme des pourcentages entré (%d) est supérieur strictement a 100  !!\n",total_pourcentage);
        printf("!!L'image n'a pas été crée!!\n");
        exit(1);
    }else if(total_pourcentage<100){    //On crée quand meme l'image et on complete les pourcentage avec le pays "LE FAMEUX PAYS IMAGINAIRE"
        printf("WARNING : La somme des pourcentages entré (%d) est inférieur strictement a 100  !!\n",total_pourcentage);
    }

    //On déssine les parts du gateau
    int debut =0;
    int fin =0;

    //Coordonnées du centre du gateau ou camembert
    int centreX = 500;
    int centreY = 500;

    //Distance du rayon du camembert en pixel
    int rayon = 500;
    srand(time(NULL));
    printf("------------------------------------------------\n");
    //Couleurs
    int white = gdImageColorAllocate(*img, 255, 255, 255);
    int black = gdImageColorAllocate(*img, 0, 0, 0);

    for(int i=0; i<size_data; i++){

        //DESSIN DES PARTS DE GATEAU
        int couleur_random = gdImageColorAllocate(*img, rand() % 256, rand() % 256, rand() % 256);      //On génère une nouvelle couleur aléatoire pour chaque part du camembert
        fin = debut + ((double)data_pourcentage[i]/100)*360;

        int epaisseur = 20; //epaisseur en pixel du camembert pour le 3D
        for(int i=0;i<epaisseur;i++){
            gdImageFilledArc(*img, centreX,centreY-i, rayon,rayon, debut,fin, couleur_random,gdArc); //On dessine la part de camembert
            centreY=centreY-1;
        }

        //Ajout du nom du pays à côté des parts du camembert (coordonnées polaires)
        int stringAngle= debut + ((fin-debut)/2); //Angle moyen entre debut et fin
        int stringRayon= (rayon/2)+50; //Distance du rayon pour les noms de pays

        //convertit coordonnée polaire en coordonnée cartésienne
        double stringX= centreX + (stringRayon * cos(stringAngle*M_PI/180));
        double stringY= centreY + (stringRayon * sin(stringAngle*M_PI/180));

        // Dessiner le texte à côté des parts du camembert en utilisant gdImageString
        printf("%s_angle=%d\n",data_pays[i],stringAngle);
        printf("%s_X=%lf\n",data_pays[i],stringX);
        printf("%s_Y=%lf\n",data_pays[i],stringY);
        printf("\n");
        gdFontPtr smallFont = gdFontGetGiant();
        gdImageLine(*img, centreX, centreY, stringX+5, stringY+5, couleur_random);
        if(fond==1){
            gdImageString(*img, smallFont, stringX, stringY, (unsigned char*)data_pays[i], black);      //Met le texte en noir si le fond est blanc
        }else{
            gdImageString(*img, smallFont, stringX, stringY, (unsigned char*)data_pays[i], white);      //Met le texte en blanc si le fond est noir
        }
        debut = fin;
    }

    //Libération de la mémoire pour ce tableau data_pays ephémere
    for (int i = 0; i < size_data; i++) {
        free(data_pays[i]);
    }
}

/* Fonction dessine_histogramme : dessine un histogramme à partir du tableau tab[]
 * return : rien
 * param : gdImagePtr *img, int data_pourcentage[], int size_data
 */
void dessine_histogramme(gdImagePtr *img, char* tab[], int tab_size) {

    int size_data = tab_size / 2;

    int data_pourcentage[size_data]; // Tableau data_pourcentage contient tous les pourcentages (type int)
    char* data_pays[size_data];      // Tableau data_pays contient tous les noms de pays (type char*)

    for(int i=0; i<size_data; i++){
        data_pays[i] = strdup("");
    }
    // Initialisation des tableaux data à l'aide du tableau de base exemple:
    // de tab[] = {"Paris","50","Londre","12","Tokyo","30"} vers data_pourcentage[]={50,12,30} et data_pays={"Paris","Londre","Tokyo"}

    int k = 0;  //Incrémenteur pour le tableau data_pourcentage
    int l = 0;  //Incrémenteur pour le tableau data_pays
    printf("------------------------------------------------\n");
    for (int i = 0; i < tab_size && k < size_data; i++) {
        if (atoi(tab[i]) != 0) {
            data_pourcentage[k] = atoi(tab[i]);
            printf("data_pourcentage[%d]=%d\n", k, data_pourcentage[k]);
            k++;
        }else{
            data_pays[l]=strdup(tab[i]);
            printf("data_pays[%d]=%s\n", l, data_pays[l]);
            l++;
        }
    }
    //Coordonnées de départ de l'histogramme
    int startX = 100;
    int startY = 900;

    //Largeur et hauteur d'une barre de l'histogramme
    int barWidth = 150;
    int maxBarHeight = 800; // Hauteur maximale de la barre

    //Couleurs
    int white = gdImageColorAllocate(*img, 255, 255, 255);
    int black = gdImageColorAllocate(*img, 0, 0, 0);

    // Dessine les barres de l'histogramme
    for (int i = 0; i < size_data; i++) {
        int barHeight = (int)(((double)data_pourcentage[i] / 100) * maxBarHeight);

        int x1 = startX + i * barWidth;
        int y1 = startY - barHeight;
        int x2 = x1 + barWidth - 1;
        int y2 = startY - 1;

        int couleur_random = gdImageColorAllocate(*img, rand() % 256, rand() % 256, rand() % 256);
        gdImageFilledRectangle(*img, x1, y1, x2, y2, couleur_random);

        // Dessine le texte indiquant la valeur de la barre
        char valeur[10];
        snprintf(valeur, sizeof(valeur), "%d", data_pourcentage[i]);
        int texteX = x1 + barWidth / 2 - 10;
        int texteY = y1 - 20;
        gdFontPtr smallFont = gdFontGetGiant();
        if(fond ==1){
            gdImageString(*img, smallFont, texteX, texteY, (unsigned char *)valeur, black);
        }else{
            gdImageString(*img, smallFont, texteX, texteY, (unsigned char *)valeur, white);
        }

        // Dessine le nom du pays en dessous de la barre
        int nomX = x1 + barWidth / 2 - (strlen(data_pays[i]) * 3); // Ajustement pour centrer le texte sous la barre
        int nomY = startY + 10;
        gdFontPtr mediumBoldFont = gdFontGetGiant();
        if(fond==1){
            gdImageString(*img, mediumBoldFont, nomX, nomY, (unsigned char *)data_pays[i], black);      //Met le text en noir si le fond est blanc
        }else{
            gdImageString(*img, mediumBoldFont, nomX, nomY, (unsigned char *)data_pays[i], white);      //Met le text en blanc si le fond est noir
        }

    }
    //Libération de la mémoire pour ce tableau data_pays ephémere
    for (int i = 0; i < size_data; i++) {
        free(data_pays[i]);
    }
}


/* Initialise le tableau (tab) avec les données entrée en arguments lors du lancement du programme avec une commande exemple :tab[] = {"Paris","50","Londre","12","Tokyo","30"}
 * elle initialise les valeurs de tab[]
 * return : rien
 * param : char* argv[], int argc, char* tab[], int tab_size
 */
void initialise_tableau(char* argv[], int argc, char* tab[], int tab_size) {
    if((argc * 2)-2==0){//On vérifie qu'il y a au moins 1 argument entrée, sinon on arrête le programme
        printf("WARNING : Vous n'avez entré aucun arguments !!\n");
        exit(1);
    }

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
    int black = gdImageColorAllocate(*img,0,0,0);

    //Choix de couleur du fond d'écran (noir ou blanc)
    int x=0;

    while(x!=1 && x!=2){
        printf("Fond d'écran en blanc ou en noir :\nTaper 1 : Fond d'écran en blanc\nTaper 2 : Fond d'écran en noir\n");
        scanf("%d",&x);
    }
    //création de formes
    if(x==1){
        gdImageFilledRectangle(*img, 0,0, 999 ,999,white);  //Met le fond en blanc
    }else if(x==2){
        gdImageFilledRectangle(*img, 0,0, 999 ,999,black);  //Met le fond en noir
    }
    fond = x;

}

/* Fonction telecharge_image
 * Elle télécharge l'image au format .png
 * return : rien
 * param : gdImagePtr
 */
void telecharge_image(gdImagePtr img,int wich){
    FILE *fichier;
    if(wich==0){
        fichier = fopen(titre_pie, "wb");
    }else if(wich==1){
        fichier = fopen(titre_histo, "wb");
    }
    gdImagePng(img, fichier);
    fclose(fichier);
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
void clean(char* tab[], int tab_size, gdImagePtr img1, gdImagePtr img2){
    for(int i=0; i<tab_size && tab[i]!=NULL;i++){
        free(tab[i]);
    }
    gdImageDestroy(img1); //Destruction de l'image ici (liberation de la memoire)
    gdImageDestroy(img2);
    return;
}
