/**
 * @file condition.h
 *
 *      Définition des conditions
 *
 *                                                    (C) Manu Chaput 2000-2024
 */
#ifndef CONDITION_DEF
#define CONDITION_DEF

#include <manux/types.h>
#include <manux/listetaches.h>  // Pour les listes de tâches en attente 
#include <manux/exclusion-mutuelle.h>

/**
 * @brief Définition des conditions
 */
typedef struct _condition {
   ListeTache tachesEnAttente;
#if defined(MANUX_CONDITION_AUDIT)
   int nbSignaler;
   int nbDiffuser;
#endif
} Condition;

/**
 * @brief Initialisation d'une condition
 */
void conditionInitialiser(Condition * cond);

/**
 * @brief Attente de la prochaine occurence d'une condition
 *
 * La tâche appelante doit être dans l'exclusion mutuelle
 * (qui sera automatiquement libérée le temps de l'attente puis
 * reprise avant que cette fonction ne rende la main)
 */
void conditionAttendre(Condition * cond, ExclusionMutuelle * em);

/**
 * @brief Signaler une occurence d'une condition à une tâche en
 * attente 
 *
 * Attention, doit être utilisée sous la protection de l'exclusion
 * mutuelle détenue par les tâches en attente.
 * Ell est donc inutilisable dans un handler d'interruption.
 */
void conditionSignaler(Condition * cond);

/**
 * @brief Signaler une occurence d'une condition à toutes les tâches
 * en attente
 *
 * Attention, doit être utilisée sous la protection de l'exclusion
 * mutuelle détenue par les tâches en attente.
 * Ell est donc inutilisable dans un handler d'interruption.
 */
void conditionDiffuser(Condition * cond);

#if defined(MANUX_CONDITION_AUDIT)

/**
 * @brief Affichage de l'état des variables condition
 */
void conditionsAfficherEtat(void);

#endif //  defined(MANUX_CONDITION_AUDIT)

#endif // CONDITION_DEF
