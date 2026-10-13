/**
 * @file debug.c
 * @brief Les outils de débogage
 *
 *                                                     (C) Manu Chaput 2023-2025
 */
#include <manux/debug.h>
#include <manux/printk.h>    // snprintk
#include <manux/stdlib.h>    // atoihex
#include <manux/registre.h>

#ifdef MANUX_DEBUGMASK_VAR
uint32_t masqueDebugageConsole = _masqueDebugageConsole;
uint32_t masqueDebugageFichier = _masqueDebugageFichier;
#endif

/**
 * @brief Mise à jour d'un masque de debug via le registre
 */
static
void debugMiseAJourMasque(void * masque, char * valeur)
{
   uint32_t * _masque = (uint32_t *)masque;

   *_masque = atoihex(valeur);
}

/**
 * @brief Initialisation du sytème de débogage
 *
 *    Cette phase d'initialisation n'est utile que si les masques de
 * debogage sont implantés sous forme de variables (c'est-à-dire si
 * MANUX_DEBUGMASK_VAR est définie) et si le registre est configuré
 * (donc si MANUX_REGISTRE est définie).
 *    Elle sert alors à placer les paramètres dans le registre.
 */
void debugInitialiser(void)
{
#if defined(MANUX_DEBUGMASK_VAR) && defined(MANUX_REGISTRE)
   char v[32];

   snprintk(v, 32, "0x%x", _masqueDebugageConsole);
   registreSystemeAffecterParametre(v, &masqueDebugageConsole, debugMiseAJourMasque,
				    "debug", "masque", "console", NULL);
   snprintk(v, 32, "0x%x", _masqueDebugageConsole);
   registreSystemeAffecterParametre(v, &masqueDebugageFichier, debugMiseAJourMasque,
				    "debug", "masque", "fichier", NULL);

#endif
}
