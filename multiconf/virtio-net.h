/**
 * @file multiconf/virtio-net.h
 * @brief Utilisation d'une interface réseau de type virtio
 *
 *                                                     (C) Manu Chaput 2000-2026
 */

#ifdef MANUX_FICHIER_CONFIG
#   error "Cette chose ne devrait pas se produire !"
#   include MANUX_FICHIER_CONFIG
#else
#ifndef MANUX_CONFIG
#define MANUX_CONFIG

#define MANUX_FICHIER_MAIN main-virtio-net

#include <config/base.h>
#include <config/bootloader.h>       // Nécessaire pour construire mon bootloader
#include <config/clavier.h>
#include <config/gestion-memoire.h>
#include <config/kmalloc.h>
#include <config/pc-i386.h>
#include <config/synchronisation.h>
#include <config/console.h>
#include <config/consoles-virtuelles.h>
#include <config/printk.h>
#include <config/taches.h>
#include <config/stdlib.h>
#include <config/reseau.h>
#include <config/pci.h>
#include <config/virtio.h>
#undef MANUX_VIRTIO_CONSOLE
#include <config/verifications.h>

#endif  // MANUX_CONFIG
#endif  // MANUX_FICHIER_CONFIG
