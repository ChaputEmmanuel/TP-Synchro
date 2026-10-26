/**
 * @file multiconf/debug-dyn.h
 * @brief Gestion dynamique du masque de debugage de ManuX.
 *
 *                                                     (C) Manu Chaput 2023-2025
 */

#ifdef MANUX_FICHIER_CONFIG
#   error "Cette chose ne devrait pas se produire !"
#   include MANUX_FICHIER_CONFIG
#else
#ifndef MANUX_CONFIG
#define MANUX_CONFIG

#define MANUX_FICHIER_MAIN main-debmaskdyn

#include <config/base.h>
#include <config/console.h>
#include <config/printk.h>
#include <config/debug.h>
#include <config/stdlib.h>   // Pour atoihex
#include <config/pc-i386.h>
#include <config/gestion-memoire.h>  // Pourquoi ?
#include <config/bootloader.h> // On va lire des paramètres fournis par le bootloader

#include <config/verifications.h>

#endif  // MANUX_CONFIG
#endif  // MANUX_FICHIER_CONFIG
