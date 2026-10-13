/**
 * @file  : main-debmaskdyn.c
 * @brief : Comment utiliser les options du bootloader pour modifier
 * les masques de débogage.
 *
 *   On n'utilise pas le registre ici, ce qui permet d'alléger
 * sensiblement les choses. 
 *
 *   Après les initialisations nécessaires, quelques messages de
 * debogage sont affichés pour montrer l'impact des masques de debug.
 *
 *   On pourra par exemple passer en paramètre au bootloader
 *
 * debug.masque.console=0x803
 *
 *   ou
 *
 * debug.masque.console=0x003
 *
 *   Car 0x800 active les messages "a faire".
 *                                                  (C) Manu Chaput 2000-2025 
 */
#include <manux/config.h>
#include <manux/types.h>     // TRUE
#include <manux/console.h>   // Pour printk
#include <manux/printk.h>
#include <manux/memoire.h>
#include <manux/bootloader.h>

#include <manux/debug.h>

void startManuX()
{
   // Initialisation de la console noyau
   consoleInitialisation();

   bootloaderInitialiser();

   initialiserMemoire(infoSysteme.memoireDeBase,
		      infoSysteme.memoireEtendue);

   // Initialisation du système de debug
   debugInitialiser();
   
   // On peut maintenant analyser la ligne de commande
   bootloaderLireLigneCmd();

   // Affichage des masques, pour le plaisir
   printk("Masques de debug : 0x%x (console) et 0x%x (fichier)\n",
	  masqueDebugageConsole, masqueDebugageFichier);

   // Quelques messages de debug, pour vérifier
   printk_debug(DBG_KERNEL_START, "Message de start\n");
   printk_debug(DBG_KERNEL_BOOTLOADER, "Message de debug du bootloader\n");
   printk_debug(DBG_KERNEL_A_FAIRE, "Message a faire\n");
}   /* startManuX */


