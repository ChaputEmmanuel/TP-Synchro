/**
 * @file multiconf/registre.h
 * @brief Gestion dynamique des paramètres de ManuX dans un registre
 *
 *                                                     (C) Manu Chaput 2023-2024
 */

#ifdef MANUX_FICHIER_CONFIG
#   error "Cette chose ne devrait pas se produire !"
#   include MANUX_FICHIER_CONFIG
#else
#ifndef MANUX_CONFIG
#define MANUX_CONFIG

#define MANUX_FICHIER_MAIN main-registre

#include <config/base.h>
#include <config/console.h>
#include <config/printk.h>
#include <config/pc-i386.h>
#include <config/stdlib.h>           // atoi
#include <config/bootloader.h>       // Pour infosys pour mémoire
#include <config/gestion-memoire.h>  // Pour kmalloc
#include <config/kmalloc.h>          // Le registre utilise kmalloc
#include <config/registre.h>

#include <config/verifications.h>

#endif  // MANUX_CONFIG
#endif  // MANUX_FICHIER_CONFIG
