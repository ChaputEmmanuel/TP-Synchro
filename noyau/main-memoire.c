/**
 * @file  noyau/main-memoire.c
 * @brief Comment allouer une page mémoire dans le noyau ?
 *
 *                                                     (C) Manu Chaput 2000-2026
 */
#include <manux/config.h>
#include <manux/memoire.h>
#include <manux/console.h>
#include <manux/printk.h>
#include <manux/bootloader.h>
#include <manux/i386.h>         // ADDR_VERS_PAGE

/**
 * Nombre de pages dont on va demander l'allocation
 */  
#define NB_PAGES_ALLOC 16

void startManuX()
{
   void * p;

   // Initialisation de la console noyau
   consoleInitialisation();

   // Initialisation des informations depuis le bootloader
   bootloaderInitialiser();

   /* Initialisation de la gestion mémoire */
   initialiserMemoire(infoSysteme.memoireDeBase,
		      infoSysteme.memoireEtendue);

   if (NB_PAGES_ALLOC > 0) {
      printk("Allocation de %d pages :\n", NB_PAGES_ALLOC);
   }
   for (int n = 0; n < NB_PAGES_ALLOC; n++) {
      p = allouerPage();
      printk("0x%x (page 0x%2x) ", p, ADDR_VERS_PAGE((int)p));
      if (((n+1) % 4 ) == 0 ) printk("\n");
   }
}   /* _start */

