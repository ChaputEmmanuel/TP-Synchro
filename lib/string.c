/*----------------------------------------------------------------------------*/
/*      Implantation des fonctions de manipulation des chaines.               */
/*                                                                            */
/*                                                  (C) Manu Chaput 2000-2021 */
/*----------------------------------------------------------------------------*/
#include "manux/stddef.h"
#include <manux/string.h>

/**
 * WARNING à refaire en assembleur
 */
void * memcpy(void *dest, const void *src, size_t n)
{
   char * d = dest;
   const char * s = src;
   while (n--)
      *d++ = *s++; 

   return dest;
}

/**
 * WARNING à refaire en assembleur
 */
void * memset(void *dest, int val, size_t n)
{
   char * d = dest;
   while (n--)
      *d++ = val; 

   return dest;
}


void bcopy (const void *src, void *dest, int n)
/*
 * bcopy/bzero ne sont ni ISO C ni POSIX (deprecated depuis 2001 et removed en 2008)
 */
{
   char * d = dest;
   const char * s = src;
   while (n--)
      *d++ = *s++; 
}

int strlen(const char * s)
{
   int result = 0;
   
   while (s[result]) {
      result++;
   }
   
   return result;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
   size_t i;

   // On cherche la première différence
   for (i = 0 ; (i < n) && (s1[i] == s2[i]) ; i++) { }

   // Si on n'en a pas trouvé, c'est égalité, sinon on compare
   if (i == n) {
      return 0;
   } else  if (s1[i] < s2[i]) {
      return -1;
   } else {
     return +1;
   }
}

int strcmp(const char *s1, const char *s2)
{
   int l1 = strlen(s1);
   int l2 = strlen(s2);
   int l = l1;
   int result;

   if (l2 < l) {
      l = l2;
   }

   result = strncmp(s1, s2, l);

   // Si elles sont égales sur la longueur commune
   if (result == 0) {
      if (l1 < l2) {
         return -1;
      } else if (l2 < l1) {
 	return +1;
      }
   }
   return result;
}

/**
 * @brief Recherche d'un délimiteur dans une chaîne
 * 
 * renvoie un pointeur dans la chaine sur le premier délimiteur ou un
 * pointeur sur le 0 final.
 */
char * prochainDelimiteur(char * chaine, char delimiteur)
{
   char * result = chaine;

   while ((*result != 0) && (*result != delimiteur)) {
      result ++;
   }
   return result;
}
