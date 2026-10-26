/**
 * @file config/synchronisation.h
 *
 * @brief Configuration des outils de synchronisation de ManuX
 */
#define MANUX_SYNCHRONISATION

#if defined (MANUX_SYNCHRONISATION)
/**
 * Utilisation des outils de synchro de base
 */
#define MANUX_ATOMIQUE

/**
 * @brief Intégration des exclusions mutuelles
 */
#define MANUX_EXCLUSION_MUTUELLE

/**
 * @brief Intégration des conditions
 */
#define MANUX_CONDITION

/**
 * @brief Ajout de structures et fonctions permettant d'observer l'état
 * des outils de synchronisation 
 */
#define MANUX_ATOMIQUE_AUDIT

#define MANUX_EXCLUSION_MUTUELLE_AUDIT

#define MANUX_CONDITION_AUDIT

/**
 * @brief Le noyau est-il réentrant ?
 *
 * Si un élément ne l'est pas, cette macro ne doit pas être définie
 */
#define MANUX_REENTRANT
//#undef MANUX_REENTRANT

#endif // defined (MANUX_SYNCHRONISATION)

