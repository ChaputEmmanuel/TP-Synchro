/**
 * @file reseau/reseau.c
 * @brief Implantation des éléments principaux du sous système réseau
 *
 *                                                                  (C) Manu Chaput 2025
 */
#ifndef MANUX_RESEAU_INTERFACE
#define MANUX_RESEAU_INTERFACE

/**
 * @brief Description d'un pilote d'interface réseau
 */
typedef struct _ReseauPilote {
   char * nom;
   void (* recevoirTrame)(void *); // La fonction permettant d'aller
			           // récupérer des trames de façon
			           // asynchrone 
} ReseauPilote;

typedef struct _ReseauInterface {
   ReseauPilote * pilote;
   void * prive;
} ReseauInterface;

/**
 * @brief
 */
void reseauEnregistrerPilote(ReseauPilote * p);

/**
 * @brief
 */
void reseauAjouterInterface(ReseauPilote * p, void * d);


#endif
