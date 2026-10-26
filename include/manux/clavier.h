/*----------------------------------------------------------------------------*/
/* Définition des sous-programmes permettant de manipuler le clavier.         */
/*                                                                            */
/*                                                  (C) Manu Chaput 2000-2021 */
/*----------------------------------------------------------------------------*/
#ifndef CLAVIER_DEF
#define CLAVIER_DEF

extern int toucheTouche;

#define portDonneesClavier 0x60
#define portCmdClavier     0x64

void initialiserClavier(void);
/*
 * Initialisation du clavier, à appeler avant toute utilisation de ce dernier.
 */

void handlerClavier(void * toto);
/*
 * Le handler de l'interruption clavier
 */

#endif
