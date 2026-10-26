/**
 * @file config/debug.h
 * @brief Configuration du débugage de ManuX
 *
 *                                                      (C) E. Chaput 2022-2026
 */

/**
 * @brief Le débugage est-il paramétré à la compilation ou à l'exécution ?
 *
 * Si cette macro est définie, le masque définissant les messages de
 * débug affichés est stoqué dans une variable, sinon il est stoqué
 * dans une macro.
 */
#define MANUX_DEBUGMASK_VAR

/**
 * Si les masques sont définis dans des variables, elles peuvent être
 * modifiées via le bootloader, au travers du registre ou
 * directement. Les macros ci-dessous définissent le nom à affecter.
 */
#define NOM_MASQUE_CONSOLE "debug.masque.console"
#define NOM_MASQUE_FICHIER "debug.masque.fichier"


/**
 * @brief Doit-on activer les "assert" ?
 * Si cette macro n'est pas définie, les assert ne produisent aucun code.
 */
#define MANUX_ASSERT_ACTIVES
