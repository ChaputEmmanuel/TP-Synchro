/**
 * @file  : main-bootreg.c
 * @brief : Comment peupler le registre en fonction des paramètres de
 * la ligne de commande  obtenus par le bootloader.
 *
 *                                                  (C) Manu Chaput 2024-2025 
 */
#include <manux/config.h>
#include <manux/types.h>     // TRUE
#include <manux/stdlib.h>     // atoi
#include <manux/console.h>
#include <manux/printk.h>
#include <manux/memoire.h>
#include <manux/kmalloc.h>
#include <manux/bootloader.h>
#include <manux/registre.h>

#include <manux/debug.h>

/**
 * @brief Exemple de fonction de mise-à-jour d'un paramètre dans un
 * registre 
 */
uint16_t a, b, c;
void miseAJourPararametre(void * prive, char * valeur)
{
   uint16_t * n = (uint16_t *)prive;

   printk("Nouvelle valeur = '%s' pour 0x%x\n", valeur, prive);

   // Une conversion bas de gamme
   *n = atoihex(valeur);   
}
			  
void startManuX()
{
   // On doit commencer par ça a priori
   bootloaderInitialiser();
  
   // Initialisation de la console noyau
   consoleInitialisation();

   // On peut initialiser la gestion de la mémoire
   initialiserMemoire(infoSysteme.memoireDeBase,
		      infoSysteme.memoireEtendue);

   // Initialsation du système kmalloc
   kmallocInitialisation();

   // On initialise le registre
   registreSystemeInitialiser();

   // Analyse des paramètres obtenus par le bootloader
   bootloaderLireLigneCmd();
   
   // Maintenant le sous-système test s'initialise. Il a une valeur
   // par défaut pour a, b et c, mais celles de a et b ne seront pas
   // utilisées, puisqu'une valeur a déjà été donnée au boot.
   registreSystemeAffecterParametre("23", &a, miseAJourPararametre,
	                            "test", "a", NULL);

   registreSystemeAffecterParametre("17", &b, miseAJourPararametre,
	                            "test", "b", NULL);

   registreSystemeAffecterParametre("69", &c, miseAJourPararametre,
	                            "test", "c", NULL);

   registreSystemeAfficher();

   printk("Finalement, le systeme va demarrer avec a=%d, b=%d et c=%d\n", a, b, c);

}   /* startManuX */


