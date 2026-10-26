/**
 * @file stdlib.h
 * @brief Définition de quelques fonctions de base
 *                                                  (C) Manu Chaput 2020-2023 */
#ifndef MANUX_DEF_STDLIB
#define MANUX_DEF_STDLIB

#include <manux/types.h>

#define RAND_MAX 32767

/**
 * @brief Génération d'un nombre "aléatoire" entre 0 et RAND_MAX 
 */
uint32_t rand(void);

/**
 * @brief Modification de la graine du générateur "aléatoire"
 */
void srand(uint32_t graine);

/**
 * @brief Conversion d'une chaîne en entier
 *
 * Le début de la chaîne est converti en entier. 0 en cas d'erreur
 */
int atoi(char * ch);

/**
 * @brief Version hexadécimale de atoi()
 *
 * La chaîne doit impérativement commencer par 0x
 */
int atoihex(char * ch);

#endif // MANUX_DEF_STDLIB
