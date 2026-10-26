/**
 * @file main-acces-concurrent.c
 * @brief Un exemple pitoyable de début de noyau.
 *
 *                                                     (C) Manu Chaput 2000-2023
 */
#include <manux/config.h>
#include <manux/errno.h>
#include <manux/debug.h>
#include <manux/console.h>
#include <manux/kmalloc.h>    // Pour l'initialisation
#include <manux/bootloader.h>
#include <manux/registre.h>
#include <manux/printk.h>
#include <manux/memoire.h>
#include <manux/journal.h>    // initialiserJournal()
#include <manux/fichier.h>
#include <manux/clavier.h>
#include <manux/pagination.h>
#include <manux/exclusion-mutuelle.h>
#include <manux/logo.h>

extern void init(void); // Faire un init.h
void startManuX(void);

void startManuX(void)
{
   // Initialisation de la console
   consoleInitialisation();
   printk(logoManuXTexte());
   printk("\n\n");
   
#if MANUX_ARCH == i386
   union {
      uint32_t registres[3];
      char     caracteres[13];
   } descriptionProc;

   // Lecture du nom du processeur
   descriptionProcesseur(0, descriptionProc.registres);
   descriptionProc.caracteres[12] = 0;

   // Affichage d'un premier message
   printk("ManuX 32 bits a votre service sur un '%s' ...\n",
		descriptionProc.caracteres);
#endif

   printk("\n\n                                              (ESC pour basculer entre consoles)\n");

   // Récupération des informations depuis le bootloader
   bootloaderInitialiser();

   // Initialisation de la gestion des pages mémoire (utilise le
   // bootloader)
   initialiserMemoire(infoSysteme.memoireDeBase,
		      infoSysteme.memoireEtendue);
   
   /* Initilisation des descripteurs de segments */
   initialiserGDT();

   /* Initialisation de la table des interruptions */
   initialiserIDT();

   // Initialisation de l'allocateur généraliste
   kmallocInitialisation();
   
   // Initialisation du registre (utilise kmalloc)
   registreSystemeInitialiser();

   // Analyse de la ligne de commande (utilise le registre)
   bootloaderLireLigneCmd();

   // Initialisation du système de debug
   debugInitialiser();

   // Le clavier nous permettra de basculer entre consoles
   initialiserClavier();

   // On va utiliser des appels systèmes
   initialiserAppelsSysteme();

   // On va utiliser des tubes, donc le système de fichiers
   sfInitialiser();

   // Initialisation de la gestion des processus
   initialiserScheduler();

   // On a besoin de l'horloge pour l'ordonnanceur
   initialiserHorloge();

   printk_debug(DBG_KERNEL_START, "On passe en usr ...\n");

   // Avant de passer la main à init, on relâche le verrou global. On
   // n'a donc plus accès au noyau, il faudra passer par des appels
   // systèmes

   printk("--------------------------------------------------------------------------------\n");
#if !defined(MANUX_REENTRANT) && defined(MANUX_EXCLUSION_MUTUELLE)
   tacheDansLeNoyau = 0;
   exclusionMutuelleSortir(&verrouGeneralDuNoyau);
#endif

   init();
}   /* _startManuX */


