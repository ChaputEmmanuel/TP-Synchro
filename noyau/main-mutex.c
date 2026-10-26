/**
 * @file :
 * @brief : Démonstration d'utilisation des exclusions mutuelles
 *
 *                                                     (C) Manu Chaput 2000-2023
 */
#include <manux/config.h>
#include <manux/console.h>
#include <manux/interruptions.h>
#include <manux/intel-8259a.h>
#include <manux/tache.h>
#include <manux/memoire.h>
#include <manux/bootloader.h>

void albert()
{
  while (1) {
    printk("O");
  }
}

void barnabe()
{
  while (1) {
    printk("X");
  }
}

void startManuX()
{
   Tache * t0, *t1;

   // Initialisation de la console noyau
   consoleInitialisation();

   // Initialisation des informations depuis le bootloader
   bootloaderInitialiser();

   // Affichage de la mémoire disponible 
   printk("Memoire : %d + %d Ko\n",
	  infoSysteme.memoireDeBase,
	  infoSysteme.memoireEtendue);

   /* Initialisation de la gestion mémoire */
   initialiserMemoire(infoSysteme.memoireDeBase,
		      infoSysteme.memoireEtendue);
  
   /* Initialisation de la table des interruptions */
   initialiserIDT();   

   i8259aInit(MANUX_INT_BASE_IRQ);

   initialiserScheduler();

   initialiserHorloge();

   // Affichage d'un message
   consoleNoyauAfficher("La console vous salue ...\n");
   /*
   t0 = tacheCreer(albert);
   t1 = tacheCreer(barnabe);
  while (1) {
    printk(".");
  }
   */
}   /* _startManuX */

