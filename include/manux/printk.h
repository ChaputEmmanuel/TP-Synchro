/**
 * @file printk.h
 * @brief Définition des fonctions de base d'entrée-sortie du noyau.            
 *                                                                            
 *                                                  (C) Manu Chaput 2000-2023 
 *                                                                            */
#ifndef STD_IO_DEF
#define STD_IO_DEF

#include <manux/config.h>
#include <manux/stdarg.h>
#include <manux/stddef.h>   // size_t
#ifdef MANUX_FICHIER
#include <manux/fichier.h>
#endif

/**
 * Longueur maximale d'une chaîne affichable (à supprimer dès qu'on
 * aura de la mémoire dynamique)
 */
#define MAX_PRINTK_LENGTH 2048 //512

/**
 * @brief Les différentes préfixes pour les niveaux de criticité
 */
#define PRINTK_PANIQUE      "{0}"
#define PRINTK_URGENCE      "{1}"
#define PRINTK_CRITIQUE     "{2}"
#define PRINTK_ERREUR       "{3}"
#define PRINTK_ATTENTION    "{4}"
#define PRINTK_NOTIFICATION "{5}"
#define PRINTK_INFORMATION  "{6}"
#define PRINTK_DEBUGAGE     "{7}"

/**
 * @brief Fonction principale d'affichage dans le noyau
 *
 * Un grand classique. Elle construit une chaîne de caractères puis
 * l'envoie au journal. Si le journal n'est pas actif (MANUX_JOURNAL
 * non défini) alors la chaîne est envoyée sur la console spécifique
 * au noyau.
 *
 * Les formats gérés pour le moment :
 *
 *    %[n][l[l]]{dxo} %s \n
 */
void printk(char * format, ...);

/**
 * @brief Une version de printk qui écrit dans la console de la tâche
 * en cours
 *
 * C'est un peu un printf en monde noyau, ...
 */
#ifdef MANUX_CONSOLES_VIRTUELLES
void printkc(char * format, ...);

// Si on n'a pas de console virtuelle, printkc ne peut pas faire mieux
// que printk
#else
#   define printkc printk
#endif

/**
 * @brief : Écriture formattée dans une chaîne de caractères
 */
int sprintk(char * str, char * format, ...);

/**
 * @brief : Écriture formattée dans une chaîne de caractères
 */
int vsprintk(char * str, char * format, va_list argList);

/**
 * @brief : Écriture formattée dans une chaîne de caractères
 *
 * C'est cette fonction qui doit être utilisée pour écrire dans une
 * chaîne de caractères
 */
int snprintk(char * str, size_t l, char * format, ...);

#ifdef MANUX_FICHIER
#define fprintk(f, format, ...) \
  {	   \
   char chaine[MAX_PRINTK_LENGTH]; \
   fichierEcrire(f, chaine, snprintk(chaine, MAX_PRINTK_LENGTH, format, __VA_ARGS__));	\
}
#endif // MANUX_FICHIER
#endif
