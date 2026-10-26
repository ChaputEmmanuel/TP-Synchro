/**
 * @file usr/init-test-synchro.c
 * @brief L'init du monde utilisateur permettant de montrer les
 * besoins de synchronisation 
 * 
 * Attention, ici les include sont dans usr/include !!!
 */
#include "include/manux/types.h"
#include "include/unistd.h"
#include <manux/types.h>
#include <stdio.h>
#include <unistd.h>   // testSynchro

void travailleur(void)
{
   int c = 0;
   int n;
   
   for (n = 0 ; n < 3 ; n++) {
      c = testSynchro(1, 2);
      printf("[%d] Compteur = %d\n", identifiantTache(), c);
   }
}

void init(void)
{
  int n;
   printf("Sympa le mode utilisateur !\n");

   creerNouvelleTache(travailleur, FALSE);
   creerNouvelleTache(travailleur, FALSE);

   while(1) {printf("."); for (n=0;n<1000000;n++){};}
}
