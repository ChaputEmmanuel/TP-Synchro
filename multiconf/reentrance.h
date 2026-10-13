/**
 * @file multiconf/reentrance.h
 * @brief Comparaison noyau réentrant vs non réentrant
 *
 *                                                           (C) Manu Chaput 2025
 */

#ifdef MANUX_FICHIER_CONFIG
#   error "Cette chose ne devrait pas se produire !"
#   include MANUX_FICHIER_CONFIG
#else
#ifndef MANUX_CONFIG
#define MANUX_CONFIG

#define MANUX_FICHIER_MAIN main-reentrance
#define MANUX_USR_INIT init-reentrance.o

#include <config/base.h>
#include <config/bootloader.h>       // Nécessaire pour construire mon bootloader
#include <config/pc-i386.h>
#include <config/appels-systeme.h>  // pour le mode utilisateur
#include <config/synchronisation.h>
#include <config/console.h>
#include <config/printk.h>
#include <config/taches.h>
#include <config/gestion-memoire.h>
#include <config/kmalloc.h>
#include <config/stdlib.h>
#include <config/usr.h>

#include <config/verifications.h>

#endif  // MANUX_CONFIG
#endif  // MANUX_FICHIER_CONFIG
