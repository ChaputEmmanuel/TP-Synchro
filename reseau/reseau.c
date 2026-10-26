/**
 * @file reseau/reseau.c
 * @brief Implantation des éléments principaux du sous système réseau
 *
 *                                                                  (C) Manu Chaput 2025
 */
#include <manux/reseau.h>
#include <manux/buffer-reseau.h>
#include <manux/debug.h>

#ifdef MANUX_TACHES
#   include <manux/tache.h>
#endif

/**
 * @brief La liste des buffers qui ont été reçus mais non encore
 * traités 
 */
ListeBufferReseau * listeBuffersRecus = NULL;

static
void reseauTraiterReceptions(void)
{
   printk_debug(DBG_KERNEL_NET, "A-t-on des buffers a recevoir ?\n");
   while (1) {
      if (!listeBufferReseauVide(listeBuffersRecus)) {
         printk_debug(DBG_KERNEL_NET, "Buffers reseau en attente !\n");
      } else {
         ordonnanceur();
      }
   }
}

/**
 * @brief Initialisation du sous-système réseau
 */
void reseauInitialiser(void)
{
   printk_debug(DBG_KERNEL_NET, "Initialisation du sous systeme reseau\n");

   listeBuffersRecus = listeBufferReseauCreer();
   
#ifdef MANUX_TACHES
   // Lancement d'une tâche de traitement des trames reçues
   ordonnanceurCreerTacheNoyau(reseauTraiterReceptions);
#endif
   
   printk_debug(DBG_KERNEL_NET, "Tache de traitement des receptions lancee\n");
}

/**
 * @brief Affichage de la liste de buffers de réception
 */
void reseauAfficherListeReception()
{
   listeBufferReseauAfficher(listeBuffersRecus);
}
