/*----------------------------------------------------------------------------*/
/*      Définition des éléments de configuration de ManuX. Les options et     */
/* valeurs définies ici sont utilisées dans le code C mais également parfois  */
/* ailleurs. Le Makefile génère pour cela un fichier make.conf qui est ensuite*/
/* inclus.                                                                    */
/*                                                                            */
/*    Pour le bon fonctionnement de cette procédure, il est impératif que les */
/* macros en question débutent par le préfixe MANUX_                          */
/*                                                                            */
/*                                                  (C) Manu Chaput 2000-2023 */
/*----------------------------------------------------------------------------*/

#ifdef MANUX_FICHIER_CONFIG
#   include MANUX_FICHIER_CONFIG
#else
#ifndef MANUX_CONFIG
#define MANUX_CONFIG

#include <config/base.h>
#include <config/plan-memoire.h>
#include <config/bootloader.h>       // Nécessaire pour construire mon bootloader
#include <config/systeme-fichiers.h>
#include <config/ramdisk.h>
#include <config/pc-i386.h>
#include <config/appels-systeme.h>
#include <config/consoles-virtuelles.h>
#include <config/printk.h>
#include <config/journal.h>
#include <config/noyau.h>
#include <config/taches.h>
#include <config/gestion-memoire.h>
#include <config/kmalloc.h>
#include <config/clavier.h>
#include <config/reseau.h>
#include <config/pci.h>
#include <config/virtio.h>
#include <config/stdlib.h>
#include <config/usr.h>
#include <config/verifications.h>

#endif  // MANUX_CONFIG
#endif  // MANUX_FICHIER_CONFIG
