#include <stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>


typedef struct
{
    int minute;
    int heure;
    int jour;
}Horaire;
typedef struct
{
    int row;
    int column;
    int codeRue;
}Subdivision;
typedef struct
{
   int id;
   int idLigne;
   int sens;
   Horaire horaireDateDepart;
}Tache;
typedef struct
{
    int id;
    Horaire horaireDateSouhaite;
    Horaire dureeHeureMinute;
    int typeActivite;
    int idZoneSrc;
    int idZoneDest;
    int idLigne;
    int sens;
}Activite;
typedef struct
{
    int cin;
    int idZoneHabitat;
    int idZoneTravail;
    int idZoneCourante;
    int intentionVoyage;
    int idLigneCourante;
    int sensCourant;
    int idZoneDest;
    int nombreActivites;
    Activite activites[8];
}Personne;

typedef struct
{
  int id;
  int nombreZones;
  int idZones[10];
}Ligne;
typedef struct
{
   int id;
   int row;
   int column;
   int nombrePersonnes;
   Personne personnes[300];
}Zone;
typedef struct
{
  int idZoneSrc;
  int idZoneDest;
  int distance;
  Horaire dureeAproximative;
  int nombresubdivisions;
  Subdivision parcours[20];
}Troncon;
typedef struct
{
    int zoneSrc;
    int zoneDest;
    int idLigne;
    int sens;
    int idBus;
    Horaire dateVente;
    int distance;
    int prix;
}Ticket;

typedef struct
{
   int id;
   int idZoneCourante;
   int row;
   int column;
   int enRoute;
   int idLigneCourante;
   int sensCourant;
   int nombreTaches;
   Tache taches[10];
   int nombrePassagers;
   Personne passagers[50];
   int nombreTickets;
   Ticket tickets[1000];
}Bus;
typedef struct
{
    int rue;
    int zone;
    int bus;
}Casecarte;


void chargerZones(Zone zones[], int *nombreZones)
{
  FILE *fptr = fopen("zones.txt", "r");
  if (fptr == NULL)
        exit(1);
  int i=-1;
  while (!feof(fptr))
        {
            i++;
            fscanf(fptr, "%d %d %d", &zones[i].id, &zones[i].row, &zones[i].column);
            zones[i].nombrePersonnes=0;
        }
    *nombreZones=i+1;
    fclose(fptr);
}

void chargerLignes(Ligne lignes[], int *nombreLignes)
{
    int i=-1,nbLignes=0,j=0;
    FILE *fptr = fopen("lignes.txt", "r");
    if (fptr == NULL)
        exit(1);

    while(!feof(fptr))
    {
        if(nbLignes==0)
        {
            i++;
            fscanf(fptr, "%d %d", &lignes[i].id, &lignes[i].nombreZones);
            nbLignes=lignes[i].nombreZones;
        }
        else
        {
            fscanf(fptr,"%d",&lignes[i].idZones[j]);
            j++;
            nbLignes--;
        }
    }
    *nombreLignes=i+1;
    fclose(fptr);
}

void chargerTroncons(Troncon troncons[], int *nombreTroncons)
{
    int i = -1, j ,nbsubdivisions=0;
    FILE *fptr =fopen("troncons.txt", "r");
    if (fptr == NULL) {
        exit(1);
    }

    while (!feof(fptr)) {
        if(nbsubdivisions==0)
        {
            i++;
            fscanf(fptr,"%d %d %d %d %d %d %d",&troncons[i].idZoneSrc, &troncons[i].idZoneDest, &troncons[i].distance,&troncons[i].dureeAproximative.jour, &troncons[i].dureeAproximative.heure, &troncons[i].dureeAproximative.minute, &troncons[i].nombresubdivisions);
            nbsubdivisions=troncons[i].nombresubdivisions;
            j=0;
        }
        else
        {
            fscanf(fptr, "%d %d %d", &troncons[i].parcours[j].row, &troncons[i].parcours[j].column, &troncons[i].parcours[j].codeRue);
            j++;
            nbsubdivisions--;
        }
    }
     *nombreTroncons=i+1;
    fclose(fptr);
}

void chargerFlotteBus(Bus flotteBus[], int *nombreBus)
{
     int i = -1, j ,nbTchaches=0;
     FILE *fptr =fopen("flotteBus.txt", "r");
     if (fptr == NULL)
        exit(1);

    while (!feof(fptr))
    {
        if(nbTchaches==0)
        {
            i++;
            fscanf(fptr,"%d %d %d %d %d %d %d %d",&flotteBus[i].id,&flotteBus[i].idZoneCourante,&flotteBus[i].row,&flotteBus[i].column,&flotteBus[i].enRoute,&flotteBus[i].idLigneCourante,&flotteBus[i].sensCourant,&flotteBus[i].nombreTaches);
            nbTchaches=flotteBus[i].nombreTaches;
            j=0;
        }
        else
        {
            fscanf(fptr,"%d %d %d %d %d %d",&flotteBus[i].taches[j].id,&flotteBus[i].taches[j].idLigne,&flotteBus[i].taches[j].sens,
                   &flotteBus[i].taches[j].horaireDateDepart.jour,&flotteBus[i].taches[j].horaireDateDepart.heure,&flotteBus[i].taches[j].horaireDateDepart.minute);
            j++;
            nbTchaches--;
        }
    }
    *nombreBus=i+1;
    fclose(fptr);
}

void afficherZones(Zone zones[], int nombreZones) {
    for (int i = 0; i < nombreZones; i++)
    {
        printf("%d %d %d\n", zones[i].id, zones[i].row, zones[i].column);
    }
}
void afficherLignes(Ligne lignes[], int nombreLignes)
{
    for (int i = 0; i < nombreLignes; i++)
    {
        printf("%d %d\n", lignes[i].id, lignes[i].nombreZones);
        for(int j=0;j<lignes[i].nombreZones;j++)
        {
            printf("%d\n",lignes[i].idZones[j]);
        }
    }
}
void afficherTroncons(Troncon troncons[], int nombreTroncons)
{
    int i,j;
    for( i=0; i<nombreTroncons; i++){
        printf("%d %d %d %d %d %d %d\n", troncons[i].idZoneSrc, troncons[i].idZoneDest, troncons[i].distance,troncons[i].dureeAproximative.jour, troncons[i].dureeAproximative.heure, troncons[i].dureeAproximative.minute, troncons[i].nombresubdivisions);
        for ( j = 0; j < troncons[i].nombresubdivisions; j++) {
            printf("%d %d %d\n", troncons[i].parcours[j].row, troncons[i].parcours[j].column, troncons[i].parcours[j].codeRue);
        }
    }
}
void afficherFlotteBus(Bus flotteBus[], int nombreBus)
{
    int i,j;
     for( i=0; i<nombreBus; i++)
     {
         printf("%d %d %d %d %d %d %d %d\n",flotteBus[i].id,flotteBus[i].idZoneCourante,flotteBus[i].row,flotteBus[i].column,flotteBus[i].enRoute,flotteBus[i].idLigneCourante,flotteBus[i].sensCourant,flotteBus[i].nombreTaches);
        for(j=0;j<flotteBus[i].nombreTaches;j++)
            printf("%d %d %d %d %d %d\n",flotteBus[i].taches[j].id,flotteBus[i].taches[j].idLigne,flotteBus[i].taches[j].sens,flotteBus[i].taches[j].horaireDateDepart.jour,flotteBus[i].taches[j].horaireDateDepart.heure,flotteBus[i].taches[j].horaireDateDepart.minute);
     }
}

void sauvegarderZones(Zone zones[], int nombreZones, char* zonesSauv)
{
    int i;
    FILE *fptr = fopen("zonesSauv.txt", "w");
    if (fptr == NULL)
        exit(1);
    for( i=0;i<nombreZones;i++)
    {
        fprintf(fptr,"%d %d %d\n",zones[i].id, zones[i].row, zones[i].column);
    }
    fclose(fptr);
}
void sauvegarderLignes(Ligne lignes[],int nombreLignes, char* lignesSauv)
{
    int i,j;
    FILE *fptr = fopen("lignesSauv.txt", "w");
    if (fptr == NULL)
        exit(1);
    for ( i = 0; i < nombreLignes; i++)
    {
        fprintf(fptr,"%d %d\n",lignes[i].id,lignes[i].nombreZones);
        for( j=0;j<lignes[i].nombreZones;j++)
        {
            fprintf(fptr,"%d\n",lignes[i].idZones[j]);
        }
    }
    fclose(fptr);
}
void sauvegarderTroncons(Troncon troncons[], int nombreTroncons, char* tronconsSauv)
{
    int i,j;
    FILE *fptr = fopen("tronconsSauv.txt", "w");
    if (fptr == NULL)
        exit(1);
    for( i=0; i<nombreTroncons; i++){
        fprintf(fptr,"%d %d %d %d %d %d %d\n", troncons[i].idZoneSrc, troncons[i].idZoneDest, troncons[i].distance,troncons[i].dureeAproximative.jour, troncons[i].dureeAproximative.heure, troncons[i].dureeAproximative.minute, troncons[i].nombresubdivisions);
        for (j = 0; j < troncons[i].nombresubdivisions; j++) {
            fprintf(fptr,"%d %d %d\n", troncons[i].parcours[j].row, troncons[i].parcours[j].column, troncons[i].parcours[j].codeRue);
        }
    }
    fclose(fptr);
}
void sauvegarderFlotteBus(Bus flotteBus[], int nombreBus, char* flotteBusSauv)
{
    int i,j;
    FILE *fptr = fopen("flotteBusSauv.txt", "w");
    if (fptr == NULL)
        exit(1);
     for(i=0; i<nombreBus; i++)
     {
         fprintf(fptr,"%d %d %d %d %d %d %d %d\n",flotteBus[i].id,flotteBus[i].idZoneCourante,flotteBus[i].row,flotteBus[i].column,flotteBus[i].enRoute,flotteBus[i].idLigneCourante,flotteBus[i].sensCourant,flotteBus[i].nombreTaches);
        for(j=0;j<flotteBus[i].nombreTaches;j++)
            fprintf(fptr,"%d %d %d %d %d %d\n",flotteBus[i].taches[j].id,flotteBus[i].taches[j].idLigne,flotteBus[i].taches[j].sens,flotteBus[i].taches[j].horaireDateDepart.jour,flotteBus[i].taches[j].horaireDateDepart.heure,flotteBus[i].taches[j].horaireDateDepart.minute);
     }
     fclose(fptr);
}

int rechercherLigneSens(int idZoneSrc, int idZoneDest, Ligne lignes[] , int *nombreLignes, int *idLigne){
    int i,j,p1,p2;
    for(i=0;i<*nombreLignes;i++){
        for(j=0;j<lignes[i].nombreZones;j++){
             if (lignes[i].idZones[j] == idZoneSrc)
                p1=j;
            else if(lignes[i].idZones[j]==idZoneDest)
                p2=j;
        }
    }
    if(p1>p2)
        return 1;
    else
        return 0;
}

void affecterPersonneZone(Zone zones[10], Personne pers){
    int j;
    j=zones[pers.idZoneHabitat].nombrePersonnes;
    zones[pers.idZoneHabitat].personnes[j]=pers;
    zones[pers.idZoneHabitat].nombrePersonnes++;
}

void genererPersonne(Zone zones[], int nombreZones, Ligne lignes[], int nombreLignes){
        srand(time(0));
        Personne pers;
        int i;
        for (i=0;i<300;i++){
        pers.cin=i+1;
        pers.idZoneHabitat=rand()%10;
        do{
            pers.idZoneTravail=rand()%10;
        }while (pers.idZoneTravail ==  pers.idZoneHabitat );
        pers.idZoneCourante=pers.idZoneHabitat;
        pers.intentionVoyage=0;
        pers.idLigneCourante=-1;
        pers.sensCourant=-1;
        pers.idZoneDest=-1;
        pers.nombreActivites=3;
        //act0
        pers.activites[0].id=0;
        pers.activites[0].horaireDateSouhaite.jour=0;
        pers.activites[0].horaireDateSouhaite.heure=6 + rand() % 3;
        pers.activites[0].horaireDateSouhaite.minute=0;
        pers.activites[0].dureeHeureMinute.jour=0;
        pers.activites[0].dureeHeureMinute.heure=8;
        pers.activites[0].dureeHeureMinute.minute=0;
        pers.activites[0].typeActivite=1;
        pers.activites[0].idZoneSrc=pers.idZoneHabitat;
        pers.activites[0].idZoneDest=pers.idZoneTravail;
        int r;
        r=rechercherLigneSens(pers.activites[0].idZoneSrc,pers.activites[0].idZoneDest,lignes,&nombreLignes,&pers.activites[0].idLigne);
        pers.activites[0].idLigne=0;
        pers.activites[0].sens=r;
        //act1
        pers.activites[1].id=1;
        pers.activites[1].horaireDateSouhaite.jour=0;
        pers.activites[1].horaireDateSouhaite.heure=18 + rand() % 3;
        pers.activites[1].horaireDateSouhaite.minute=0;
        pers.activites[1].dureeHeureMinute.jour=0;
        pers.activites[1].dureeHeureMinute.heure=2;
        pers.activites[1].dureeHeureMinute.minute=0;
        pers.activites[1].typeActivite=2;
        pers.activites[1].idZoneSrc=pers.idZoneTravail;
        do{
        pers.activites[1].idZoneDest=rand()%10;
        }while((pers.activites[1].idZoneDest == pers.idZoneHabitat) || (pers.activites[1].idZoneDest == pers.idZoneTravail));
        int r1;
        r1=rechercherLigneSens(pers.activites[1].idZoneSrc,pers.activites[1].idZoneDest,lignes,&nombreLignes,&pers.activites[1].idLigne);
        pers.activites[1].idLigne=0;
        pers.activites[1].sens=r1;
        //act2
        pers.activites[2].id=2;
        pers.activites[2].horaireDateSouhaite.jour=0;
        pers.activites[2].horaireDateSouhaite.heure=20 + rand() % 3;
        pers.activites[2].horaireDateSouhaite.minute=0;
        pers.activites[2].dureeHeureMinute.jour=0;
        pers.activites[2].dureeHeureMinute.heure=2;
        pers.activites[2].dureeHeureMinute.minute=0;
        pers.activites[2].typeActivite=3;
        pers.activites[2].idZoneSrc=pers.activites[1].idZoneDest;
        pers.activites[2].idZoneDest=pers.idZoneHabitat;
        int r2;
        r2=rechercherLigneSens(pers.activites[2].idZoneSrc,pers.activites[2].idZoneDest,lignes,&nombreLignes,&pers.activites[2].idLigne);
        pers.activites[2].idLigne=0;
        pers.activites[2].sens=r2;
        //affectation pers
        affecterPersonneZone(zones,pers);
    }
}

void afficherPersonnes(Zone zones[], int nombreZones){
     int i=0,j,h;

           while(i<nombreZones)
            {
                printf("zone %d :\n",i);
                for(j=0;j< zones[i].nombrePersonnes;j++){
                    printf("%d %d %d %d %d %d %d %d %d\n",zones[i].personnes[j].cin,zones[i].personnes[j].idZoneHabitat,zones[i].personnes[j].idZoneTravail,zones[i].personnes[j].idZoneCourante,zones[i].personnes[j].intentionVoyage,zones[i].personnes[j].idLigneCourante,zones[i].personnes[j].sensCourant,zones[i].personnes[j].idZoneCourante,zones[i].personnes[j].nombreActivites);

                    for(h=0;h<zones[i].personnes[j].nombreActivites;h++)
                    {

                   printf("%d %d %d %d %d %d %d %d %d %d %d %d\n",zones[i].personnes[j].activites[h].id,zones[i].personnes[j].activites[h].horaireDateSouhaite.jour,zones[i].personnes[j].activites[h].horaireDateSouhaite.heure,zones[i].personnes[j].activites[h].horaireDateSouhaite.minute,zones[i].personnes[j].activites[h].dureeHeureMinute.jour,zones[i].personnes[j].activites[h].dureeHeureMinute.heure,zones[i].personnes[j].activites[h].dureeHeureMinute.minute,zones[i].personnes[j].activites[h].typeActivite,zones[i].personnes[j].activites[h].idZoneSrc,zones[i].personnes[j].activites[h].idZoneDest,zones[i].personnes[j].activites[h].idLigne,zones[i].personnes[j].activites[h].sens);
                    } }
            i++;
            }

}
void sauvegarderPersonnes(Zone zones[], int nombreZones, char* personnes_sauv){
    int i=0,j,h;
    FILE *fptr = fopen("personnes_sauv.txt", "w");
    if (fptr == NULL)
        exit(1);

          while(i<nombreZones)
            {

                for(j=0 ; j < zones[i].nombrePersonnes ; j++){
                    fprintf(fptr,"%d %d %d %d %d %d %d %d %d\n",zones[i].personnes[j].cin,zones[i].personnes[j].idZoneHabitat,zones[i].personnes[j].idZoneTravail,zones[i].personnes[j].idZoneCourante,zones[i].personnes[j].intentionVoyage,zones[i].personnes[j].idLigneCourante,zones[i].personnes[j].sensCourant,zones[i].personnes[j].idZoneCourante,zones[i].personnes[j].nombreActivites);

                    for(h=0 ; h < zones[i].personnes[j].nombreActivites ; h++)
                    {

                   fprintf(fptr,"%d %d %d %d %d %d %d %d %d %d %d %d\n",zones[i].personnes[j].activites[h].id,zones[i].personnes[j].activites[h].horaireDateSouhaite.jour,zones[i].personnes[j].activites[h].horaireDateSouhaite.heure,zones[i].personnes[j].activites[h].horaireDateSouhaite.minute,zones[i].personnes[j].activites[h].dureeHeureMinute.jour,zones[i].personnes[j].activites[h].dureeHeureMinute.heure,zones[i].personnes[j].activites[h].dureeHeureMinute.minute,zones[i].personnes[j].activites[h].typeActivite,zones[i].personnes[j].activites[h].idZoneSrc,zones[i].personnes[j].activites[h].idZoneDest,zones[i].personnes[j].activites[h].idLigne,zones[i].personnes[j].activites[h].sens);
                    } }
                    i++;

            }

    fclose(fptr);
}
void chargerPersonnes(Zone zones[], int* nombreZones, char* personnes_sauv){
     FILE *fptr =fopen("personnes_sauv.txt", "r");
     int nbactivites=0;
     int j;
     Personne pers;
     if (fptr == NULL)
        exit(1);
         while (!feof(fptr)){


            if(nbactivites==0 )
            {

                    fscanf(fptr,"%d %d %d %d %d %d %d %d %d\n",&pers.cin,&pers.idZoneHabitat,&pers.idZoneTravail,&pers.idZoneCourante,&pers.intentionVoyage,&pers.idLigneCourante,&pers.sensCourant, &pers.idZoneCourante, &pers.nombreActivites);
                    nbactivites=pers.nombreActivites;
                    j=0;}
            else{

                    fscanf(fptr,"%d %d %d %d %d %d %d %d %d %d %d %d\n",&pers.activites[j].id,&pers.activites[j].horaireDateSouhaite.jour,&pers.activites[j].horaireDateSouhaite.heure,&pers.activites[j].horaireDateSouhaite.minute,&pers.activites[j].dureeHeureMinute.jour,&pers.activites[j].dureeHeureMinute.heure,&pers.activites[j].dureeHeureMinute.minute,&pers.activites[j].typeActivite,&pers.activites[j].idZoneSrc,&pers.activites[j].idZoneDest, &pers.activites[j].idLigne, &pers.activites[j].sens);
                    j++;
                    nbactivites--;
}

        }
    fclose(fptr);
    }



int calculerDistance(int idZoneSrc, int idZoneDest, Ligne lignes[] , int idLigne, Troncon troncons[] , int nombreLignes, int nombreTroncons){
    int distance=0 ;
    int arriver,depart;
    for (int i=0;i<nombreLignes;i++){
         for (int j=0;j<lignes[i].nombreZones;j++){
            if(lignes[i].idZones[j]==idZoneSrc){
                depart=j;
            }
            if(lignes[i].idZones[j]==idZoneDest){
                arriver=j;
            }
    }
    }
    int i;
    if(depart>arriver){
        i=arriver;
        while(i<depart){
            distance=distance+troncons[i].distance;
            i++;
        }
    }
    else{
        i=depart;
        while(i<arriver){
            distance=distance+troncons[i].distance;
            i++;
    }}
    return distance;
}

void initialiserCarte(Casecarte cartegeo[20][20], Zone zones[], Ligne lignes[], Troncon troncons[], Bus flotteBus[], int nombreZones, int nombreLignes, int nombreTroncons, int nombreBus)
{ int i,j,r,c;
//intialisation
    for( i=0;i<20;i++){
        for  (j=0;j<20;j++){

            cartegeo[i][j].rue=0;
            cartegeo[i][j].zone=-1;
            cartegeo[i][j].bus=-1;
        }
    }
    //mise a jour
         for( i=0;i<nombreZones;i++){
            r=zones[i].row;
            c=zones[i].column;
            cartegeo[r][c].zone=zones[i].id;
         }
         for( i=0;i<nombreBus;i++){
            r=flotteBus[i].row;
            c=flotteBus[i].column;

            if(cartegeo[r][c].bus==-1){
                cartegeo[r][c].bus=flotteBus[i].id;
          }
          else{
             cartegeo[r][c].bus=100;
          }}
        for( i=0;i<nombreTroncons;i++){
                for (j=0;j<troncons[i].nombresubdivisions;j++){
                    r=troncons[i].parcours[j].row;
                    c=troncons[i].parcours[j].column;
                    cartegeo[r][c].rue=troncons[i].parcours[j].codeRue; }
        }

}

void remplissageComplet(Zone zones[], Ligne lignes[], Troncon troncons[], Bus flotteBus[], int *nombreZones, int *nombreLignes, int *nombreTroncons, int *nombreBus)
{
    //charger + genererpersonne
     // Chargement
    chargerZones(zones,&(*nombreZones));
    chargerLignes(lignes,&(*nombreLignes));
    chargerTroncons(troncons,&(*nombreTroncons));
    chargerFlotteBus(flotteBus,&(*nombreBus));
}

void afficherCarteGeo(Casecarte carteGeographique[20][20], Zone zones[], Bus flotteBus[], int nombreZones, int nombreLignes, int nombreTroncons, int nombreBus) {
    // Affichage en haut de la carte
    printf("    ");
    for (int j=0; j<20; j++) {
        printf("  %-2d",j);
    }
    printf("\n");
    printf("    ");
    for (int j=0; j<20; j++) {
        printf("----");
    }
    printf("\n");

    // Affichage de la carte géographique
    for (int i=0; i<20; i++) {
        //Affichage en gauche du carte
        printf("%-2d| ", i);
        for (int j=0; j<20; j++) {
            if ((carteGeographique[i][j].zone != -1) && (carteGeographique[i][j].bus == -1))
                printf("%-2dZ%d", zones[carteGeographique[i][j].zone].nombrePersonnes, carteGeographique[i][j].zone);
            else if ((carteGeographique[i][j].zone != -1) && (carteGeographique[i][j].bus != -1)) {
                if (carteGeographique[i][j].bus < 100)
                    printf("Z%db%d", carteGeographique[i][j].zone, carteGeographique[i][j].bus);
                else
                    printf("Z%dbM", carteGeographique[i][j].zone);
            } else if (carteGeographique[i][j].bus != -1) {
                if (carteGeographique[i][j].bus < 100)
                   printf("%-2db%d",flotteBus[carteGeographique[i][j].bus].nombrePassagers,carteGeographique[i][j].bus);
                else
                    printf("[bM]");

            } else if (carteGeographique[i][j].rue == 3)
                printf("%c%c%c%c",205,205,205,205);
            else if (carteGeographique[i][j].rue == 12)
                printf("  %c ",186);
            else if (carteGeographique[i][j].rue == 5)
                printf("%c%c%c ",205,205,187);
            else if (carteGeographique[i][j].rue == 6)
                printf("  %c%c",201,205);
            else if (carteGeographique[i][j].rue == 9)
                printf("%c%c%c ",205,205,188);
            else if (carteGeographique[i][j].rue == 10)
                printf("  %c%c",200,205);
            else if(carteGeographique[i][j].rue==11){
                printf("%c%c%c%c",205,202,205,205);
            }
            else if(carteGeographique[i][j].rue==13){
                printf("%c%c%c ",205,205,185);
            }
            else if (carteGeographique[i][j].rue == 15)
                printf("%c%c%c%c",205,205,206,205);
            else
                printf("    ");
        }

        //Affichage en droite de la carte
        printf("|");
        printf("\n");
    }

    //Affichae en bas de la carte
    printf("    ");
    for(int i=0;i<20;i++)
        printf("----");
    printf("\n\n");
}
void affichageComplet(Zone zones[], Ligne lignes[], Troncon troncons[], Bus flotteBus[], int nombreZones, int nombreLignes, int nombreTroncons, int nombreBus)
{
//tout les affichage
        printf("\nZones:\n");
        afficherZones(zones,nombreZones);
        printf("\nLignes:\n");
        afficherLignes(lignes,nombreLignes);
        printf("\nTroncons:\n");
        afficherTroncons(troncons,nombreTroncons);
        printf("\nFlotteBus:\n");
        afficherFlotteBus(flotteBus,nombreBus);
        printf("\nLes Personnes: \n");
        afficherPersonnes(zones,nombreZones);

}


void sauvegardeComplete(Zone zones[], Ligne lignes[], Troncon troncons[], Bus flotteBus[], int nombreZones, int nombreLignes, int nombreTroncons, int nombreBus){

         //Sauvgarde
        sauvegarderZones(zones,nombreZones,"zonesSauv.txt");
        sauvegarderLignes(lignes,nombreLignes,"lignesSauv.txt");
        sauvegarderTroncons(troncons,nombreTroncons,"tronconsSauv.txt");
        sauvegarderFlotteBus(flotteBus,nombreBus,"flotteBusSauv.txt");
        sauvegarderPersonnes(zones,nombreZones,"personnes_sauv.txt");

}

int numTacheSuivante(Horaire horaireCourant, int idBusCourant, Bus flotteBus[])
{
            int i = 0,num;
                 while(i<flotteBus[idBusCourant].nombreTaches && ((flotteBus[idBusCourant].taches[i].horaireDateDepart.jour!=horaireCourant.jour)||(flotteBus[idBusCourant].taches[i].horaireDateDepart.heure!=horaireCourant.heure)||(flotteBus[idBusCourant].taches[i].horaireDateDepart.minute!=horaireCourant.minute)))
                {
                    i++;
                }
                if(i >= flotteBus[idBusCourant].nombreTaches){
                    num = -1;
                }
                else{
                 num = flotteBus[idBusCourant].taches[i].id;
                }

                 return num;
}



void deplacerBus(Casecarte carteGeographique[20][20], int idBusCourant, Zone zones[], Ligne lignes[], Troncon troncons[], Bus flotteBus[], int nombreZones, int nombreLignes, int nombreTroncons, int nombreBus, Horaire horaireCourant)
{
    int i = 0, j = 0, dest, a = 0, r, c, r1, c1;

    // Trouver l'indice de la zone actuelle du bus dans la ligne
    while (flotteBus[idBusCourant].idZoneCourante != lignes[i].idZones[j])
    {
        j++;
    }

    // Déterminer la destination du bus sur la ligne
    if (flotteBus[idBusCourant].sensCourant == 0)
        dest = j + 1;
    else
        dest = j - 1;

    int k = 0;
    // Trouver le tronçon correspondant à la prochaine étape du bus
    if (flotteBus[idBusCourant].sensCourant == 0)
    {
        while ((k < nombreTroncons) && ((flotteBus[idBusCourant].idZoneCourante != troncons[k].idZoneSrc) || (lignes[i].idZones[dest] != troncons[k].idZoneDest)))
            k++;
    }
    else
    {
        while ((k < nombreTroncons) && ((flotteBus[idBusCourant].idZoneCourante != troncons[k].idZoneDest) || (lignes[i].idZones[dest] != troncons[k].idZoneSrc)))
            k++;
    }

    // Trouver la position actuelle du bus sur le tronçon
    while ((a < troncons[k].nombresubdivisions) && ((troncons[k].parcours[a].row != flotteBus[idBusCourant].row) || (troncons[k].parcours[a].column != flotteBus[idBusCourant].column)))
    {
        a++;
    }

    // Déplacer le bus selon le sens courant
        //Le bus se trouve dans la 1ere subdivision dans le sens 0
    if ((flotteBus[idBusCourant].sensCourant == 0) && (flotteBus[idBusCourant].row == zones[troncons[k].idZoneSrc].row) && (flotteBus[idBusCourant].column == zones[troncons[k].idZoneSrc].column)
        &&(a==troncons[k].nombresubdivisions))
    {
        r = troncons[k].parcours[0].row;
        c = troncons[k].parcours[0].column;
        carteGeographique[r][c].bus = flotteBus[idBusCourant].id;
        flotteBus[idBusCourant].row = r;
        flotteBus[idBusCourant].column = c;
        if(k==0)
        {
            int nb=0,pos;
            for(int i=0;i<nombreBus;i++)
            {
                if((flotteBus[i].row==zones[troncons[k].idZoneSrc].row)&&(flotteBus[i].column==zones[troncons[k].idZoneSrc].column))
                {
                    nb++;
                    pos=i;
                }
            }
            if(nb==0)
                carteGeographique[zones[troncons[k].idZoneSrc].row][zones[troncons[k].idZoneSrc].column].bus = -1;
            else if(nb==1)
                carteGeographique[zones[troncons[k].idZoneSrc].row][zones[troncons[k].idZoneSrc].column].bus = flotteBus[pos].id;
            else
                carteGeographique[zones[troncons[k].idZoneSrc].row][zones[troncons[k].idZoneSrc].column].bus = 100;
        }
        else
            carteGeographique[zones[troncons[k].idZoneSrc].row][zones[troncons[k].idZoneSrc].column].bus = -1;
    }
    //Le bus se trouve dans la 1ere subdivison du zone dans le sens 1
    else if ((flotteBus[idBusCourant].sensCourant == 1) && (flotteBus[idBusCourant].row == zones[troncons[k].idZoneDest].row) && (flotteBus[idBusCourant].column == zones[troncons[k].idZoneDest].column)
             &&(a==troncons[k].nombresubdivisions))
    {
        r = troncons[k].parcours[troncons[k].nombresubdivisions - 1].row;
        c = troncons[k].parcours[troncons[k].nombresubdivisions - 1].column;
        carteGeographique[r][c].bus = flotteBus[idBusCourant].id;
        flotteBus[idBusCourant].row = r;
        flotteBus[idBusCourant].column = c;
        if(k==8)
        {

           int nb1=0,pos;
            for(int i=0;i<nombreBus;i++)
            {
                if((flotteBus[i].row==zones[troncons[k].idZoneDest].row)&&(flotteBus[i].column==zones[troncons[k].idZoneDest].column))
                {
                    nb1++;
                    pos=i;
                }
            }
            if(nb1==0)
                carteGeographique[zones[troncons[k].idZoneDest].row][zones[troncons[k].idZoneDest].column].bus = -1;
            else if(nb1==1)
                carteGeographique[zones[troncons[k].idZoneDest].row][zones[troncons[k].idZoneDest].column].bus = flotteBus[pos].id;
            else
            {
                carteGeographique[zones[troncons[k].idZoneDest].row][zones[troncons[k].idZoneDest].column].bus = 100;
            }
        }
        else
            carteGeographique[zones[troncons[k].idZoneDest].row][zones[troncons[k].idZoneDest].column].bus = -1;

    }
    //Le bus se trouve dans la derniére subdivision du troncons dans le sens 0

    else if ((a == troncons[k].nombresubdivisions -1 ) && (flotteBus[idBusCourant].sensCourant == 0))
    {

        r = zones[troncons[k].idZoneDest].row;
        c = zones[troncons[k].idZoneDest].column;
        if(carteGeographique[r][c].bus==-1)
            carteGeographique[r][c].bus = flotteBus[idBusCourant].id;
        else
            carteGeographique[r][c].bus = 100;

        flotteBus[idBusCourant].row = r;
        flotteBus[idBusCourant].column = c;
        r1 = troncons[k].parcours[a].row;
        c1 = troncons[k].parcours[a].column;
        carteGeographique[r1][c1].bus = -1;

        if (troncons[k].idZoneDest == lignes[0].idZones[nombreZones - 1])
        {
            flotteBus[idBusCourant].enRoute = 0;
        }
        flotteBus[idBusCourant].idZoneCourante=troncons[k].idZoneDest;
    }
    //Le bus se trouve dans la derniére subdivision du troncons dans le sens 1
    else if ((a == 0) && (flotteBus[idBusCourant].sensCourant == 1))
    {
        r = zones[troncons[k].idZoneSrc].row;
        c = zones[troncons[k].idZoneSrc].column;
        if(carteGeographique[r][c].bus==-1)
            carteGeographique[r][c].bus = flotteBus[idBusCourant].id;
        else
            carteGeographique[r][c].bus = 100;

        flotteBus[idBusCourant].row = r;
        flotteBus[idBusCourant].column = c;
        r1 = troncons[k].parcours[a].row;
        c1 = troncons[k].parcours[a].column;
        carteGeographique[r1][c1].bus = -1;
        if (troncons[k].idZoneSrc == lignes[0].idZones[0])
        {
            flotteBus[idBusCourant].enRoute = 0;
        }
        flotteBus[idBusCourant].idZoneCourante=troncons[k].idZoneSrc;
    }
    else
    {
        if (flotteBus[idBusCourant].sensCourant == 0)
        {
            r = troncons[k].parcours[a + 1].row;
            c = troncons[k].parcours[a + 1].column;
            if(carteGeographique[r][c].bus ==-1)
                carteGeographique[r][c].bus = flotteBus[idBusCourant].id;
            else
                carteGeographique[r][c].bus=100;

        }
        else if(flotteBus[idBusCourant].sensCourant == 1)
        {
            r = troncons[k].parcours[a - 1].row;
            c = troncons[k].parcours[a - 1].column;
            if(carteGeographique[r][c].bus ==-1)
                carteGeographique[r][c].bus = flotteBus[idBusCourant].id;
            else
                carteGeographique[r][c].bus=100;
        }
        flotteBus[idBusCourant].row = r;
        flotteBus[idBusCourant].column = c;
        r1 = troncons[k].parcours[a].row;
        c1 = troncons[k].parcours[a].column;
        carteGeographique[r1][c1].bus = -1;
    }
}
//ajouter nouveau
int numActiviteSuivante(Horaire horaireCourant,int idPersonne,int idZone,Zone zones[])
{
    int i=0,num,nbAct,z;
     z=zones[idZone].personnes[idPersonne].idZoneCourante;
     nbAct=zones[z].personnes[idPersonne].nombreActivites;
     while(i<nbAct)
    {
        if(zones[z].personnes[idPersonne].activites[i].horaireDateSouhaite.jour!=horaireCourant.jour || zones[z].personnes[idPersonne].activites[i].horaireDateSouhaite.heure!=horaireCourant.heure
           ||zones[z].personnes[idPersonne].activites[i].horaireDateSouhaite.minute!=horaireCourant.minute)
            i++;
        else
            break;
    }
    if(i>=zones[z].personnes[idPersonne].nombreActivites)
        num=-1;
    else
       num=zones[z].personnes[idPersonne].activites[i].id;
    return num;
}

void deplacerPersonne(Personne src[], Personne dest[], int indicePersonneSrc, int* nombrePersonnesSrc, int* nombrePersonnesDest){
    //copie de personne dans dest
    dest[*nombrePersonnesDest] = src[indicePersonneSrc];
    (*nombrePersonnesDest)++;
    //suppression de personne de src
    for (int i = indicePersonneSrc; i <*nombrePersonnesSrc; i++) {
        src[i] = src[i+1];
    }
        (*nombrePersonnesSrc)--;
}

void majPersonnesUneZone(Horaire horaireCourant, Zone zones[], int idZone, Ligne lignes[] , int nombreLignes , Troncon troncons[], int nombreTroncons){
            for(int i=0;i<zones[idZone].nombrePersonnes;i++)
            {
                for (int j=0;j< zones[idZone].personnes[i].nombreActivites ; j++)
                {
                    if(zones[idZone].personnes[i].activites[j].horaireDateSouhaite.jour==horaireCourant.jour && zones[idZone].personnes[i].activites[j].horaireDateSouhaite.heure==horaireCourant.heure && zones[idZone].personnes[i].activites[j].horaireDateSouhaite.minute==horaireCourant.minute){
                        zones[idZone].personnes[i].intentionVoyage=1;
                        zones[idZone].personnes[i].idLigneCourante=zones[idZone].personnes[i].activites[j].idLigne;
                        zones[idZone].personnes[i].sensCourant=zones[idZone].personnes[i].activites[j].sens;
                        zones[idZone].personnes[i].idZoneDest=zones[idZone].personnes[i].activites[j].idZoneDest;
                        zones[idZone].personnes[i].idZoneCourante=zones[idZone].personnes[i].activites[j].idZoneSrc;
                   }
                }
            }

}

void majPersonnesToutesZones(Horaire horaireCourant,Zone zones[], int nombreZones, Ligne lignes[] , int nombreLignes , Troncon troncons[], int nombreTroncons){
   for(int x=0;x<nombreZones;x++){
            majPersonnesUneZone(horaireCourant,zones,x,lignes,nombreLignes,troncons,nombreTroncons);
        }
}

int calculerPrixTicket(int distance){
    int px=0;
    if(distance>1 &&  distance<=30 ){
        px=300;
    }
    else if( distance>30 && distance<=60 ){
        px=600;
    }
    else if(distance>60 && distance<=90 ){
        px=900;
    }
    else{
        px=1000;
    }
    return px;
}

Ticket venteTicket(Horaire horaireVente,int idZoneSrc, int idZoneDest, int idLigne, int sens,int idBus,Ligne lignes[] , Troncon troncons[] , int nombreLignes, int nombreTroncons){
    Ticket ticket;
    ticket.zoneSrc = idZoneSrc;
    ticket.zoneDest = idZoneDest;
    ticket.sens = sens;
    ticket.idLigne = idLigne;
    ticket.dateVente=horaireVente;
    ticket.dateVente.jour = horaireVente.jour;
    ticket.dateVente.heure = horaireVente.heure;
    ticket.dateVente.minute = horaireVente.minute;
    ticket.idBus = idBus;
    ticket.distance =calculerDistance(idZoneSrc,idZoneDest,lignes,idLigne,troncons,nombreLignes,nombreTroncons) ;
    ticket.prix = calculerPrixTicket(ticket.distance);
    return ticket;
}

int calculerGainNet(Bus flotteBus[], int nombreBus, Ligne lignes[], int nombreLignes){
    //prix des tickets totale vendue
     int gain=0;
    for(int i=0;i<nombreBus;i++)
    {
        for(int j=0;j<flotteBus[i].nombreTickets;j++)
            gain= gain + flotteBus[i].tickets[j].prix;
    }
    return gain;
}


void afficherStatistiques(Bus flotteBus[], int nombreBus, Ligne lignes[], int nombreLignes){
    //affiche un tab pour chaque bus le montant totale ==> gain de chaque bus

     for(int i=0;i<nombreBus;i++)
    {
        int gain=0;
        for(int j=0;j<flotteBus[i].nombreTickets;j++)
        {
            gain = gain + flotteBus[i].tickets[j].prix;

        }
        printf("Le gain total du bus %d est : %d\n",i,gain);
    }
         int x=calculerGainNet(flotteBus,nombreBus,lignes,nombreLignes);
         printf("Le Gain Net est: %d\n",x);

}
void sauvegarderRapport(char* RapportSauv, Zone zones[], Ligne lignes[], Troncon troncons[], Bus flotteBus[], int nombreZones, int nombreLignes, int nombreTroncons, int nombreBus){
    FILE *fptr = fopen("RapportSauv.txt", "w");
    if (fptr == NULL)
        exit(1);
     for(int i=0;i<nombreBus;i++)
      {
          int gain=0;
          for(int j=0;j<flotteBus[i].nombreTickets;j++)
          {
              gain= gain + flotteBus[i].tickets[j].prix;
          }
          fprintf(fptr,"%d\n",gain);
      }

     fclose(fptr);
}

void embarquerPersonnes(Zone zones[], Bus flotteBus[] , int idZone, int idBus, Horaire horaireCourant,Ligne lignes[],Troncon troncons[],int nombreLignes,int nombreTroncons){
     for( int i=0;i<zones[idZone].nombrePersonnes;i++)
        {
            if(zones[idZone].personnes[i].intentionVoyage==1 && zones[idZone].personnes[i].sensCourant==flotteBus[idBus].sensCourant &&
               zones[idZone].personnes[i].idLigneCourante==flotteBus[idBus].idLigneCourante && flotteBus[idBus].nombrePassagers < 50)
            {
                flotteBus[idBus].tickets[flotteBus[idBus].nombreTickets]=venteTicket(horaireCourant,idZone,zones[idZone].personnes[i].idZoneDest,flotteBus[idBus].idLigneCourante,flotteBus[idBus].sensCourant,idBus,lignes,troncons,nombreLignes,nombreTroncons);
                deplacerPersonne(zones[idZone].personnes,flotteBus[idBus].passagers,i,&zones[idZone].nombrePersonnes,&flotteBus[idBus].nombrePassagers);
                flotteBus[idBus].nombreTickets++;
            }
        }
}

void debarquerPersonnes(Zone zones[], Bus flotteBus[] , int idZone, int idBus,Horaire horaireCourant,Ligne lignes[],int nombreLignes,Troncon troncons[],int nombreTroncons){
    for(int i=0;i<flotteBus[idBus].nombrePassagers;i++){
         if(flotteBus[idBus].passagers[i].idZoneDest==idZone){
                deplacerPersonne(flotteBus[idBus].passagers,zones[idZone].personnes,i,&flotteBus[idBus].nombrePassagers,&zones[idZone].nombrePersonnes);
                flotteBus[idBus].passagers[i].intentionVoyage=0;}}
        majPersonnesUneZone(horaireCourant,zones,idZone,lignes,nombreLignes,troncons,nombreTroncons);
}

/*void deplacertEtReinitialiserTicketsBus(Bus flotteBus[],int nombreBus, Ticket tickets[],int *nombreTickets){
    //deplacement des ticket du bus vers le tab ticket

    for (int i=0; i<nombreBus ; i++){
        for(int j=0;j<flotteBus[i].nombreTickets;j++){
            tickets[*nombreTickets]=flotteBus[i].tickets[j];
            (*nombreTickets)++;
        }
    }
    printf("le nombre de tickets est %d\n",*nombreTickets);
    //reinitialiser les ticket des bus

    for (int i=0; i<nombreBus ; i++){
        for(int j=0;j<flotteBus[i].nombreTickets;j++){
            flotteBus[i].tickets[j].sens=-1;
            flotteBus[i].tickets[j].zoneDest=-1;
            flotteBus[i].tickets[j].zoneSrc=-1;
            flotteBus[i].tickets[j].dateVente.jour=-1;
            flotteBus[i].tickets[j].dateVente.heure=-1;
            flotteBus[i].tickets[j].dateVente.minute=-1;
            flotteBus[i].tickets[j].distance=-1;
            flotteBus[i].tickets[j].idBus=-1;
            flotteBus[i].tickets[j].idLigne=-1;
            flotteBus[i].tickets[j].prix=-1;

        }
        //nbticket =0
        flotteBus[i].nombreTickets=0;
    }
}*/

void deplacertEtReinitialiserTicketsBus(Bus flotteBus[],int nombreBus, Ticket tickets[],int *nombreTickets)
{
    //Deplacement des tickets vers le tableau Ticket
    for(int i=0;i<nombreBus;i++)
    {
        for(int j=0;j<flotteBus[i].nombreTickets;j++)
        {
            tickets[*nombreTickets]=flotteBus[i].tickets[j];
            (*nombreTickets)++;
        }
    }
    //Reinisialisation des tableaux Tickets des bus
     for(int i=0;i<nombreBus;i++)
     {
         for(int j=0;j<flotteBus[i].nombreTickets;j++)
         {
             flotteBus[i].tickets[j].dateVente.jour=-1;
             flotteBus[i].tickets[j].dateVente.heure=-1;
             flotteBus[i].tickets[j].dateVente.minute=-1;
             flotteBus[i].tickets[j].distance=-1;
             flotteBus[i].tickets[j].idBus=-1;
             flotteBus[i].tickets[j].idLigne=-1;
             flotteBus[i].tickets[j].prix=-1;
             flotteBus[i].tickets[j].sens=-1;
             flotteBus[i].tickets[j].zoneDest=-1;
             flotteBus[i].tickets[j].zoneSrc=-1;
         }
         flotteBus[i].nombreTickets=0;
     }
}

void deroulerSemaine(Casecarte cartegeo[20][20], Ticket tickets[], Zone zones[], Ligne lignes[], Troncon
troncons[], Bus flotteBus[], int nombreZones, int nombreLignes, int nombreTroncons, int nombreBus,int*nombreTickets)
{
        Horaire now;
        int vitesse,count=0;
        do{
        printf("\n**choix de vitesse d'avencement**\n");
        printf("1:1 minute  ");
        printf("2:10 minute ");
        printf("3:2 heure   ");
        printf("4:1 jour    ");
        scanf("%d",&vitesse);

        }while(vitesse<1 || vitesse>4);
    initialiserCarte(cartegeo,zones,lignes,troncons,flotteBus,nombreZones,nombreLignes,nombreTroncons,nombreBus);
    int h,m,j;
    for (j=0;j<6;j++){
    for (h=6;h<19;h++){
        for (m=0;m<=59;m++){
            printf("\nnow : (j%d) %dH:%dM\n",j,h,m);
            now.jour=j;
            now.heure=h;
            now.minute=m;
            majPersonnesToutesZones(now,zones,nombreZones,lignes,nombreLignes,troncons,nombreTroncons);
            afficherCarteGeo(cartegeo,zones,flotteBus,nombreZones,nombreLignes,nombreTroncons,nombreBus);
           for (int i=0 ; i<nombreBus ; i++){
               if(i==0){
                printf("la zone courante est %d et le sens est %d\n",flotteBus[i].idZoneCourante,flotteBus[i].sensCourant);
               }
                if (flotteBus[i].enRoute == 0){
                    int k = numTacheSuivante(now,flotteBus[i].id,flotteBus);
                    if (k > -1){
                        flotteBus[i].enRoute=1;
                        flotteBus[i].idLigneCourante=flotteBus[i].taches[k].idLigne;
                        flotteBus[i].sensCourant=flotteBus[i].taches[k].sens;
                    }
                }
                if((flotteBus[i].row == zones[flotteBus[i].idZoneCourante].row) && (flotteBus[i].column == zones[flotteBus[i].idZoneCourante].column))
                {
                    debarquerPersonnes(zones,flotteBus ,zones[flotteBus[i].idZoneCourante].id,i,now,lignes,nombreLignes,troncons,nombreTroncons);
                    embarquerPersonnes(zones,flotteBus,zones[flotteBus[i].idZoneCourante].id,i,now,lignes,troncons,nombreLignes,nombreTroncons);
                }
                  if(flotteBus[i].enRoute==1)
                  {
                      deplacerBus(cartegeo,flotteBus[i].id,zones,lignes,troncons,flotteBus,nombreZones,nombreLignes,nombreTroncons,nombreBus,now);
                      sauvegardeComplete(zones,lignes,troncons,flotteBus,nombreZones,nombreLignes,nombreTroncons,nombreBus);
                  }
            }

             if(count==0)
                 {
                     do
                     {
                         printf("\ndonner la vitesse souhaiter: ");
                         scanf("%d",&vitesse);
                     }
                     while(vitesse<1 || vitesse>5);
                 }
                 count++;
                 if(vitesse==1)
                 {
                    count=0;
                 }
                 else if(vitesse==2)
                 {
                     if(count==10)
                     {
                        count=0;
                     }
                 }
                 else if(vitesse==3)
                 {
                     if(count==120)
                     {
                        count=0;
                     }
                 }
                 else if(vitesse==4)
                 {
                     if(count==780)
                     {
                        count=0;
                     }
                 }
///////////////////////////////////////////
//a la fin de la journee : le tab des ticket
            if(h==18 && m==59){
                    for(int i=0;i<nombreBus;i++){
                        printf("le nombre des tickets vendu est = %d pour la bus %d\n",flotteBus[i].nombreTickets,i);
                    }
                    afficherStatistiques(flotteBus,nombreBus,lignes,nombreLignes);
                    sauvegarderRapport("RapportSauv.txt",zones,lignes,troncons,flotteBus,nombreZones,nombreLignes,nombreTroncons,nombreBus);
                    //deplacertEtReinitialiserTicketsBus(flotteBus,nombreBus,tickets,nombreTickets);
            }

        }
     }

    }
}
int main()
{
    /*Zone zones[10];
    Ligne lignes[1];
    Troncon troncons[9];
    Bus flotteBus[10];
    Ticket tickets[10000];*/

    Zone *zones=malloc(sizeof(Zone)*10);
    Ligne *lignes=malloc(sizeof(Ligne)*1);
    Troncon *troncons=malloc(sizeof(Troncon)*9);
    Bus *flotteBus=malloc(sizeof(Bus)*10);
    Ticket *tickets=malloc(sizeof(Ticket)*10000);
    Casecarte carteGeographique[20][20];
    int choixdonnee,nombreZones,nombreLignes,nombreTroncons,nombreBus,nombreTickets,choix;
    printf(" ----------Le Menu Principale du jeu----------\n");
    printf("|              0-nouveau jeu                  |\n");
    printf("|              1-charger jeu                  |\n");
    printf(" ---------------------------------------------\n");
    printf("Entrer votre choix:");
    scanf("%d",&choix);
        while((choix<0)||(choix>1))
    {
        printf("donner votre choix :");
        scanf("%d",&choix);
    }
    //Nouveau jeu
    if(choix==0){
    remplissageComplet(zones,lignes,troncons,flotteBus,&nombreZones,&nombreLignes,&nombreTroncons,&nombreBus);
    genererPersonne(zones,nombreZones,lignes,nombreLignes);
    }
    //Charger jeu
    else{
        remplissageComplet(zones,lignes,troncons,flotteBus,&nombreZones,&nombreLignes,&nombreTroncons,&nombreBus);
        chargerPersonnes(zones,&nombreZones,"personneSauve");
    }

     do{
        printf(" ----------Le Menu Secondaire du jeu---------\n");
        printf("|              1-Affichage                   |\n");
        printf("|              2-Sauvegarde                  |\n");
        printf("|              3-Derouler Semaine            |\n");
        printf("|              -Votre choix-                 |\n");
        printf(" -------------------------------------------- \n");
        scanf("%d",&choixdonnee);
    }while(choixdonnee<1 || choixdonnee>3);
    // Affichage
    if(choixdonnee==1)
    {
        affichageComplet(zones,lignes,troncons,flotteBus,nombreZones,nombreLignes,nombreTroncons,nombreBus);
    }
    //Sauvgarde
    else if(choixdonnee==2)
    {
        sauvegardeComplete(zones,lignes,troncons,flotteBus,nombreZones,nombreLignes,nombreTroncons,nombreBus);
        printf("**Sauvgarde du jeu avec succes***");
    }
    //derloulement semaine
    else{
        deroulerSemaine(carteGeographique,tickets,zones,lignes,troncons,flotteBus,nombreZones,nombreLignes,nombreTroncons,nombreBus,&nombreTickets);
    }
    return 0;
}
