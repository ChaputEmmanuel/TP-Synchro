/**
 * @file usr/init.c
 * @brief L'init du monde utilisateur
 * 
 * Attention, ici les include sont dans usr/include !!!
 */
#include <manux/types.h>
#include <stdio.h>
#include <stdio.h>
#include <manux/errno.h>
#include <unistd.h>   // creerNouvelleTache
#include <manux/string.h>

void uneActiviteQuelconque()
{
   int i = 1;

   printf("[%d] va incrementer\n", identifiantTache());

   i = testSynchro(1, 2);

   printf("[%d] trouve %d\n", identifiantTache(), i);
}

void init()
{

   printf("Sympa le mode utilisateur !\n");
   
   for (int n = 0 ; n < 2; n++){
      creerNouvelleTache(uneActiviteQuelconque, FALSE);
   }
   while(1){};
}
