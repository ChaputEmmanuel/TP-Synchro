/**
 * @file usr/init-userland.c
 * @brief Un exemple trivial de init
 * 
 * Attention, ici les include sont dans usr/include !!!
 */
#include <manux/types.h>
#include <stdio.h>
#include <unistd.h>   // creerNouvelleTache

void init()
{
   printf("Sympa le mode utilisateur !\n");
}
