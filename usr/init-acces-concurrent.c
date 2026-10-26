/**
 * @file usr/init.c
 * @brief L'init du monde utilisateur
 * 
 * Attention, ici les include sont dans usr/include !!!
 */
#include <manux/types.h>
#include <stdio.h>
#include <manux/errno.h>
#include <unistd.h>   // creerNouvelleTache
#include <manux/string.h>

/**
 * @brief Taille du buffer utilisé en lecture
 */ 
#define TAILLE_BUFFER 64

/**
 * @brief Nombres de lecteurs et écrivains
 */ 
#define NB_LECTEURS  2
#define NB_ECRIVAINS 2

int fd[2];  // Le tube

void lecteur()
{
   int r, c=0;
   char b[TAILLE_BUFFER];
   
   printf("[%d] Je suis un lecteur !\n", identifiantTache());
   fermer(fd[1]);
   
   do {
      r = lire(fd[0], b, TAILLE_BUFFER - 1);
      if (r > 0) {
         b[r] = 0;
         printf(b);
         c += r;
      } else if (r < 0){
	printf("[%d] Erreur lecture\n", identifiantTache());
      }
   } while (r > 0);

   printf("\n[%d] Fini ... En tout, j'ai lu %d !\n", identifiantTache(), c);
}

void ecrivain()
{
   int r, c  = 0;
   
   char * b = "Bonjour les jeunes ! ";

   printf("[%d] Je suis un ecrivain !\n", identifiantTache());
   fermer(fd[0]);

   do {
      printf("[%d] Je vais ecrire %d\n", identifiantTache(), strlen(b));
      r = ecrire(fd[1], b, strlen(b));
      printf("[%d] Voila j'ai ecrit %d\n", identifiantTache(), r);
      if (r >= 0) {
         c += r;
      } else {
         printf("[%d] Erreur ecriture\n", identifiantTache());
      }
   } while ((r > 0) && (c < 4096)); // On arrête au bout de 4096 (un peu plus) 

   printf("[%d] En tout, j'ai ecrit %d !\n", identifiantTache(), c);

   fermer(fd[1]);
}

void init()
{
   int r;
   int l, e;

   printf("Bonjour le mode utilisateur !\n");

   r = tube(fd);

   if ( r != ESUCCES) {
      printf("r = %d : casse la pipe !?\n", r);
   } else {
      for (e = 0; e < NB_ECRIVAINS; e++) {
         r = creerNouvelleTache(ecrivain, FALSE);
      }
      for (l = 0; l < NB_LECTEURS; l++) {
         r = creerNouvelleTache(lecteur, FALSE);
      }
   }
   fermer(fd[0]);
   fermer(fd[1]);

   // Nous sommes dans init, et le système s'arrête lorsque cette
   // tâche se termine
   while(1){};
}

/*
 * Ci dessous les fonctions utilisées pour la vidéo
 */
void ecrivainA()
{
   int r, c  = 0;
   
   char * b = "AAAAAAAAAAAAAAAA";

   printf("[%d] Je suis l'ecrivain A.\n", identifiantTache());
   fermer(fd[0]);

   do {
      printf("[%d] Je vais ecrire %d octets\n", identifiantTache(), strlen(b));
      r = ecrire(fd[1], b, strlen(b)-c);
      printf("[%d] Voila j'ai ecrit %d octets\n", identifiantTache(), r);
      if (r >= 0) {
         c += r;
      } else {
         printf("[%d] Erreur ecriture\n", identifiantTache());
      }
   } while ((r > 0) && (c < strlen(b)));

   printf("[%d] En tout, l'ecrivain A a ecrit %d octets !\n", identifiantTache(), c);

   fermer(fd[1]);
}

void ecrivainB()
{
   int r, c  = 0;
   
   char * b = "BBBBBBBBBBBB";

   printf("[%d] Je suis l'ecrivain B.\n", identifiantTache());
   fermer(fd[0]);

   do {
      printf("[%d] Je vais ecrire %d\n", identifiantTache(), strlen(b));
      r = ecrire(fd[1], b, strlen(b)-c);
      printf("[%d] Voila j'ai ecrit %d\n", identifiantTache(), r);
      if (r >= 0) {
         c += r;
      } else {
         printf("[%d] Erreur ecriture\n", identifiantTache());
      }
   } while ((r > 0) && (c < strlen(b)));

   printf("[%d] En tout, l'ecrivain B a ecrit %d !\n", identifiantTache(), c);

   fermer(fd[1]);
}

/**
 * @brief Lancement du code en mode utilisateur
 */
void initBis()
{
   int r;

   printf("Bonjour le mode utilisateur !\n");

   r = tube(fd);

   if ( r != ESUCCES) {
      printf("r = %d : casse la pipe !?\n", r);
   } else {
      // On crée deux écrivains
      creerNouvelleTache(ecrivainA, FALSE);
      creerNouvelleTache(ecrivainB, FALSE);

      // On les laisse un peu se battre !
      attendreMillisecondes(500);

      // Voyons le résultat avec un lecteur
      creerNouvelleTache(lecteur, FALSE);
   }
   fermer(fd[0]);
   fermer(fd[1]);

   // Nous sommes dans init, et le système s'arrête lorsque cette
   // tâche se termine
   while(1){};
}

