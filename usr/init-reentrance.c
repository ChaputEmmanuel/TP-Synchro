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

void init()
{
   printf("Bonjour le mode utilisateur !\n");

   while(1){};
}
