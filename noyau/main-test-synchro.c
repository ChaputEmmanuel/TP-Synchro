/**
 * @file noyau/main-test-synchro.c
 * @brief Démonstration des besoins de sychronisation
 *                                                          (C) Manu Chaput 2025
 */
#include <manux/config.h>
#include <manux/console.h>
#include <manux/printk.h>
#include <manux/interruptions.h>
#include <manux/intel-8259a.h>

extern void init(void);
void startManuX(void);

void startManuX(void)
{
   // Initialisation de la console noyau
   consoleInitialisation();

   // Initialisation de la table des interruptions
   initialiserIDT();   

   i8259aInit(MANUX_INT_BASE_IRQ);
   
   initialiserAppelsSysteme();
   
   // Un petit message
   printk("Printk dit bonjour !\n");

   init();
}   /* startManuX */


