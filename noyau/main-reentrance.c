/**
 * @file noyau/main-reentrance.c
 * @brief Démonstration de la différence entre un noyau réentrant et
 * un noyau non réentrant
 *
 *                                                          (C) Manu Chaput 2025
 */
#include <manux/config.h>
#include <manux/errno.h>
#include <manux/debug.h>      // A virer j'espère !
#include <manux/console.h>
#include <manux/kmalloc.h>    // Pour l'initialisation
#include <manux/printk.h>
#include <manux/memoire.h>
#include <manux/bootloader.h>

extern void init(); // Faire un init.h

void startManuX()
{
   // Récupération des informations depuis le bootloader
   bootloaderInitialiser();

   initialiserMemoire(infoSysteme.memoireDeBase,
		      infoSysteme.memoireEtendue);
   
   // Initialisation de la console noyau
   consoleInitialisation();

   /* Initilisation des descripteurs de segments */
   printk_debug(DBG_KERNEL_START, "Initialisation de la GDT ...\n");
   initialiserGDT();

   /* Initialisation de la table des interruptions */
   initialiserIDT();

   // Initialisation de l'allocateur généraliste
   kmallocInitialisation();
   
   // On va utiliser des appels systèmes
   printk_debug(DBG_KERNEL_START, "Initialisation des AS ...\n");
   initialiserAppelsSysteme();

   // Initialisation de la gestion des processus
   printk_debug(DBG_KERNEL_START, "Initialisation du scheduler ...\n");
   initialiserScheduler();

   // On a besoin de l'horloge pour l'ordonnanceur
   printk_debug(DBG_KERNEL_START, "Initialisation de l'horloge...\n");
   initialiserHorloge();

   printk_debug(DBG_KERNEL_START, "On passe en usr ...\n");

   // Avant de passer la main à init, on relâche le verrou global. On
   // n'a donc plus accès au noyau, il faudra passer par des appels
   // systèmes
   
#if defined(MANUX_TACHES) && !defined(MANUX_REENTRANT)
   tacheDansLeNoyau = 0;
   sortirExclusionMutuelle(&verrouGeneralDuNoyau);
#endif

   init();
}   /* _startManuX */


