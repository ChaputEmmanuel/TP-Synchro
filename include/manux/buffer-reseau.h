/**
 * @file include/manux/buffer-reseau.h
 * @brief Définition des buffers réseau et les fonctions associcées
 *
 *                                                             (C) Manu Chaput 2025-2026
 */
#ifndef BUFFER_RESEAU
#define BUFFER_RESEAU

#include <manux/interface-reseau.h>

/**
 * @brief Défnition d'un buffer réseau
 */
typedef struct _bufferReseau {
   void * donnees;
   int taille;
   ReseauInterface * interfaceReception;  // Sur quelle interface
                                          // l'a-t-on reçu (ou NULL) 
} BufferReseau;

/**
 * @brief Un liste de buffers réseau
 *
 */
typedef struct _ListeBufferReseau ListeBufferReseau;

/**
 * @brief Création (allocation et initialisation des champs de base)
 * d'un buffer réseau.
 */
BufferReseau * bufferReseauCreer(void * donnees, int taille);

/**
 * @brief Création d'une liste vide de buffers réseau
 */
ListeBufferReseau * listeBufferReseauCreer(void);

/**
 * @brief Une liste de buffers est-elle vide ?
 */
int listeBufferReseauVide(ListeBufferReseau * l);

/**
 * @brief Insertion d'un buffer réseau à la fin d'une liste
 */
int listeBufferReseauInserer(ListeBufferReseau * l, BufferReseau * b);

/**
 * @brief Extraction d'un buffer réseau d'une liste
 *
 * NULL si liste inexistante ou vide
 */
BufferReseau * listeBufferReseauExtraire(ListeBufferReseau * l);

/**
 * @brief Affichage de la liste de buffers
 */
void listeBufferReseauAfficher(ListeBufferReseau * l);

#endif
