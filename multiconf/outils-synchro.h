/**
 * @file multiconf/mutex.h
 * @brief Exemple de configuration pour l'utilisation des exclusions mutuelles
 *
 *                                                     (C) Manu Chaput 2000-2023
 */

#ifdef MANUX_FICHIER_CONFIG
#   error "Cette chose ne devrait pas se produire !"
#   include MANUX_FICHIER_CONFIG
#else
#ifndef MANUX_CONFIG
#define MANUX_CONFIG

/**
 * Pour utiliser un AS spécifique
 */
#define MANUX_AS_TEST_SYNCHRO

#define MANUX_FICHIER_MAIN main-verrou-noyau
#define MANUX_USR_INIT init-verrou-noyau.o

#include <config/bootloader.h>
#include <config/base.h>
#include <config/plan-memoire-pc.h>
#include <config/clavier.h>
#include <config/console.h>
#include <config/consoles-virtuelles.h>
#include <config/printk.h>
#include <config/gestion-memoire.h>
#include <config/pc-i386.h>    // Besoin des inter pour les tâches
#include <config/synchronisation.h>
#undef MANUX_ATOMIQUE_AUDIT
#undef MANUX_EXCLUSION_MUTUELLE_AUDIT
#undef MANUX_CONDITION_AUDIT
#undef MANUX_REENTRANT
#include <config/taches.h>
#include <config/appels-systeme.h>
#include <config/usr.h>

#include <config/verifications.h>

#endif  // MANUX_CONFIG
#endif  // MANUX_FICHIER_CONFIG
