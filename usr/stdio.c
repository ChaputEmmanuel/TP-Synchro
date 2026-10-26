/*----------------------------------------------------------------------------*/
/*      Implantion des fonctions de base d'entrée-sortie du mode utilisateur. */
/*                                                                            */
/*                                                  (C) Manu Chaput 2002-2021 */
/*----------------------------------------------------------------------------*/
#include <manux/config.h>     // Suivant la config on passe par fs ou pas
#include <manux/appelsysteme.h>
#include <stdio.h>

/**
 * WARNING : On utilise ici cette fonction définie dans le noyau, il
 * faudra tôt ou tard changer ça, ...
 */
int vsnprintk(char * str, const size_t l, char * format, va_list argList);

#define MAX_PRINTF_LENGTH 64

#define chiffre "0123456789abcdef"

appelSysteme3(NBAS_ECRIRE, int, ecrire, int, void *, int);

appelSysteme3(NBAS_LIRE, int, lire, int, void *, int);

appelSysteme1(NBAS_FERMER, int, fermer, int);


/*
 * ecrireConsole est un appel système. Son "implantation" côté
 * utilisateur passe donc par une macro.
 */
appelSysteme2(NBAS_ECRIRE_CONS, int, ecrireConsole, char *, int);

/**
 * @brief Le formatage effectf
 *
 * WARNING : attention, c'est un clone de vsnprintk, ...
 */
int vsnprintf(char * str, const size_t l, char * format, va_list argList)
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

   str[(indice < l)?indice:(l-1)] = 0;

   return indice;
}

/**
 * @brief Un premier printf
 *
 */
void printf(char * format, ...)
{
   va_list argList;
   char      chaine[MAX_PRINTF_LENGTH];   // WARNING, il faut une gestion dynamique
                            // attention aux risques de telescopage avec la pile !
   int       result;

   va_start(argList, format);
   result = vsnprintf(chaine, MAX_PRINTF_LENGTH, format, argList);
   va_end(argList);

#ifdef MANU_FS
   ecrire(1, chaine, result); // WARNING : 1 à remplacer par stdout par exemple
#else
   // C'est exactement le but de l'AS ecrireConsole
   ecrireConsole(chaine, result); 
#endif

   va_end(argList);
}

