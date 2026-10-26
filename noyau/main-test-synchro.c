/**
 * @file noyau/main-test-synchro.c
 * @brief Démonstration des besoins de sychronisation
 *                                                          (C) Manu Chaput 2025
 */
#include <manux/config.h>
#include <manux/memoire.h>
#include <manux/kmalloc.h>
#include <manux/exclusion-mutuelle.h>
#include <manux/console.h>
#include <manux/clavier.h>
#include <manux/scheduler.h>
#include <manux/printk.h>
#include <manux/bootloader.h>

extern void init(void);
void startManuX(void);

void startManuX(void)
{
   // Initialisation de la console
   consoleInitialisation();

   // Récupération des paramètres de boot
   bootloaderInitialiser();

   // Initialisation de la gestion des pages mémoire (utilise le
   // bootloader)
   initialiserMemoire(infoSysteme.memoireDeBase,
		      infoSysteme.memoireEtendue);

   // Initialisation des descripteurs de segments
   initialiserGDT();

   // Initialisation de la table des interruptions
   initialiserIDT();   

   // Initilisation du système kmalloc
   kmallocInitialisation();
   
   // Initialisation du clavier
   initialiserClavier();
   
   // Initialisation de la gestion des processus
   initialiserScheduler();

   initialiserAppelsSysteme();
   
   initialiserHorloge();

   // Un petit message
   printk("Printk dit bonjour !\n");

#if !defined(MANUX_REENTRANT) && defined(MANUX_EXCLUSION_MUTUELLE)
   tacheDansLeNoyau = 0;
   exclusionMutuelleSortir(&verrouGeneralDuNoyau);
#endif

   init();
}   /* startManuX */


