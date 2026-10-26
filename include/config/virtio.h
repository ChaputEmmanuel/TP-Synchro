/**
 * @file include/manux/virtion.h
 * @brief Définition des pilotes de périphériques de type virtio.
 *
 *                                                     (C) Manu Chaput 2025-2026
 */
#define MANUX_VIRTIO

/**
 * @brief Fait-on du réseau virtio ?
 */
#define MANUX_VIRTIO_NET

/**
 * @brief Fait-on une console virtio ?
 */
#define MANUX_VIRTIO_CONSOLE

/**
 * @brief Le numéro majeur des consoles virtio
 */
#define MANUX_VIRTIO_CONSOLE_MAJEUR 1

/**
 * @brief Implantation de fonctions d'audit des virtio
 */
#define MANUX_VIRTIO_AUDIT
