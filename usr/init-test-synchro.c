/**
 * @file usr/init-test-synchro.c
 * @brief L'init du monde utilisateur permettant de montrer les
 * besoins de synchronisation 
 * 
 * Attention, ici les include sont dans usr/include !!!
 */
#include <manux/types.h>
#include <stdio.h>
#include <unistd.h>   // testSynchro

void init()
{
   int r = 1;

   printf("Sympa le mode utilisateur !\n");

   r = testSynchro(10, 10);
   
   printf("r = %d\n", r);

}
