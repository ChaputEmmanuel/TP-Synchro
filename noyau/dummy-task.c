/**
 * @file noyau/dummy-task.c
 * @brief Implantation d'une tâche de fond du noyau, surtout utile
 * pour de l'affichage  pour le moment.
 *                                                     (C) Manu Chaput 2000-2026
 */
#include <manux/config.h>

#include <manux/ecran.h>
#include <manux/ascii.h>
#include <manux/printk.h>
#include <manux/dummy-task.h>

#include <manux/debug.h>   // printk_debug

#ifdef MANUX_TACHES
#   include <manux/tache.h>
#endif // MANUX_TACHES
#include <manux/scheduler.h>
#ifdef MANUX_CONSOLE
#   include <manux/console.h>
#endif  // MANUX_CONSOLE
#ifdef MANUX_CLAVIER_CONSOLE
#   include <manux/clavier.h>
#endif
#ifdef MANUX_AS_AUDIT
#   include <manux/appelsysteme.h>
#endif
#ifdef MANUX_KMALLOC_STAT
#   include <manux/kmalloc.h>    // kmalloc kmallocAfficherStatistiques
#endif
#ifdef MANUX_VIRTIO_CONSOLE
#   include <manux/virtio-console.h> // A virer
#endif
#ifdef MANUX_VIRTIO_NET
#   include <manux/virtio-net.h> // A virer
#endif
#if defined(MANUX_EXCLUSION_MUTUELLE)
#   include <manux/exclusion-mutuelle.h> // exclusionsMutuellesAfficherEtat
#endif
#if defined(MANUX_CONDITION)
#   include <manux/condition.h>  // exclusionsMutuellesAfficherEtat
#endif
#if defined(MANUX_RESEAU)
#   include <manux/reseau.h>
#endif

/*----------------------------------------------------------------------------*/
/*   Ci dessous les différentes fonctions qui sont impliquées dans des menus  */
/* de la dummyTask. Elles ne peuvent être invoquées que si la gestion du      */
/* clavier est opérationnelle (et donc les menus utilisables !)               */
/*    C'est pourquoi ils sont dans ce gros ifdef (sinon on peut les en sortir */
/* et les mettre en maybe_unused).                                            */
/*----------------------------------------------------------------------------*/
#ifdef MANUX_CLAVIER_CONSOLE    // On ne peut pas les gérer sans clavier !

#ifdef MANUX_DEBUGMASK_VAR
char * debugFlagNames[32] = {
  "ERREUR",
  "START",
  "PAGIN",
  "SYSFI",
  "ORDON",
  "TACHE",
  "MEMOIRE",
  "AS",
  "PCI",
  "NET",
  "VIRTIO",
  "A_FAIRE",
  "BOOTLOADER",
  "TUBE",
  "REGISTRE",
  "ALL"
};

#define MIN(a, b) (((a)<(b))?(a):(b))
#define MAX(a, b) (((a)>(b))?(a):(b))
uint32_t masqueDebugageConsoleSave;
static
void debugMasqueAfficherFlag(int n)
{
   uint32_t f;

   f = 1 << n;
   if (f & masqueDebugageConsoleSave) {
      printkc("%c[47m");
   } else {
      printkc("%c[40m");
   }
   if (f & masqueDebugageFichier) {
      printkc("%c[31m");
   } else {
      printkc("%c[37m");
   }
   printkc("%c[%d;%dH%s", ASCII_ESC, n+2, 5, debugFlagNames[n]);
   printkc("%c[0m");
}

static
void debugMasqueAfficher(void)
{
   int       n;
   char      c;
   int       i = 0;
   
   masqueDebugageConsoleSave = masqueDebugageConsole;
   masqueDebugageConsole = 0x00;

   printkc("%c[2J", ASCII_ESC);

   for (n = 0 ; n < 15 ; n++) {
      debugMasqueAfficherFlag(n);
   }
   printkc("%c[20;0H");
   printkc("masqueDebugageConsole = 0x%x\n", masqueDebugageConsoleSave);
   printkc("masqueDebugageFichier = 0x%x\n", masqueDebugageFichier);

   printkc("%c[30m", ASCII_ESC);
   printkc("%c[100m", ASCII_ESC);

   printkc("%c[24;0Hi = Haut / k = bas / j = Console / l = fichier / espace = fin                  ", ASCII_ESC);
   printkc("%c[0m", ASCII_ESC);

   do {
      while (consoleLire(tacheEnCours->console, &c, 1) == 0){};  // WARNING
      printkc("%c[%d;%dH ", ASCII_ESC, i+2, 4);
      switch (c) {
         case 'i' :
            i--; 
            i = MAX(i, 0);
         break;
         case 'k' :
            i++; 
            i = MIN(i, 14);
         break;
         case 'j' :
            masqueDebugageConsoleSave = masqueDebugageConsoleSave ^(1 << i);
            debugMasqueAfficherFlag(i);
            printkc("%c[20;0H");
            printkc("masqueDebugageConsole = 0x%x\n", masqueDebugageConsoleSave);
            printkc("masqueDebugageFichier = 0x%x\n", masqueDebugageFichier);
         break;
         case 'l' :
            masqueDebugageFichier = masqueDebugageFichier ^(1 << i);
            debugMasqueAfficherFlag(i);
            printkc("%c[20;0H");
            printkc("masqueDebugageConsole = 0x%x\n", masqueDebugageConsoleSave);
            printkc("masqueDebugageFichier = 0x%x\n", masqueDebugageFichier);
         break;
      }
      printkc("%c[%d;%dH>", ASCII_ESC, i+2, 4);
   } while (c != ' ');
   masqueDebugageConsole = masqueDebugageConsoleSave;
}

[[maybe_unused]]
static
void debugMasqueModifier(void)
{
   printkc("masqueDebugageConsole : ");
   masqueDebugageConsole = consoleLireEntier(consoleNoyau());
   printkc("0x%x\n", masqueDebugageConsole);
   printkc("masqueDebugageFichier : ");
   masqueDebugageFichier = consoleLireEntier(consoleNoyau());
   printkc("0x%x\n", masqueDebugageFichier);
}

#endif // MANUX_DEBUGMASK_VAR

#ifdef MANUX_AS_AUDIT
/**
 * @brief Affichage sur la console des AS de chaque tâche
 */
static
void appelsSystemeAfficher(void)
{
   CelluleTache * celluleTache;

   printkc("\nTache  | Appels Systeme (num:in/out)\n");
   printkc("-------+----------------------------------------\n");
   for (celluleTache = listeToutesLesTaches.tete;
      celluleTache != NULL;
      celluleTache = celluleTache->suivant){
      printkc("%3d    | ", celluleTache->tache->numero);

      for (int i=0; i < NB_MAX_APPELS_SYSTEME; i++) {
         if (celluleTache->tache->nbAppelsSystemeIn[i]) {
	    printkc("%d:%d/%d ", i,
		   celluleTache->tache->nbAppelsSystemeIn[i],
		   celluleTache->tache->nbAppelsSystemeOut[i]);
         }
      }
      printkc("\n");
   }
}
#endif  // MANUX_AS_AUDIT

#ifdef MANUX_INT_AUDIT
/**
 * @brief Affichage des IT reçues
 *
 * Le but est de présenter un écran synthétique avec le nombre
 * d'occurences de chacune des interruptions.
 */
static
void interruptionAfficher(void)
{
   int i;

   printkc("----[ %d Exceptions ]--------------------------\n", MANUX_NB_EXCEPTIONS);
   for (i = 0; i < MANUX_NB_EXCEPTIONS ; i ++) {
      if (nbItRecues[i]) {
         printkc(" [ %3d : %5d ]", i, nbItRecues[i]); 
      }
   }
   printkc("\n");
   
   printkc("----[ %d IRQ ]---------------------------------\n", MANUX_NB_IRQ);
   for (i = MANUX_NB_EXCEPTIONS; i < MANUX_NB_EXCEPTIONS + MANUX_NB_IRQ ; i ++) {
      if (nbItRecues[i]) {
         printkc(" [ %3d : %5d ]", i, nbItRecues[i]); 
      }
   }
   printkc("\n");
   
   printkc("----[ %d Interruptions ]-----------------------\n", MANUX_NB_INTERRUPTIONS);
   for (i = MANUX_NB_EXCEPTIONS + MANUX_NB_IRQ; i < MANUX_NB_INTERRUPTIONS ; i ++) {
      if (nbItRecues[i]) {
         printkc(" [ %3d : %5d ]", i, nbItRecues[i]); 
      }
   }
   printkc("\n");
}
#endif // MANUX_INT_AUDIT

#if  defined(MANUX_TACHES) \
 &&  defined(MANUX_SYNCHRONISATION) \
 &&  defined(MANUX_EXCLUSION_MUTUELLE) \
 && !defined(MANUX_REENTRANT)
/**
 * @brief Etat du verrou général
 */
void afficherEtatMutex()
{
   printkc("\n-- Tache dans le noyau : %d \n-- Taches en attente : ", tacheDansLeNoyau);
   for (CelluleTache * celluleTache = verrouGeneralDuNoyau.tachesEnAttente.tete;
        celluleTache != NULL;
	celluleTache = celluleTache->suivant){
      printkc("%d ", celluleTache->tache->numero);
   }
#ifdef MANUX_EXCLUSION_MUTUELLE_AUDIT
   printkc("\n-- %d ent / %d sor\n", verrouGeneralDuNoyau.nbEntrees, verrouGeneralDuNoyau.nbSorties);
#endif // MANUX_EXCLUSION_MUTUELLE_AUDIT
}
#endif

/*----------------------------------------------------------------------------*/
/* Gestion des menus.                                                         */
/*----------------------------------------------------------------------------*/
static void dummyMessageAide(void);
static void dummyCopieEcran(void);

typedef struct _MenuDebogage {
   char touche;
   void (*action)(void);
   char * aide;
} MenuDebogage;

/**
 * @brief Définition et initialisation du menu de la dummyTask
 */
MenuDebogage menuDebogage[] = {
  {'h', dummyMessageAide, "Afficher ce menu d'aide"},
#ifdef MANUX_DEBUGMASK_VAR
  {'d', debugMasqueAfficher, "Editer les masques de debug"},
#endif // MANUX_DEBUGMASK_VAR
#ifdef MANUX_APPELS_SYSTEME
  {'a', appelsSystemeAfficher, "Voir le decompte des appels systeme"},
#endif // MANUX_APPELS_SYSTEME
#ifdef MANUX_INT_AUDIT
  {'i', interruptionAfficher, "Voir le decompte des interruptions"},
#endif // MANUX_INT_AUDIT
  {'p', afficherEtatTaches, "Voir l'etat des taches en cours"},
#ifdef MANUX_KMALLOC_STAT
  {'m', kmallocAfficherStatistiques, "Voir l'etat des allocations memoire"},
#endif // MANUX_KMALLOC_STAT
#if defined(MANUX_BUFFER_RESEAU)
  {'r', reseauAfficherListeReception, "Voir la liste de reception"},
#endif // MANUX_BUFFER_RESEAU
   {0, NULL, NULL}
};  
int menuActif = 0;

/**
 * @brief Affichage de l'aide des menus de la dummyTask
 */
static void dummyMessageAide(void)
{
   int i;
   
   printkc("%c[2JBienvenue dans la tache d'observation/debogage\n", ASCII_ESC);
   printkc("\n\n");
   printkc("Vous pouvez utiliser les touches suivantes :\n");
   printkc("\n\n");

   for (i = 0; menuDebogage[i].touche != 0; i++){
      printkc("   [%c]   %s\n", menuDebogage[i].touche, menuDebogage[i].aide);
   }
}

/**
 * @brief Copie du contenu de la console via printk
 */
#define TAILLE_ECRAN (MANUX_CON_LIGNES*MANUX_CON_COLONNES)

static void dummyCopieEcran(void)
{
   int s = TAILLE_ECRAN - 1 - 80;   // - 80 pour ne pas copier la ligne d'aide 
   int d = s; // Destination
   char * ecran = tacheEnCours->console->adresseEcran;
   
#ifdef MANUX_KMALLOC
   char * buffer = kmalloc(TAILLE_ECRAN);
#else
   char buffer[TAILLE_ECRAN];
#endif   // MANUX_KMALLOC

   buffer[d--] = 0;
   buffer[d--] = '\n';
   
   // On consomme les espace de fin
   for (; ((s >=0) && (ecran[2*s] == ' ')); s--) {}

   while (s >= 0) {
     if (((TAILLE_ECRAN - 1 - s)% MANUX_CON_COLONNES) == 0) {
         buffer[d--] = '\n';
     }
     buffer[d--] = ecran[2*s];
     s--;
   }
   printk(buffer+d+1);
   
#ifdef MANUX_KMALLOC
   kfree(buffer);
#endif // MANUX_KMALLOC
}

/**
 * @brief Invocation du menu choisi et affichage cohérent
 */
static void afficherMenuActif(void)
{
   printkc("%c[2J", ASCII_ESC);
   menuDebogage[menuActif].action();
   printkc("%c[30m", ASCII_ESC);  // WARNING macro
   printkc("%c[42m", ASCII_ESC);
   printkc("%c[24;0H<espace> refresh <h> aide <!> log ecran                                        ", ASCII_ESC);
   printkc("%c[m", ASCII_ESC);
}

/**
 * @brief Gestion du clavier pour la dummy
 *
 * Tant qu'il y a des caractères à lire, on les traite en affichant en
 * particulier le menu correspondant.
 */
static void dummyTraiterClavier(void)
{
   Console * cons = tacheEnCours->console; // C'est éventuellement celle du noyau
   
   char c;
   int i;
   
   while (cons->nbCarAttente) {
      consoleLire(cons, &c, 1);
      for (i = 0; menuDebogage[i].touche != 0; i++){
	 if (c == menuDebogage[i].touche) {
            menuActif = i;
	    afficherMenuActif();
         } else if (c == ' ') {   // Mise-à-jour
	    afficherMenuActif();
         } else if (c == '!') {   // printk
            dummyCopieEcran();
	 }
      }
   }  
}
#endif // MANUX_CLAVIER_CONSOLE

/**
 * @brief Le corps d'une tâche à exécuter lorsqu'on n'a que ça à faire, ...
 *
 * Cette tâche ne fait rien de très important, si ce n'est récupérer
 * les événements clavier sur la console noyau. Grâce à ça c'est elle
 * qui permet d'afficher quelques informations sur le système
 */
void aDummyKernelTask(void)
{
#ifdef MANUX_CLAVIER_CONSOLE
   dummyMessageAide(); // WARNING : mutex ?
#endif

   while(1) {
#if defined(MANUX_EXCLUSION_MUTUELLE) && !defined(MANUX_REENTRANT)
      // Cette tâche passe sa vie dans le noyau, elle doit donc
      // acquérir le verrou si le noyau n'est pas réentrant.
      exclusionMutuelleEntrer(&verrouGeneralDuNoyau);
      assert(tacheDansLeNoyau == 0);
      tacheDansLeNoyau = tacheEnCours->numero;
#endif

      printk_debug(DBG_KERNEL_ORDON, "aDummyKernelTask running\n");

#ifdef MANUX_CLAVIER_CONSOLE
      dummyTraiterClavier();
#endif

#if defined(MANUX_EXCLUSION_MUTUELLE) && !defined(MANUX_REENTRANT)
      // Cette tâche passe sa vie dans le noyau, elle doit donc
      // rendre le verrou si le noyau n'est pas réentrant.
      tacheDansLeNoyau = 0;
      exclusionMutuelleSortir(&verrouGeneralDuNoyau);
#endif

#ifdef MANUX_TACHES
      ordonnanceur();
#endif
   }
}



