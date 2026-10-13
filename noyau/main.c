/**
 * @file : noyau/main.c
 * @brief : Un exemple de début de noyau.
 *
 * J'intègre à peu près tout dans ce noyau, donc régulièrement il ne
 * compile pas ou ne fonctionne pas.
 *                                                     (C) Manu Chaput 2000-2026
 **/

#include <manux/config.h>
#include <manux/bootloader.h>
#include <manux/registre.h>
#include <manux/errno.h>
#include <manux/console.h>
#include <manux/clavier.h>
#include <manux/tache.h>
#include <manux/horloge.h>        // initialiserHorloge
#include <manux/scheduler.h>
#include <manux/interruptions.h>
#include <manux/io.h>
#include <manux/segment.h>
#include <manux/memoire.h>
#include <manux/stdlib.h>
#include <manux/kmalloc.h>
#include <manux/atomique.h>
#include <manux/appelsysteme.h>
#include <manux/pci.h>
#include <manux/ramdisk.h>
#include <manux/printk.h>
#include <manux/debug.h>
#include <manux/limites.h>
#include <manux/pagination.h>
#include <manux/journal.h>
#include <manux/fichier.h>
#include <manux/virtio-net.h>
#include <manux/virtio-console.h>

void startManuX(void);  // WARNING faire un include ?
extern void init(void); // Faire un init.h

// Pour manipuler la console virtuelle comme un fichier
INoeud iNoeudVirtioConsole;
Fichier fichierVirtioConsole;

/**
 * Configuration de la console
 */
INoeud  iNoeudConsole;  // Le INoeud qui décrit la console

void startManuX(void)
{
   // Initialisation de la console manipulée comme un fichier
   consoleInitialisationINoeud(&iNoeudConsole);

#if MANUX_ARCH == i386
   union {
      uint32_t registres[3];
      char     caracteres[13];
   } descriptionProc;

   // Lecture du nom du processeur
   descriptionProcesseur(0, descriptionProc.registres);
   descriptionProc.caracteres[12] = 0;

   // Affichage d'un premier message
   printk_debug(DBG_KERNEL_START, "32 bit ManuX running on a '%s' ...\n",
		descriptionProc.caracteres);
#endif

   // Initialisation du bootloader : infoSysteme, cmdLine, ...
   printk_debug(DBG_KERNEL_START, "Initialisation du bootloader ...\n");
   bootloaderInitialiser();

   // Initialisation de la gestion des pages mémoire (utilise le
   // bootloader)
   printk_debug(DBG_KERNEL_START, "Initialisation memoire ...\n");
   initialiserMemoire(infoSysteme.memoireDeBase,
		      infoSysteme.memoireEtendue);

   // Affichage de la mémoire disponible 
   printk_debug(DBG_KERNEL_START, "Memoire : %d + %d Ko\n",
		infoSysteme.memoireDeBase,
		infoSysteme.memoireEtendue);

   // Initialisation des descripteurs de segments
   initialiserGDT();

   // Initialisation de la table des interruptions
   initialiserIDT();   

   // Initialisation de la pagination
   printk_debug(DBG_KERNEL_START, "Initialisation pagination ...\n");
   initialiserPagination(infoSysteme.memoireEtendue);
   printk_debug(DBG_KERNEL_START, "Pagination initialisee\n");

   // Initilisation du système kmalloc
   printk_debug(DBG_KERNEL_START, "Initialisation de kmalloc ...\n");
   kmallocInitialisation();

   // Initialisation du système de debug
   printk_debug(DBG_KERNEL_START, "Initialisation du debug ...\n");
   debugInitialiser();

   // Initialisation du journal
   printk_debug(DBG_KERNEL_START, "Initialisation du journal ...\n");
   journalInitialiser();
   printk_debug(DBG_KERNEL_START, "Journal initialise ...\n");

   // Initialisation de la table des appels système
   printk_debug(DBG_KERNEL_START, "Initialisation appels systeme ...\n");
   initialiserAppelsSysteme();
   printk_debug(DBG_KERNEL_START, "Appels systeme initialises\n");
   
   // Initialisation du bus PCI
   printk_debug(DBG_KERNEL_START, "Initialisation du bus PCI ...\n");
   PCIEnumerationDesEquipements();
   printk_debug(DBG_KERNEL_START, "Bus PCI initialise...\n");

#ifdef MANUX_VIRTIO_CONSOLE
   printk_debug(DBG_KERNEL_START, "Initialisation de virtio console ...\n");
   if (virtioConsoleInitialisation(&iNoeudVirtioConsole) == ESUCCES) {
      fichierOuvrir(&iNoeudVirtioConsole, &fichierVirtioConsole, O_WRONLY, 0);
      journalAffecterFichier(&fichierVirtioConsole);
   }
   printk_debug(DBG_KERNEL_START, "Virtio console initialise...\n");
#endif

#ifdef MANUX_RESEAU
   // Initialisation du réseau
   printk_debug(DBG_KERNEL_START, "Initialisation du reseau ...\n");
#   ifdef MANUX_VIRTIO_NET
   virtioNetInit();
#   endif
   printk_debug(DBG_KERNEL_START, "Reseau initialise\n");
#endif

#ifdef MANUX_FICHIER
   // Initialisation de la gestion des systèmes de fichiers
   printk_debug(DBG_KERNEL_START, "Initialisation du systeme de fichiers ...\n");
   sfInitialiser();
   printk_debug(DBG_KERNEL_START, "Systeme de fichiers initialise\n");
#endif

#ifdef MANUX_CLAVIER
   // Initialisation du clavier
   printk_debug(DBG_KERNEL_START, "Initialisation du clavier ...\n");
   initialiserClavier();
   printk_debug(DBG_KERNEL_START, "Clavier initialise\n");
#endif

#ifdef MANUX_TACHES
   // Initialisation de la gestion des processus
   printk_debug(DBG_KERNEL_START, "Initialisation du scheduler ...\n");
   initialiserScheduler();
   printk_debug(DBG_KERNEL_START, "Scheduler initialise\n"); 
#endif

   printk_debug(DBG_KERNEL_START, "Initialisation de l'horloge ...\n");
   initialiserHorloge();
   printk_debug(DBG_KERNEL_START, "Horloge initialisee\n");


#ifdef MANUX_REGISTRE
   // Initialisation du registre (utilise kmalloc)
   printk_debug(DBG_KERNEL_START, "Initialisation du registre ...\n");
   registreSystemeInitialiser();
   printk_debug(DBG_KERNEL_START, "Registre initialise...\n");
#endif

#ifdef MANUX_CONSOLES_VIRTUELLES
   // On va maintenant faire de la tâche en cours une tâche "banale"
   tacheSetConsole(tacheEnCours, creerConsoleVirtuelle());
#endif

   init();
}   /* startManuX */


