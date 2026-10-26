/**
 * @file : noyau/main.c
 * @brief : Un exemple de début de noyau.
 *
 * J'intègre à peu près tout dans ce noyau, donc régulièrement il ne
 * compile pas ou ne fonctionne pas car il n'est pas complètement à
 * jours avec mes dernières modifications.
 * Il est de plus assez pénible à lire car j'utilise intensivement les
 * conditions du préprocesseurs pour bien les identifier. Dans une
 * utilisation plus classique, la plupart doivent disparaître.
 *                                                                            
 *                                                     (C) Manu Chaput 2000-2026
 **/
#undef MANUX_RAMDISK

#include <manux/config.h>
#ifdef MANUX_BOOTLOADER
#include <manux/bootloader.h>
#endif
#ifdef MANUX_REGISTRE
#   include <manux/registre.h>
#endif
#include <manux/errno.h>
#ifdef MANUX_CONSOLE
#include <manux/console.h>
#endif // MANUX_CONSOLE
#ifdef MANUX_CLAVIER
#include <manux/clavier.h>
#endif // MANUX_CLAVIER
#include <manux/tache.h>
#include <manux/horloge.h>        // initialiserHorloge
#include <manux/scheduler.h>
#include <manux/interruptions.h>
#include <manux/io.h>
#include <manux/segment.h>
#include <manux/memoire.h>
#ifdef MANUX_STDLIB
#include <manux/stdlib.h>
#endif
#ifdef MANUX_KMALLOC
#include <manux/kmalloc.h>
#endif
#include <manux/atomique.h>
#ifdef MANUX_APPELS_SYSTEME
#include <manux/appelsysteme.h>
#endif
#ifdef MANUX_PCI
#include <manux/pci.h>
#endif
#ifdef MANUX_RAMDISK
#include <manux/ramdisk.h>
#endif
#ifdef MANUX_PRINTK
#include <manux/printk.h>
#endif
#include <manux/debug.h>
#include <manux/limites.h>
#ifdef MANUX_PAGINATION
#   include <manux/pagination.h>
#endif
#ifdef MANUX_JOURNAL
#   include <manux/journal.h>
#endif
#ifdef MANUX_FICHIER
#   include <manux/fichier.h>
#endif
#ifdef MANUX_RESEAU
#   ifdef MANUX_VIRTIO_NET
#      include <manux/virtio-net.h>
#   endif
#endif
#ifdef MANUX_VIRTIO_CONSOLE
#   include <manux/virtio-console.h>
#endif

void startManuX(void);  // WARNING faire un include ?
extern void init(void); // Faire un init.h

#ifdef MANUX_VIRTIO_CONSOLE
INoeud iNoeudVirtioConsole;
Fichier fichierVirtioConsole;
#endif

/**
 * Configuration de la console
 */
INoeud  iNoeudConsole;  // Le INoeud qui décrit la console

void startManuX(void)
{
#ifdef MANUX_CONSOLE
#   ifdef MANUX_FICHIER
   consoleInitialisationINoeud(&iNoeudConsole);
#   else
   // Initialisation de la console noyau (n'utilise rien !)
   consoleInitialisation();
#   endif
#endif

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

   // Initialisation du système de debug
   debugInitialiser();
   
#ifdef MANUX_BOOTLOADER
   // Initialisation du bootloader : infoSysteme, cmdLine, ...
   printk_debug(DBG_KERNEL_START, "Initialisation du bootloader ...\n");
   bootloaderInitialiser();

   // Affichage de la mémoire disponible 
   printk_debug(DBG_KERNEL_START, "Memoire : %d + %d Ko\n",
		infoSysteme.memoireDeBase,
		infoSysteme.memoireEtendue);
#endif
   
#ifdef MANUX_GESTION_MEMOIRE
   // Initialisation de la gestion des pages mémoire (utilise le
   // bootloader)
   printk_debug(DBG_KERNEL_START, "Initialisation memoire ...\n");
   initialiserMemoire(infoSysteme.memoireDeBase,
		      infoSysteme.memoireEtendue);
#endif

   // Initilisation des descripteurs de segments
   initialiserGDT();

   // Initialisation de la table des interruptions
   initialiserIDT();   

#ifdef MANUX_JOURNAL
   // Initialisation du journal
   printk_debug(DBG_KERNEL_START, "Initialisation du journal ...\n");
   journalInitialiser();
   printk_debug(DBG_KERNEL_START, "Journal initialise ...\n");
#endif

#ifdef MANUX_PAGINATION
   /* Initialisation de la pagination */
   printk_debug(DBG_KERNEL_START, "Initialisation pagination ...\n");
   initialiserPagination(infoSysteme.memoireEtendue);
   printk_debug(DBG_KERNEL_START, "Pagination initialisee\n");
#endif
   
#ifdef MANUX_APPELS_SYSTEME
   /* Initialisation de la table des appels système*/
   printk_debug(DBG_KERNEL_START, "Initialisation appels systeme ...\n");
   initialiserAppelsSysteme();
   printk_debug(DBG_KERNEL_START, "Appels systeme initialises\n");
#endif
   
#ifdef MANUX_PCI
   /* Initialisation du bus PCI */
   printk_debug(DBG_KERNEL_START, "Initialisation du bus PCI ...\n");
   PCIEnumerationDesEquipements();
   printk_debug(DBG_KERNEL_START, "Bus PCI initialise...\n");
#endif

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

#ifdef MANUX_RAMDISK
   // Initialisation du ramdisk 
   if (infoSysteme.tailleRamdisk > 0) {
      printk_debug(DBG_KERNEL_START, "Ramdisk (a %d de taille %d Ko) ...\n",
	     adresseRamdisk, infoSysteme.tailleRamdisk);
      initialiserRamDisk(adresseRamdisk, infoSysteme.tailleRamdisk);
      printk_debug(DBG_KERNEL_START, "Ramdisk initialise\n");
   }
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

#ifdef MANUX_KMALLOC
   // Initilisation du système kmalloc
   printk_debug(DBG_KERNEL_START, "Initialisation de kmalloc ...\n");
   kmallocInitialisation();
   printk_debug(DBG_KERNEL_START, "kmalloc initialise\n");
#endif

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


