/**
 * @file : main-printk.c  
 * @brief Exemple d'utilisation de printk et de certains codes ANSI
 *
 *                                                     (C) Manu Chaput 2000-2026
 */
#include "manux/ascii.h"
#include "manux/ecran.h"
#include <manux/config.h>
#include <manux/console.h>
#include <manux/printk.h>

void startManuX()
{
  uint8_t fond, texte, l, c;

   // Initialisation de la console noyau
   consoleInitialisation();

   // Un petit message
   printk("Printk dit bonjour !\n\n");

   printk("Voici les couleurs utilisables pour le fond (colonne) et le texte (ligne)");
   
   // On affiche un tableau des différentes couleurs
   // 1 - La première ligne des couleurs de fond
   c = 4;
   for (fond = 40; fond <= 107; fond ++) {
      printk("%c[%d;%dH%3d", ASCII_ESC, 4, c, fond); // goto ligne colonne
      if (fond == 47) {fond = 99;} // J'ai honte !
      c+=4;
   }

   // 2 - La colonne des couleurs de texte
   l = 5;
   for (texte = 30; texte <= 97; texte ++){
      printk("%c[%d;%dH%2d", ASCII_ESC, l, 1, texte);   // goto ligne colonne
      if (texte == 37) {texte = 89;} // J'ai honte !
      l++;
   }   

   l = 5;
   for (texte = 30; texte <= 97; texte ++){
      c = 4;
      for (fond = 40; fond <= 107; fond ++) {
         printk("%c[%d;%dH", ASCII_ESC, l, c);   // goto ligne colonne
         printk("%c[%dm", ASCII_ESC, texte);     // couleur de texte
         printk("%c[%dm", ASCII_ESC, fond);      // couleur de fond
	 printk(" XY ");
         if (fond == 47) {fond = 99;} // J'ai toujours honte !
	 c+=4;
      }
      if (texte == 37) {texte = 89;} // J'ai encore honte !
      l++;
   }
   
}   /* startManuX */


