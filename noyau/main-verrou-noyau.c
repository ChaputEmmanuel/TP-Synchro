/**
 * @file : main-verrou-noyau.c
 * @brief : Mise en évidence de a (non) ré-entrance du noyau. On
 * compilera une fois avec la macro MANUX_REENTRANT activée une fois
 * avec cette macro désactivée.
 *                                                                            
 *                                                     (C) Manu Chaput 2000-2023
 **/
#undef MANUX_RAMDISK

#include <manux/config.h>
#include <manux/bootloader.h>
#include <manux/errno.h>
#include <manux/console.h>
#include <manux/clavier.h>
#include <manux/tache.h>
#include <manux/horloge.h>        // initialiserHorloge
#include <manux/scheduler.h>
#include <manux/interruptions.h>
#include <manux/io.h>
#include <manux/segment.h>
#include <manux/memoire.h>
#include <manux/atomique.h>
#include <manux/appelsysteme.h>
#include <manux/printk.h>
#include <manux/debug.h>
#include <manux/segment.h>
#include <manux/pagination.h>
#include <manux/exclusion-mutuelle.h> // Pour le verrou général

extern void init(); // Faire un init.h

/**
 * Configuration de la console
 */
INoeud  iNoeudConsole;  // Le INoeud qui décrit la console

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

   // Le clavier nous permettra de basculer entre consoles
   printk_debug(DBG_KERNEL_START, "Initialisation du clavier ...\n");
   initialiserClavier();

   // On va utiliser des appels systèmes
   printk_debug(DBG_KERNEL_START, "Initialisation des AS ...\n");
   initialiserAppelsSysteme();

   // Initialisation de la gestion des processus
   printk_debug(DBG_KERNEL_START, "Initialisation du scheduler ...\n");
   initialiserScheduler();

   // On a besoin de l'horloge pour l'ordonnanceur
   printk_debug(DBG_KERNEL_START, "Initialisation de l'horloge...\n");
   initialiserHorloge();

   // Avant de passer la main à init, on relâche le verrou global. On
   // n'a donc plus accès au noyau, il faudra passer par des appels
   // systèmes
   
#if defined(MANUX_TACHES)
#   if defined(MANUX_REENTRANT)
   printk_debug(DBG_KERNEL_START, "Noyau reentrant ...\n");
#   else
   tacheDansLeNoyau = 0;
   exclusionMutuelleSortir(&verrouGeneralDuNoyau);
   printk_debug(DBG_KERNEL_START, "Noyau NON reentrant ...\n");
#   endif
#endif
   
   // On va maintenant faire de la tâche en cours une tâche "banale"
   //tacheSetConsole(tacheEnCours, creerConsoleVirtuelle());

   printk_debug(DBG_KERNEL_START, "On passe en usr ...\n");

   init();
}   /* startManuX */


