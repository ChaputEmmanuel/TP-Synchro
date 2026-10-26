/**
 * @file : main-userland.c
 * @brief : Démonstration d'utilisation du mode utilisateur
 *
 *                                                     (C) Manu Chaput 2026-2026
 */
#include <manux/config.h>
#include <manux/console.h>
#include <manux/bootloader.h>

void init(void);

[[maybe_unused]]
static
void startManuX(void)
{
   // Initialisation de la console noyau
   consoleInitialisation();

   init();
   while(1){};   
}   /* startManuX */

