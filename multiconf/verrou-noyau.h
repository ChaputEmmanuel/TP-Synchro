/**
 * @file multiconf/mutex.h
 * @brief Configuration permettant de mettre en évidence la (non)
 * ré-entrance du noyau.
 *
 *                                                     (C) Manu Chaput 2000-2025
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

#define MANUX_FICHIER_MAIN main-verrou-noyau
#define MANUX_USR_INIT init-verrou-noyau.o// init-acces-concurrent.o //

#include <config/synchronisation.h>
#include <config/base.h>
#include <config/bootloader.h>           // Nécessaire pour construire mon bootloader
#include <config/pc-i386.h>              // nécessaire pour les appels systeme
#include <config/appels-systeme.h>       // pour le mode utilisateur
#include <config/consoles-virtuelles.h>  // On va basculer
#include <config/synchronisation.h>      // Pour la synchro (verrou ...)

//#undef MANUX_REENTRANT

/*
#undef MANUX_ATOMIQUE_AUDIT
#undef MANUX_EXCLUSION_MUTUELLE_AUDIT
#undef MANUX_CONDITION_AUDIT
*/
#include <config/kmalloc.h>

#include <config/printk.h>
#include <config/taches.h>
#include <config/gestion-memoire.h>      // Nécessaire pour les tâches 
#include <config/clavier.h>
#include <config/usr.h>                  // Définition de init

#include <config/verifications.h>

#endif  // MANUX_CONFIG
#endif  // MANUX_FICHIER_CONFIG
