/*----------------------------------------------------------------------------*/
/*      Définition des sous-programmes de manipulation de la mémoire sous     */
/*   ManuX.                                                                   */
/*                                                                            */
/*                                                  (C) Manu Chaput 2000-2021 */
/*----------------------------------------------------------------------------*/
#ifndef MEMOIRE_DEF
#define MEMOIRE_DEF

#include <manux/types.h>
#include <manux/stddef.h>    // NULL

#ifdef MANUX_AS
#   include <manux/appelsysteme.h> /* ParametreAS */
#endif

#define MANUX_DEBUT_MEMOIRE_ETENDUE 0x100000

/**
 * Combien de pages pour stocker n octets ?
 */
#define NB_PAGES(n) ((n + MANUX_TAILLE_PAGE - 1)/MANUX_TAILLE_PAGE)

/*
 * Nombre de pages communes à toutes les tâches.
 */
extern int nombrePagesSysteme;

/**
 * @brief Initialisation de la mémoire au niveau pages.
 *
 * Cette initialisation est nécessaire avant toute demande
 * d'allocation. C'est elle qui va permettre de mettre en place le
 * mécanisme d'allocation des pages dans le système.
 */
void initialiserMemoire(uint32_t tailleMemoireDeBase,
			uint32_t tailleMemoireEtendue);

void * allouerPage(void);
/*
 * Réservation d'une page (de 4 Ko)
 *
 * Retour
 *    première adresse dispo si la pages a pu être allouée,
 *    NULL sinon
 */

void * allouerPages(uint32_t nombre);
/*
 * Réservation d'un nombre choisi de pages (de 4 Ko) contigues.
 *
 * Retour
 *    première adresse dispo si les pages ont pu être allouées,
 *    NULL sinon
 * WARNING à mettre en appel système ?
 */

void libererPage(void * pageLiberee);
/*
 * Libération d'une page préalablement allouée. Attention, aucune
 * vérification n'est effectuée.
 */

int nombrePagesAllouees(void);

int nombrePagesTotal(void);

#ifdef MANUX_AS
int AS_obtenirPages(ParametreAS p, int nbPages);
/*
 * Demande d'accroissement de la zone mémoire disponible à
 * la tâche.
 * Retour : nombre de pages obtenues.
 */
#endif

#endif
