/**
 *                                                  (C) Manu Chaput 2020-2025
 */
#include <manux/stdlib.h>

static uint32_t valeurAleatoire = 4832;
uint32_t factMult = 742938285;

/**
 * @brief Génération d'un nombre "aléatoire" entre 0 et RAND_MAX 
 */
uint32_t rand(void)
{
   valeurAleatoire = (valeurAleatoire * factMult) % 2147483647;
   return valeurAleatoire % (RAND_MAX + 1);
}

/**
 * @brief Modification de la graine du générateur "aléatoire"
 */
void srand(uint32_t graine)
{
   valeurAleatoire = graine;
}

int atoi(char * ch)
{
   int i = 0, result = 0;

   while ((ch[i] >= '0') && (ch[i] <= '9')) {
      result = 10 * result + ch[i++] - '0';
   }
   return result;
}

int atoihex(char * ch)
{
   int i = 2, result = 0;

   if ((ch[0] == '0') && (ch[1] == 'x')) {
     
      while (   ((ch[i] >= '0') && (ch[i] <= '9'))
	    || ((ch[i] >= 'A') && (ch[i] <= 'F')) 
            || ((ch[i] >= 'a') && (ch[i] <= 'f'))) {
         if ((ch[i] >= '0') && (ch[i] <= '9')) {
            result = 16 * result + ch[i] - '0';
	 } else if ((ch[i] >= 'A') && (ch[i] <= 'F')) {
            result = 16 * result + 10 + ch[i] - 'A';
	 } else {
	   result = 16 * result + 10 + ch[i] - 'a';
	 }
         i++;
      }
   }
   return result;
}


