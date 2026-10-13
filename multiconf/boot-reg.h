/**
 * @file multiconf/boot-reg.h
 * @brief Ajout dans le rergistre systèmes de paramètres fournis au boot
 *
 *                                                          (C) Manu Chaput 2025
 */

#ifdef MANUX_FICHIER_CONFIG
#   error "Cette chose ne devrait pas se produire !"
#   include MANUX_FICHIER_CONFIG
#else
#ifndef MANUX_CONFIG
#define MANUX_CONFIG

#define MANUX_FICHIER_MAIN main-bootreg

#include <config/base.h>
#include <config/console.h>
#include <config/printk.h>
#include <config/pc-i386.h>
#include <config/stdlib.h>  // atoi
#include <config/plan-memoire-pc.h>
#include <config/gestion-memoire.h>  // Pourquoi ?
#include <config/kmalloc.h> 
#include <config/bootloader.h> // On va lire des paramètres fournis par le bootloader
#include <config/registre.h>

#include <config/verifications.h>

#endif  // MANUX_CONFIG
#endif  // MANUX_FICHIER_CONFIG
