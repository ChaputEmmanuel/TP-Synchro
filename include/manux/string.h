/*----------------------------------------------------------------------------*/
/*      Définition des fonctions de manipulation des chaines.                 */
/*                                                                            */
/*                                                  (C) Manu Chaput 2000-2024 */
/*----------------------------------------------------------------------------*/
#ifndef DEF_MANUX_STRING
#define DEF_MANUX_STRING

#include <manux/types.h>

int strlen(const char * s);

void * memcpy(void *dest, const void *src, size_t n);

void * memset(void *dest, int val, size_t n);

void bcopy (const void *src, void *dest, int n);
/*
 * Copie de n octets depuis src vers dest.
 */

int strncmp(const char *s1, const char *s2, size_t n);

int strcmp(const char *s1, const char *s2);

/**
 * @brief Recherche d'un délimiteur dans une chaîne
 * 
 * renvoie un pointeur dans la chaine sur le premier délimiteur ou un
 * pointeur sur le 0 final.
 * Contrairement à strtok, la chaîne n'est pas modifiée, pas de
 * problème de réentrance.
 */
char * prochainDelimiteur(char * chaine, char delimiteur);

#endif
