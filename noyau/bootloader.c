/**
 * @file bootloader.c
 * @brief Gestion de l'interfaçage avec le bootloader
 *
 *                                                     (C) Manu Chaput 2000-2025 
 */
#include <manux/bootloader.h>
#include <manux/stdlib.h>    // atoihex
#include <manux/debug.h>     // paniqueNoyau, assert
#include <manux/string.h>    // memcpy, prochainDelimiteur
#include <manux/registre.h>  // registreSystemeAjouterC

#define CMDLINE_MAX_LENGTH 512

/**
 * @brief Zone destinée à héberger le contenu de la ligne de commande.
 */
static char cmdLine[CMDLINE_MAX_LENGTH];

/**
 * @brief Valeur passée via %eax par le bootloader pour s'identifier
 */ 
uint32_t signatureBootloader;

/**
 * @brief Stockage des informations décrivant le système fournies par
 * le bootloader.
 */
InfoSysteme infoSysteme;

/**
 * @brief Adresse de la zone mémoire dans laquelle le bootloader nous
 * fournit les informations sur le système.
 */
InfoSysteme * _infoSysteme;

/**
 * @brief Lecture des informations fournies par le bootloader
 *
 *    Cette fonction doit être invoquée relativement tôt, car elle a la
 * charge de recopier des informations fournies par le bootloader. Ces
 * informations étant dans une zone mémoire que l'on risque ensuite
 * d'utiliser.
 *    Il s'agit de infoSysteme et cmdLine.
 */
void bootloaderInitialiser(void)
{
   // On copie les infos dans la variable infoSysteme, qui est dans
   // une zone que la gestion mémoire va considérée comme
   // réservée. Ainsi le contenu est préservé.
   memcpy(&infoSysteme, _infoSysteme, sizeof(InfoSysteme));
  
   // Un petit message
   printk_debug(DBG_KERNEL_BOOTLOADER, "SignatureBootloader = 0x%x, is=0x%x (flags = 0x%x)\n",
	  signatureBootloader,
	  infoSysteme,
	  infoSysteme.flags);

   // Quel est le bootloader ?
   switch (signatureBootloader) {
      case MULTIBOOT_SIGNATURE :
         printk_debug(DBG_KERNEL_BOOTLOADER, "C'est multiboot qui nous a lance.\n");
      break;
      case MANUX_INIT_SIGNATURE :
         printk_debug(DBG_KERNEL_BOOTLOADER, "C'est init-manux qui nous a lance.\n");
      break;
      default :
         paniqueNoyau("Bootloader inconnue : 0x%x\n", signatureBootloader);
   }

   // Alignement des modules à la page ? (Pas important pour nous)
   if ((infoSysteme.flags &  MULTIBOOT_PAGE_ALIGN) ==  MULTIBOOT_PAGE_ALIGN) {
      printk_debug(DBG_KERNEL_BOOTLOADER, "Alignement a la page\n");
   }

   // A-t-on des infos sur la mémoire ?
   if ((infoSysteme.flags & MULTIBOOT_MEMORY_INFO) == MULTIBOOT_MEMORY_INFO) {
      printk_debug(DBG_KERNEL_BOOTLOADER, "Base = %d, etendue = %d\n",
	  infoSysteme.memoireDeBase,
	  infoSysteme.memoireEtendue);
   } else {
      paniqueNoyau("Aucune information sur la memoire !\n");
   }

   // Informations de la ligne de commande (eg GRUB)
   if ((infoSysteme.flags & MULTIBOOT_INFO_CMDLINE) == MULTIBOOT_INFO_CMDLINE) {
      printk_debug(DBG_KERNEL_BOOTLOADER, "Command line : \"%s\"\n", infoSysteme.ligneCommande);

      // On sauvegarde ça
      int l = strlen(infoSysteme.ligneCommande);
      memcpy(cmdLine, infoSysteme.ligneCommande, ((l > CMDLINE_MAX_LENGTH)?CMDLINE_MAX_LENGTH:l));
      cmdLine[(l > CMDLINE_MAX_LENGTH)?CMDLINE_MAX_LENGTH:l] = 0;
   }
}

/**
 * @brief Analyse de la ligne de commande et insertion de paramètres
 * dans le registre
 */
#ifndef BOOTLOADER_CMD_SEPARATEUR
#   define BOOTLOADER_CMD_SEPARATEUR ','
#endif

void bootloaderLireLigneCmd(void)
{
#ifdef MANUX_REGISTRE
   char   param[512] ; // Une chaîne de caractères qui contient le
                       // paramètre en cours d'analyse
   int lgParam;
#endif
   char * debut, * fin ; // Pointeurs
   
   // La ligne de commande est une liste d'éléments séparés par des
   // BOOTLOADER_CMD_SEPARATEUR

   debut = cmdLine;
   
   while (strlen(debut)) {
      fin = prochainDelimiteur(debut, BOOTLOADER_CMD_SEPARATEUR);
      assert(fin > debut);   // Puisque strlen(debut) != 0

#ifdef MANUX_REGISTRE   // A priori on place dans le registre
      lgParam = fin - debut;
      memcpy(param, debut, lgParam);
      param[lgParam] = 0;
      registreSystemeAjouterC(param);
#else     // Si pas de registre, on gère ce qu'on sait faire
#   ifdef MANUX_DEBUGMASK_VAR
      if (strncmp(debut, NOM_MASQUE_CONSOLE, strlen(NOM_MASQUE_CONSOLE)) == 0) {
         masqueDebugageConsole = atoihex(debut + strlen(NOM_MASQUE_CONSOLE)+1);
      } else if (strncmp(debut, NOM_MASQUE_FICHIER, strlen(NOM_MASQUE_FICHIER)) == 0) {
         masqueDebugageFichier = atoihex(debut + strlen(NOM_MASQUE_FICHIER)+1);
      }
#   endif // MANUX_DEBUGMASK_VAR
#endif // MANUX_REGISTRE
      
      // Si on a un séparateur, on avance, sinon, on est sur le 0 final
      if (*fin == BOOTLOADER_CMD_SEPARATEUR) {
         debut = fin + 1;
      } else {
         debut = fin;
      }
   }
}

