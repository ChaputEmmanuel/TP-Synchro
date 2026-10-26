/**
 * @file  : main-registre.c
 * @brief : 
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
uint16_t a = 1, b = 1, c = 1, d = 1;
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

   // Initialisation du système kmalloc
   kmallocInitialisation();

   // On initialise le registre
   registreSystemeInitialiser();

   // Imaginons que la valeur 19 est affectée au paramètre a lors du
   // boot (eg via le bootloader).
   registreSystemeAjouterC("test.a=0x19");

   // La valeur 0x42 est affectée au paramètre b, mais d'une autre façon
   // (même résultat) idem pour d qui reçoit 0x17
   registreSystemeAffecterParametre("0x42", NULL, NULL,
	                            "test", "b", NULL);
   registreSystemeAffecterParametre("0x17", NULL, NULL,
	                            "test", "d", NULL);

   // Maintenant le sous-système test s'initialise. Il a une valeur
   // par défaut pour a, c et d, mais celles de a et b ne seront pas
   // utilisées, puisqu'une valeur a déjà été donnée au boot.
   registreSystemeAffecterParametre("0x23", &a, miseAJourPararametre,
	                            "test", "a", NULL);

   registreSystemeAffecterParametre(NULL, &b, miseAJourPararametre,
	                            "test", "b", NULL);

   registreSystemeAffecterParametre("0x69", &c, miseAJourPararametre,
	                            "test", "c", NULL);

   registreSystemeAffecterParametre("0x24", &d, miseAJourPararametre,
	                            "test", "d", NULL);

   // Imaginons que pour d on souhaite affecter une valeur (quitte à
   // écraser celle définie au boot), par exemple une valeur donnée
   // par une appli :
   registreSystemeAffecterParametre("0x25", &d, NULL,
	                            "test", "d", NULL);

   // Quelques autres paramètres, juste histoire de peupler un
   // registre plus riche
   registreSystemeAffecterParametre("0x483242FF", NULL, NULL,
	                            "systeme", "debug", "mask", NULL);

   registreSystemeAffecterParametre("TRUE", NULL, NULL,
				   "systeme", "debug", "assert", NULL);

   registreSystemeAffecterParametre("192.168.10.2", NULL, NULL, 
				   "reseau", "ip", "eth0", "addr", NULL);

   registreSystemeAffecterParametre("192.168.12.2", NULL, NULL,
   				   "reseau", "ip", "eth1", "addr", NULL);

   registreSystemeAfficher();

   printk("Finalement, le systeme va demarrer avec a=0x%x, b=0x%x, c=0x%x et d=0x%x\n", a, b, c, d);

}   /* startManuX */


