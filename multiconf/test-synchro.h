/**
 * @file multiconf/test-synchro.h
 * @brief Exemple de besoin de synchronisation dans le noyau
 *
 *                                                          (C) Manu Chaput 2025
 */

#ifdef MANUX_FICHIER_CONFIG
#   error "Cette chose ne devrait pas se produire !"
#   include MANUX_FICHIER_CONFIG
#else
#ifndef MANUX_CONFIG
#define MANUX_CONFIG

/**
 * @brief pour ajouer quelques fonctions qui ne servent qu'ici
 */
#define MANUX_AS_TEST_SYNCHRO

#define MANUX_FICHIER_MAIN main-test-synchro
#define MANUX_USR_INIT init-test-synchro.o

#include <config/base.h>
#include <config/pc-i386.h>         // Besoin des interuptions pour les AS
#include <config/appels-systeme.h>
#include <config/gestion-memoire.h>
#include <config/taches.h>
#include <config/console.h>
#include <config/printk.h>
#include <config/usr.h>
#include <config/synchronisation.h>
#undef MANUX_ATOMIQUE_AUDIT
#undef MANUX_CONDITION_AUDIT
#undef MANUX_EXCLUSION_MUTUELLE_AUDIT
#include <config/verifications.h>

#endif  // MANUX_CONFIG
#endif  // MANUX_FICHIER_CONFIG
