/**
 * @file printk.c
 * @bried Implantion des fonctions de base d'entrée-sortie.                     
 *                                                                            
 *                                                     (C) Manu Chaput 2000-2026 
 *                                                                            */
#include <manux/kmalloc-zs.h>
#include <manux/scheduler.h>
#include <manux/printk.h>

#ifdef MANUX_JOURNAL
#   include <manux/journal.h>
#else
#   include <manux/console.h>
#endif
#include <manux/appelsysteme.h>
#include <manux/memoire.h>       /* NULL */

#include <manux/debug.h>

#include <manux/arith64.h>
//#include "../i386/arith64.c"   // WARNING, pourquoi dois-je faire ça ?

#define chiffre "0123456789abcdef"

/**
 * @brief : Écriture formattée dans une chaîne de caractères
 * @param  str : chaîne (allouée) dans laquelle l'écriture est faite
 * @param l : capacité de la chaîne str
 * @param format : le format d'affichage
 * @argList : les arguments de l'affichage
 *
 * C'est cette  fonction qui fait tout le travail. Elle écrit dans une
 * chaîne de caractères (allouée par l'appelant) en prenant garde de
 * ne pas dépasser, quitte à ne pas tout écrire.
 *
 * WARNING : elle a un clone dans userland, ...
 */
static
int vsnprintk(char * str, const size_t l, char * format, va_list argList)
{
   size_t    indice = 0;
   //   long long int       n;   // valeur associée à un %[l]d
   long int       n;   // valeur associée à un %[l]d
   char      nombre[10];    // chaîne du nombre
   char    * s;             // valeur associée à un %s
   char      c;             // Affichage d'un caractère
   int       in;            // indice pour les boucles internes
   int       nbChiffres;    // pour les %[n]d
   int       base;          // de l'affichage entier
   int       prefixe;       // 0 int, 1 long int, 2 long long int
   
   while ((*format) && (indice < l-1)) {
      switch (*format) {
         case '%' :
            format++;
            // Lecture de la taille
            nbChiffres = 0;
            while ((*format <= '9') && (*format >= '0')) {
               nbChiffres = nbChiffres * 10 + *format - '0';
               format++;
	    }

	    // Pas forcément bien là, mais j'ai pas mieux dans l'immédiat
	    prefixe = 0;
	    if (*format == 'l') {
               format++;
	       if (*format == 'l') {
                  format++;
 		  prefixe = 2;
	       } else {
 		  prefixe = 1;
	       }
	    }
            switch (*format) {
	       case 'o' :
                  base = 8;
                  goto affent;
	       case 'x' :
                  base = 16;
                  goto affent;
	       case 'd' :
                  base = 10;
affent :          switch (prefixe) {
                     case 2 :
		       n = 0; //;va_arg(argList, long long int); WARNING
		     break;
                     case 1 :
		        n = va_arg(argList, long int);
		     break;
                     default :
		        n = va_arg(argList, int);
                     break;
		  }
                  if (n < 0) {
		     n = -n;
		     str[indice++] = '-';
		  }
                  in = 0;
                  do {
                     nombre[in++] = chiffre[n%base];
                     n = n/base;   // n est défini comme long long, 
		  } while (n != 0);
                  while (nbChiffres > in) {
                     nbChiffres--;
                     str[indice++] = ' ';
		  }
                  do {
                     str[indice++] = nombre[--in];
                  } while (in);
               break; 
               case 's' :
                  s = va_arg(argList, char *);
                  in = 0;
                  while (s[in]) {
                     str[indice++] = s[in++];
		  }
               break;
               case 'c' :
                  c = (char)va_arg(argList, int);
                  in = 0;
                  str[indice++] = c;
               break;
               default :
               break; 
	    }
         break;
	 default :
            str[indice++] = *format;
         break;
      }
      format++;
   }

   assert(indice < l);
   
   str[(indice < l)?indice:(l-1)] = 0;

   return indice;
}

/**
 * @brief : Écriture formattée dans une chaîne de caractères
 *
 * C'est cette fonction qui doit être utilisée pour écrire dans une
 * chaîne de caractères
 */
int snprintk(char * str, size_t l, char * format, ...)
{
   va_list   argList;
   int       result;

   va_start(argList, format);
   result = vsnprintk(str, l, format, argList);
   va_end(argList);

   return result;
}

/**
 * @brief La fonction d'affichage à utiliser dans le noyau
 *
 * Elle utilise le journal s'il y en a un, et sinon la console du
 * noyau.
 */
void printk(char * format, ...)
{
   va_list   argList;
#ifdef MANUX_KMALLOC_NON   // WARNING : Ca ne marche pas (kmalloc pas initialisé)
   char    * chaine = kmalloc(MAX_PRINTK_LENGTH);
#else
   char      chaine[MAX_PRINTK_LENGTH];   // WARNING, il faut une gestion dynamique
                            // attention aux risques de telescopage avec la pile !
#endif // MANUX_KMALLOC   
   [[maybe_unused]] int result;

   va_start(argList, format);
   result = vsnprintk(chaine, MAX_PRINTK_LENGTH, format, argList);
   va_end(argList);

   // On affiche 
#ifdef MANUX_JOURNAL
   journaliser(chaine);
#else
   consoleNoyauAfficher(chaine);
#endif

#ifdef MANUX_KMALLOC_NON
   kfree(chaine);
#endif
}

/**
 * @brief Un fonction d'affichage pour écrire dans la console de la
 * tâche en cours
 *
 * Je n'en suis pas très fier, mais j'ai besoin d'un truc comme ça
 * pour les tâches purement noyau comme dummyTask.
 */
#ifdef MANUX_CONSOLES_VIRTUELLES
void printkc(char * format, ...)
{
   va_list   argList;
#ifdef MANUX_KMALLOC_NON   // WARNING : Ca ne marche pas (kmalloc pas initialisé)
   char    * chaine = kmalloc(MAX_PRINTK_LENGTH);
#else
   char      chaine[MAX_PRINTK_LENGTH];   // WARNING, il faut une gestion dynamique
                            // attention aux risques de telescopage avec la pile !
#endif // MANUX_KMALLOC   
   [[maybe_unused]] int result;

   va_start(argList, format);
   result = vsnprintk(chaine, MAX_PRINTK_LENGTH, format, argList);
   va_end(argList);

   // On affiche 
   consoleAfficher(tacheEnCours->console, chaine);

#ifdef MANUX_KMALLOC_NON
   kfree(chaine);
#endif
}
#endif
