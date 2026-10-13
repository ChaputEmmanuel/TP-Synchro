/**
 * @file  : main-debmaskdyn-reg.c
 * @brief : Comment utiliser les options du bootloader pour modifier
 * les masques de débogage via le registre.
 *
 *    Après les initialisations nécessaires, quelques messages de
 * debogage sont affichés pour montrer l'impact des masques de debug.
 *
 *    On pourra par exemple passer en paramètre au bootloader
 *
 * debug.masque.console=0x803
 *
 *    ou
 *
 * debug.masque.console=0x003
 *
 *    Car 0x800 active les messages "a faire".
 *                                                  (C) Manu Chaput 2000-2025 
 */
#include <manux/config.h>
#include <manux/types.h>     // TRUE
#include <manux/console.h>
#include <manux/printk.h>
#include <manux/memoire.h>
#include <manux/kmalloc.h>
#include <manux/bootloader.h>
#include <manux/registre.h>

#include <manux/debug.h>

void startManuX()
{
   // Initialisation de la console noyau (n'utilise rien !)
   consoleInitialisation();

   // Initialisation de infoSysteme et cmdLine
   bootloaderInitialiser();

   // Initialisation de la gestion des pages mémoire (utilise le
   // bootloader)
   initialiserMemoire(infoSysteme.memoireDeBase,
		      infoSysteme.memoireEtendue);

   // Initialsation du système kmalloc
   kmallocInitialisation();

   // Initialisation du registre (utilise kmalloc)
   registreSystemeInitialiser();

   // Initialisation du système de debug (utilise le registre)
   debugInitialiser();
   
   // Analyse de la ligne de commande (utilise le registre)
   bootloaderLireLigneCmd();

   // On affiche le registre pour information
   registreSystemeAfficher();

   // Affichage desmasque, pour le plaisir
   printk("Registre de debug : 0x%x (console) et 0x%x (fichier)\n",
	  masqueDebugageConsole, masqueDebugageFichier);

   // Quelques messages de debug, pour vérifier
   printk_debug(DBG_KERNEL_START, "Message de start\n");
   printk_debug(DBG_KERNEL_BOOTLOADER, "Message de debug du bootloader\n");
   printk_debug(DBG_KERNEL_A_FAIRE, "Message a faire\n");
}   /* startManuX */


