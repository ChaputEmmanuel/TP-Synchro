/**
 * @file ecran.h
 * @brief Définition des caractéristiques de base de l'écran
 *
 * Attention, il s'agit ici de l'écran d'un bon vieux PC.
 *
 *                                                   (C) Manu Chaput 2000 - 2026
 */

#ifndef MANUX_ECRAN_DEF
#define MANUX_ECRAN_DEF

#include <manux/config.h>
#include <manux/types.h>
#include <manux/ascii.h>

/**
 * A voir où cette adresse doit être définie
 */
#ifndef MANUX_ADRESSE_ECRAN
#   define MANUX_ADRESSE_ECRAN 0xb8000
#endif

/**
 * Caractéristiques de l'écran physique
 */
#define MANUX_CON_SCREEN   (char *)MANUX_ADRESSE_ECRAN
#define MANUX_CON_COLONNES 80
#define MANUX_CON_LIGNES   25

/**
 * @brief Définition des couleurs utilisables pour l'affichage
 *
 * Ce sont les couleurs définies par le BIOS
 */
typedef enum {
   COUL_TXT_NOIR             = 0x00,
   COUL_TXT_BLEU             = 0x01,
   COUL_TXT_VERT             = 0x02,
   COUL_TXT_CYAN             = 0x03,
   COUL_TXT_ROUGE            = 0x04,
   COUL_TXT_MAGENTA          = 0x05,
   COUL_TXT_MARRON           = 0x06,
   COUL_TXT_GRIS_CLAIR       = 0x07,
   COUL_TXT_GRIS             = 0x08,
   COUL_TXT_BLEU_CLAIR       = 0x09,
   COUL_TXT_VERT_CLAIR       = 0x0A,
   COUL_TXT_CYAN_CLAIR       = 0x0B,
   COUL_TXT_ROUGE_CLAIR      = 0x0C,
   COUL_TXT_MAGENTA_CLAIR    = 0x0D,
   COUL_TXT_JAUNE            = 0x0E,
   COUL_TXT_BLANC            = 0x0F,
   COUL_FOND_NOIR            = 0x00,
   COUL_FOND_BLEU            = 0x10,
   COUL_FOND_VERT            = 0x20,
   COUL_FOND_CYAN            = 0x30,
   COUL_FOND_ROUGE           = 0x40,
   COUL_FOND_MAGENTA         = 0x50,
   COUL_FOND_MARRON          = 0x60,
   COUL_FOND_GRIS_CLAIR      = 0x70,
   COUL_FOND_GRIS            = 0x80,
   COUL_FOND_BLEU_CLAIR      = 0x90,
   COUL_FOND_VERT_CLAIR      = 0xA0,
   COUL_FOND_CYAN_CLAIR      = 0xB0,
   COUL_FOND_ROUGE_CLAIR     = 0xC0,
   COUL_FOND_MAGENTA_CLAIR   = 0xD0,
   COUL_FOND_JAUNE           = 0xE0,
   COUL_FOND_BLANC           = 0xF0,
   COUL_CLIGNOTANT           = 0x80
} Couleur;

/**
 * @brief Les couleurs utilisées par défaut pour l'écran ManuX
 *
 * WARNING : on devrait probablement le mettre ailleurs (console.h ?)
 */
#ifndef MANUX_COULEUR_TEXTE
#   define MANUX_COULEUR_TEXTE COUL_TXT_VERT
#endif
#ifndef MANUX_COULEUR_FOND
#   define MANUX_COULEUR_FOND COUL_FOND_MARRON
#endif

#endif
