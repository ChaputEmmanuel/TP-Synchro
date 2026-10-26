/**
 * @file virtio-console.h
 * @brief Définition des éléments de base des consoles virtio
 *
 *                                                     (C) Manu Chaput 2025-2026
 */
#ifndef VIRTIO_CONSOLE_DEF
#define VIRTIO_CONSOLE_DEF

#include <manux/virtio.h>
#include <manux/periph-car.h>

/**
 * Les features négociables
 */
#define VIRTIO_CONSOLE_F_SIZE      0x01
#define VIRTIO_CONSOLE_F_MULTIPORT 0x02

/**
 * Les numéros des files. Pour le moment, on n'en gère qu'un !
 */
#define VIRTIO_CONSOLE_PORT0_IN   0
#define VIRTIO_CONSOLE_PORT0_OUT  1

/**
 * @brief Initialisation des périphériques
 */
int virtioConsoleInitialisation(INoeud * iNoeudVirtioConsole);

#ifdef MANUX_VIRTIO_AUDIT
/**
 * @brief Affichage de la seule console, à ds fins de debug
 */
void virtioConsoleAfficher(void);

#endif // MANUX_VIRTIO_AUDIT

#endif
