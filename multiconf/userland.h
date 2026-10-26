/**
 * @file multiconf/userland.h
 * @brief Exemple de configuration utilisant le mode utilisateur
 *
 *                                                     (C) Manu Chaput 2026-2026
 */

#ifdef MANUX_FICHIER_CONFIG
#   error "Cette chose ne devrait pas se produire !"
#   include MANUX_FICHIER_CONFIG
#else
#ifndef MANUX_CONFIG
#define MANUX_CONFIG

#define MANUX_FICHIER_MAIN main-userland
#define MANUX_USR_INIT init-userland.o

#include <config/plan-memoire-pc.h>   // Obligatoire pour le linker
#include <config/base.h>  // Définitions de base
#include <config/console.h>  // Pour afficher des choses à l'écran
#include <config/usr.h>   // On veut un userland

#include <config/verifications.h>

#endif  // MANUX_CONFIG
#endif  // MANUX_FICHIER_CONFIG
