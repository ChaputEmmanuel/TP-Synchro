/**
 * @file include/manux/registre.h
 * @brief Gestion des registres (outils permettant de stocker des paramètres)
 *
 *   Attention en cas de mise-à-jour : faire suivre la doc !!!
 *
 *   Un registre est une structure arborescente contenant des
 * paramètres. Un registre doit d'abord être créé puis on peut y
 * insérer des paramètres avec la fonction
 * registreAffecterParametre(). Chaque paramètre est stoqué comme une
 * chaîne de caractères.
 *
 *   Il existe plusieurs façons de l'utiliser car il existe des
 * des besoins différents :
 * 
 *   . Le principe de base est que chaque sous-sytème crée les
 * paramètres dans un registre, avec pour chacun une fonction de
 * mise-à-jour (et un pointeur associé si c'est utile) et une valeur
 * par défaut s'il le souhaite
 *
 *   . Il peut être également utile de renseigner un registre avec des
 * valeurs de paramètres avant que ceux-ci aient été créés et insérés
 * dans le registre. Des paramètres peuvent par exemple être chargés
 * au démarage du système dont la valeur pourra être utilisée lors du
 * démarrage des sous-systèmes  correspondants. Lors de leur première
 * insertion dans le registre, la fonction de mise-à-jour ne peut donc
 * pas être invoquée.
 *
 *    La fonction d'affectation d'un paramètre dans un registre a donc
 *  la signature suivante
 *
 *  void registreAffecterParametre(registre * base,
 *                                 char * valeur,
 *                                 void * prive,
 *                                 registreMiseAJour miseAJour,
 *                                 ...);
 *
 * où 
 *  
 *  @param base : le registre dans lequel est le paramètre
 *  @param valeur : la valeur du paramètre sous forme de chaîne (ou
 *  NULL)
 *  @param prive : un pointeur sur une zone privée (ou NULL)
 *  @param miseAJour : la fonction de mise à jour (ou NULL)
 *  @param listargs : le nom sous forme de liste de chaînes
 *
 *  Le principe est le suivant
 *
 *  si miseAJour == NULL alors
 *     la valeur est affectée au paramètre
 *     si la fonction de mise-à-jour existe alors
 *        elle est invoquée (avec cette nouvelle valeur)
 *     finsi
 *  sinon
 *     si le paramètre n'avait pas de valeur définie, alors
 *        On lui affecte celle passée en paramètre
 *     fin si
 *     la fonction de mise-à-jour est affectée
 *     la fonction de mise-à-jour est invoquée
 *  finsi
 *
 *  Dit autrement, si une fonction de mise-à-jour est passée en
 * paramètre, alors on ne se préoccupe pas de la valeur passée en
 * paramètre, sinon on affecte la valeur (même nulle).
 *  Dans tous les cas, on invoque la màj si elle existe.
 *
 *                                                       (C) E. Chaput 2024-2025
 */

#ifndef REGISTRE_DEF
#define REGISTRE_DEF

#include <manux/stdarg.h>
#include <manux/types.h>   // uint32_t

/**
 * @brief Le type d'un registre
 */
typedef struct _registre registre;

/**
 * @brief Définition d'un parametre dans un registre
 */
typedef struct _parametre parametre;

/**
 * @brief La fonction de mise-à-jour d'un registre
 */
typedef void (* registreMiseAJour) (void *, char *) ;

/**
 * @brief Création d'un registre vide
 */
registre * registreCreer(char * nom);

/**
 * @brief Affecter un paramètre dans un registre
 *
 * Voir la doc générale du registre pour une description de l'algorithme
 */
void registreAffecterParametre(registre * reg,
                               char * valeur,
			       void * prive,
                               registreMiseAJour miseAJour,
                               ...);

/**
 * @brief Initialisation du registre système
 */
void registreSystemeInitialiser(void);

/**
 * @brief Ajouter un paramètre au registre système de ManuX
 */
void registreSystemeAffecterParametre(char * valeur,
			              void * prive,
                                      registreMiseAJour miseAJour,
	                              ...);

/**
 * @brief Ajouter dans le registre système un paramètre décrit dans une chaîne
 * de caractères.
 *
 * La chaîne doit être sous la forme "a.b.c=v"
 */
void registreSystemeAjouterC(char * chaine);

/**
 * @brief Affichage du registre de ManuX
 */
void registreSystemeAfficher(void);

#endif   // REGISTRE

