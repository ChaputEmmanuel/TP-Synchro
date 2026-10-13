/**
  @file
  @brief Implantation des fonctions de base du mode utilisateur.

                                                       (C) Manu Chaput 2002-2025
*/
#include <manux/config.h>
#include <manux/appelsysteme.h>

#include <unistd.h>

appelSysteme0(NBAS_BASCULER_TACHE, int, basculerTache);

appelSysteme0(NBAS_IDENTIFIANT_TACHE, int, identifiantTache);

appelSysteme0(NBAS_DUMB, int, appelSystemeInutile);

typedef void (CorpsTache());

appelSysteme2(NBAS_CREER_TACHE, int, creerNouvelleTache, CorpsTache, booleen);

#ifdef MANUX_TUBES
appelSysteme1(NBAS_TUBE, int, tube, int *);
#endif

/**
 * @brief un appel système sans intérêt si ce n'est de faire des tests/demo
 */
#ifdef MANUX_AS_TEST_SYNCHRO
appelSysteme2(NBAS_TEST_SYNC, int, testSynchro, int, int);
#endif
