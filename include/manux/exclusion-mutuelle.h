/**
 * @file exclusion-mutuelle.h
 *
 * @brief Définition des exlusions mutuelles.
 *
 *                                                    (C) Manu Chaput 2000-2024
 */
#ifndef EXCLUSION_MUTUELLE_DEF
#define EXCLUSION_MUTUELLE_DEF

#include <manux/types.h>
#include <manux/atomique.h>     // Atomique
#include <manux/listetaches.h>  // Pour les listes de tâches en attente

/**
 * @brief Définition des exclusions mutuelles
 */
typedef struct _ExclusionMutuelle {
   Atomique   verrou;
   ListeTache tachesEnAttente;
#if defined(MANUX_EXCLUSION_MUTUELLE_AUDIT)
   int        nbEntrees;
   int        nbSorties;
#endif
} ExclusionMutuelle;

/**
 * @brief Initialisation d'une exclusion mutuelle
 */
void exclusionMutuelleInitialiser(ExclusionMutuelle * em);

/**
 * @brief Entrée en exclusion mutuelle.
 * 
 * La tâche appelante est éventuellement mise en attente dans une
 * file spécifique. Elle n'en sera extraite que par la sortie d'une
 * tâche qui est dans la zone d'exclusion.
 */
void exclusionMutuelleEntrer(ExclusionMutuelle * em);

void exclusionMutuelleSortir(ExclusionMutuelle * em);

#if defined(MANUX_EXCLUSION_MUTUELLE_AUDIT)
/**
 * @brief Affichage de l'état des variables d'exclusion mutuelle
 */

void exclusionsMutuellesAfficherEtat(void);

#endif // defined(MANUX_EXCLUSION_MUTUELLE_AUDIT)

#endif
