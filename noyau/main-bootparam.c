/**
 * @file  : main-bootparam.c
 * @brief : Un exemple de noyau montrant le passage de paramètres au boot.
 *
 *                                                  (C) Manu Chaput 2000-2024 
 */
#include <manux/config.h>
#include <manux/console.h>
#include <manux/printk.h>
#include <manux/bootloader.h>

#include <manux/debug.h>

void startManuX()
{
   // Initialisation de la console noyau
   consoleInitialisation();

   bootloaderInitialiser();

   // Un petit message
   printk("SignatureBootloader = 0x%x, is=0x%x (flags = 0x%x)\n",
	  signatureBootloader,
	  infoSysteme,
	  infoSysteme.flags);
   
   switch (signatureBootloader) {
      case MULTIBOOT_SIGNATURE :
         printk("C'est multiboot qui nous a lance.\n");
      break;
      case MANUX_INIT_SIGNATURE :
         printk("C'est init-manux qui nous a lance.\n");
      break;
   }

   if ((infoSysteme.flags &  MULTIBOOT_PAGE_ALIGN) ==  MULTIBOOT_PAGE_ALIGN) {
     printk("Alignement a la page\n");
   }

   if ((infoSysteme.flags & MULTIBOOT_MEMORY_INFO) == MULTIBOOT_MEMORY_INFO) {
     printk("Base = %d, etendue = %d\n",
	  infoSysteme.memoireDeBase,
	  infoSysteme.memoireEtendue);

   }

   if ((infoSysteme.flags & MULTIBOOT_INFO_CMDLINE) == MULTIBOOT_INFO_CMDLINE) {
     printk("Command line : \"%s\"\n", infoSysteme.ligneCommande);
   }

   printk("That's all she wrote\n");

}   /* startManuX */


