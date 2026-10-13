/**
 * @file lib/registre.c
 * @brief Mise en oeuvre de la notion de registre permettant de gérer
 * des paramètres de façon structurée.
 *
 *                                                       (C) E. Chaput 2024-2025
 */
#include <manux/registre.h>
#include <manux/string.h>    // strlen, memcpy, prochainDelimiteur
#include <manux/printk.h>
#include <manux/kmalloc.h>
#include <manux/errno.h>     // ENOENT
#include <manux/debug.h>     // assert

/**
 * @brief Un paramètre est simplement une valeur sous forme de chaîne
 * de caractères associée à un nom.
 */
typedef struct _parametre {
   char * nom;
   int    estUnRegistre; // Sous-registre (1) ou non (0)
   char * valeur;
   registreMiseAJour miseAJour; // Invoquée par le
                                // registre lors d'une
                                // modification du
                                // paramètre
   void * prive;    // Un pointeur à disposition de l'utilisateur
                    // Ce sera un pointeur vers un registre si
                    // estUnRegistre == 1
} parametre;

/**
 * @brief On va construire des listes de paramètres
 */
typedef struct _chainonParametre {
   parametre * parametre;
   struct _chainonParametre * suivant;
   struct _chainonParametre * precedent;   
} chainonParametre;

typedef struct _listeParametres {
   chainonParametre * premier;
   chainonParametre * dernier;   
} listeParametres;

/**
 * @brief Un registre est simplement une liste (nommée) de
 * paramètres. Les paramètres peuvent être eux-mêmes des registres
 */
typedef struct _registre {
   char * nom;
   listeParametres * parametres;
} registre;

static registre * registreSysteme = NULL;

/*----------------------------------------------------------------------------*/
/* Les paramètres                                                             */
/*----------------------------------------------------------------------------*/

/**
 * @brief Mise-à-jour d'un paramètre en invoquant la fonction définie
 */
static void parametreMettreAJour(parametre * p)
{
  if ((p != NULL) && (p->miseAJour != NULL)) {
      p->miseAJour(p->prive, p->valeur);
   }
}

/**
 * @brief Affectation de la valeur d'un paramètre
 */
static void parametreAffecterValeur(parametre * p, char * v)
{
   if (p->valeur != NULL) {
      kfree(p->valeur);
   }

   if (v != NULL) {
      p->valeur = kmalloc(strlen(v) +1);
      memcpy(p->valeur, v, strlen(v) +1);
   } else {
      p->valeur = NULL;
   }
   
   // On répercute la mise-à-jour
   parametreMettreAJour(p);
}

/**
 * @brief Affectation du nom d'un paramètre
 */
static void parametreAffecterNom(parametre * p, char * n)
{
   if (p->nom != NULL) {
      kfree(p->nom);
   }  
   p->nom = kmalloc(strlen(n) +1);
   memcpy(p->nom, n, strlen(n) +1);
}

/**
 * @brief Définition de la fonction de mise à jour du paramètre
 */
static void parametreAffecterMiseAJour(parametre * param,
                                void * prive,
				registreMiseAJour miseAJour)
{
   param->miseAJour = miseAJour;
   param->prive = prive;

   // On applique car il y a peut-être déjà une valeur
   parametreMettreAJour(param);
}

/**
 * @brief Création d'un paramètre
 */
static parametre * parametreCreer(char * nom, char * valeur,
			   void * prive, registreMiseAJour miseAJour)
{
   parametre * result = (parametre *) kmalloc(sizeof(parametre));

   if (result != NULL) {
      memset(result, 0, sizeof(parametre));
      parametreAffecterNom(result, nom);
      parametreAffecterValeur(result, valeur);
      parametreAffecterMiseAJour(result, prive, miseAJour);
   } 
   return result;
}

/*----------------------------------------------------------------------------*/
/* Les listes de paramètres.                                                  */
/*----------------------------------------------------------------------------*/

/**
 * @brief Création d'une liste (vide) de paramètres
 */
static listeParametres * listeParametresCreer(void)
{
   listeParametres * result = (listeParametres *)kmalloc(sizeof(listeParametres));

   if (result != NULL) {
      result->premier = NULL;
      result->dernier = NULL;
   }
   return result;
}

/**
 * @brief Insertion du paramètre p dans la liste lp
 *
 * Attention aucune vérification n'est faite, un doublon peut apparaître
 */
static void listeParametresInserer(listeParametres * lp, parametre * p)
{
   chainonParametre * cp = (chainonParametre *) kmalloc(sizeof(chainonParametre));

   //printk_debug(DBG_KERNEL_REGISTRE, "Insertion dans 0x%x ...\n", lp);
   if (cp != NULL) {
      cp->parametre = p;
      cp->suivant = lp->premier;
      cp->precedent = NULL;
      if (lp->premier != NULL){
         lp->premier->precedent = cp;
      }
      lp->premier = cp;
      //      printk_debug(DBG_KERNEL_REGISTRE, "  0x%x premier = 0x%x\n", lp, cp);
      if (lp->dernier == NULL) {
         lp->dernier = cp;
      }
   }
}

/**
 * @brief Recherche d'un paramètre dans une liste
 */
static parametre * listeParametresChercher(listeParametres * lp, char * nom)
{
   chainonParametre * cp = lp->premier;

   while ((cp != NULL) && (strcmp(nom, cp->parametre->nom))) {
      cp = cp-> suivant;
   }

   // On l'a trouvé ?
   if (cp != NULL) {
     return cp->parametre;
   }
   
   return NULL;
}
  
/*----------------------------------------------------------------------------*/
/* Les registres.                                                             */
/*----------------------------------------------------------------------------*/

/**
 * @brief Création d'un registre vide
 */
registre * registreCreer(char * nom)
{
   registre * result = (registre *)kmalloc(sizeof(registre));

   if (result != NULL) {
      result->nom = kmalloc(strlen(nom) +1);
      memcpy(result->nom, nom, strlen(nom) +1);
      result->parametres = listeParametresCreer();
   }
   
   return result;					 
}

/**
 * @brief Recherche d'un sous registre (direct) dans un registre. NULL si inexistant.
 */
static
registre * registreChercherSousRegistre(registre * b, char * nomSr)
{
   registre * result = NULL;
   chainonParametre * ch;

   if ((b != NULL) && (b->parametres != NULL)) {
      for (ch = b->parametres->premier ; (ch != NULL) && (result == NULL) ; ch = ch->suivant) {
         // Si on trouve une sous-base avec le bon nom, on la renvoie
         if (   (ch->parametre->estUnRegistre)
	     && (strcmp(nomSr, ch->parametre->nom) == 0)){
            return (registre *) ch->parametre->prive;
	 }
      }
   }
   
   return result;
}

/**
 * @brief Ajout d'un parametre dans un registre
 * 
 * Attention, aucune vérification n'est faite, si bien qu'un doublon
 * peut apparaître
 */
static
void registreAjouterParametre(registre * reg, parametre * param)
{
   listeParametresInserer(reg->parametres, param);
}

/**
 * @brief Création et ajout d'un sous registre fils dans un registre
 *
 * Un pointeur sur le registre ainsi créé est retourné. Si le sous
 * registre existait déjà, il n'est pas créé, mais le pointeur est
 * correctement retourné.
 */
static
registre * registreAjouterSousRegistre(registre * reg, char * nomSr)
{
   registre * result;
   parametre * sRParam;
   
   // Existe-t-il déjà ?
   result = registreChercherSousRegistre(reg, nomSr);

   // Si non, on le crée
   if (result == NULL) {
   
      // Création du (sous) reg
      result = registreCreer(nomSr);

      // On en fait un paramètre
      sRParam = parametreCreer(nomSr, NULL, (void *) result, NULL);
      sRParam->estUnRegistre = 1;
      
      // On l'insère dans le registre père
      registreAjouterParametre(reg, sRParam);	 
   }
   
   return result;
}

/**
 * @brief Ajouter dans un registre un paramètre décrit dans une chaîne
 * de caractères.
 *
 * La chaîne doit être sous la forme "a.b.c=v"
 */
#ifndef REGISTRE_NOM_SEPARATEUR
#   define REGISTRE_NOM_SEPARATEUR '.'
#endif
#ifndef REGISTRE_AFFECTATION
#   define REGISTRE_AFFECTATION '='
#endif
static
void registreAjouterC(registre * reg, char * chaine)
{
   char * debut;    // Pointeur sur le début du nom
   char * fin;      // Pointeur sur la fin du nom courant
   char   nom[512]; // Une chaine pour héberger temporairement le nom courant
   int    lgNom;    // Longueur du nom courant
   char * valeur;
   registre * regSS = reg; // La base du sous-système dans lequel on
			   // va introduire
   parametre * param; // Le parametre qu'on va créer/modifier
   char * aff = NULL; // Pour restauter le signe d'affectation
   
   // On fait pointer valeur sur la représentation ou une chaîne vide
   valeur = prochainDelimiteur(chaine, REGISTRE_AFFECTATION);

   // On s'assure que le nom (qui précède l'affectation) est terminé
   // par un 0
   if (*valeur == REGISTRE_AFFECTATION) {
      *valeur = 0;
      aff = valeur; // On note où il était
      valeur ++;
   }
   
   // Nous avons donc maintenant deux chaines de caractères (collées
   // l'une derrière l'autre) l'une contient un nom, l'autre une
   // valeur (cette dernière peut être une chaîne vide)
   debut = chaine;
   
   // Il faut maintenant construire et/ou renseigner la structure sur
   // la base du nom 
   while (strlen(debut)) {
      fin = prochainDelimiteur(debut, REGISTRE_NOM_SEPARATEUR);
      assert(fin > debut);   // Puisque strlen(debut) != 0
      lgNom = fin - debut;
      memcpy(nom, debut, lgNom);
      nom[lgNom] = 0;
      // Si on a un séparateur, on descend dans l'arborescence, sinon,
      // on est sur le 0 final
      if (*fin == REGISTRE_NOM_SEPARATEUR) {

         // On descend dans l'arborescence
         regSS = registreChercherSousRegistre(reg, nom);
	 if (regSS == NULL) {
	   regSS = registreAjouterSousRegistre(reg, nom);
	 }
	 // Eventuellement en la créant
	 if (regSS != NULL) {
	   reg = regSS; // On travaille maintenant dans le sous reg
	 }
         
         // On passe au suivant
         debut = fin + 1;
      // Si on est à la dernière, on peut placer la donnée
      } else {
         param = listeParametresChercher(regSS->parametres, nom);
         if (param == NULL) {
             param = parametreCreer(nom, valeur, NULL, NULL);
	     listeParametresInserer(regSS->parametres, param);
	 } else {
             parametreAffecterValeur(param, valeur);
	 }
	
         //  On en arrive à la fin
         debut = fin;
      }
   }
   // On restaure le symbole d'affectation 
   if (aff) {
     *aff = REGISTRE_AFFECTATION;
   }
}

/**
 *  @brief : affectation de la valeur/fonction de maj d'un paramètre
 *  @param base : le registre dans lequel est le paramètre
 *  @param valeur : la valeur du paramètre sous forme de chaîne (ou
 *  NULL)
 *  @param miseAJour : la fonction de mise à jour (ou NULL)
 *  @param listargs : le nom sous forme de liste de chaînes
 *
 *  si miseAJour == NULL alors
 *     la valeur est affectée au paramètre
 *     si la fonction de mise-à-jour existe alors
 *        elle est invoquée (avec cette nouvelle valeur)
 *     finsi
 *  sinon
 *     la fonction de mise-à-jour est affectée
 *     si le paramètre n'avait pas de valeur définie, alors
 *        On lui affecte celle passée en paramètre
 *     fin si
 *     la fonction de mise-à-jour est invoquée
 *  finsi
 */
static
void registreAffecterParametreT(registre * reg,
                                char * valeur,
				void * prive,
                                registreMiseAJour miseAJour,
                                va_list argList)
{
   char * chaine, * prochaine;
   registre * regSS = reg; // La base du sous-système
   parametre * param;
   
   chaine = va_arg(argList, char *);
   prochaine = va_arg(argList, char *);

   //printk("Affectation dans %s ...\n", reg->nom);
   while (chaine != NULL) {
      // Si on n'est pas à la dernière, on descend dans l'arborescence
      // en la créant si nécessaire
      if (prochaine != NULL) {
         regSS = registreChercherSousRegistre(reg, chaine);
	 if (regSS == NULL) {
	   regSS = registreAjouterSousRegistre(reg, chaine);
	 }
	 if (regSS != NULL) {
	    reg = regSS; // On travaille maintenant dans le sous reg
	 }
      // Si on est à la dernière, on peut affecter le paramètre
      } else {
         // On le récupère s'il existait
         param = listeParametresChercher(reg->parametres, chaine);
	 // On le crée sinon
	 if (param == NULL) {
            param = parametreCreer(chaine, valeur, prive, miseAJour);
	    listeParametresInserer(reg->parametres, param);
	 } else {
            // Si c'est une nouvelle valeur, on appelle la mise-à-jour
	    if (miseAJour == NULL) {
               parametreAffecterValeur(param, valeur);
	    } else { // Affectation de la mise-à-jour
	       // La valeur fournie avec une fonction de mise-à-jour
	       // est une valeur par défaut, qui n'est utile qu'en
               // l'absence de valeur déjà renseignée
	       if (param->valeur == NULL) {
                  parametreAffecterValeur(param, valeur);
	       }
               parametreAffecterMiseAJour(param, prive, miseAJour);
	    }
	 }
      }
      // On avance
      chaine = prochaine;
      prochaine = va_arg(argList, char *);
   }
}

/**
 * @brief La fonction fournie pour affecter un paramètre dans un
 * registre 
 *
 */
void registreAffecterParametre(registre * reg,
                               char * valeur,
			       void * prive,
                               registreMiseAJour miseAJour,
                               ...)
{
   va_list   argList;

   va_start(argList, miseAJour);
   registreAffecterParametreT(reg, valeur, prive, miseAJour, argList);
   va_end(argList);
}

/**
 * @brief récupération d'un paramètre dans un registre, NULL si
 * inexistant
  */
[[maybe_unused]]
static
parametre * registreObtenirParametre(registre * reg, va_list argList)
{
   parametre * result = NULL;

   char * chaine, * prochaine;
   chainonParametre * ch;

   registre * regSS = reg; // Le registre du sous-système

   chaine = va_arg(argList, char *);
   prochaine = va_arg(argList, char *);

   while (chaine != NULL) {
      // Si on n'est pas à la dernière, on descend dans l'arborescence
      if (prochaine != NULL) {
         regSS = registreChercherSousRegistre(reg, chaine);
	 if (regSS == NULL) {
            return result;
	 }
         reg = regSS;
      // Si on est à la dernière, on peut lire la donnée
      } else {
         for (ch = regSS->parametres->premier; ch != NULL; ch = ch->suivant){
            // Si on la trouve, c'est bon
            if (strcmp(ch->parametre->nom, chaine) == 0) {
               return ch->parametre;
            }
         }
      }
      // On avance
      chaine = prochaine;
      prochaine = va_arg(argList, char *);
   }
   
   return result;
}

/**
 * @brief Affichage (via printk) d'un registre
 */
static
void registreAfficher(registre * reg, int profondeur)
{
   chainonParametre * ch;
   int p;

   for (p = 0 ; p < profondeur; p++) {printk("   ");}
   printk("[%s]\n", reg->nom);

   for (ch = reg->parametres->premier; ch != NULL; ch = ch->suivant){
      for (p = 0 ; p < profondeur; p++) {printk("   ");}
      printk("%s : ", ch->parametre->nom);
      if (ch->parametre->estUnRegistre){
         printk("(reg)\n");
         registreAfficher((registre *)ch->parametre->prive, profondeur +1);
      } else {
         printk("%s\n", ch->parametre->valeur);
      }
   }
}

/*----------------------------------------------------------------------------*/
/* Le registre système.                                                       */
/*----------------------------------------------------------------------------*/

/**
 * @brief Création du registre du système
 */
void registreSystemeInitialiser(void)
{
   registreSysteme = registreCreer("ManuX-Param");     

#ifdef MANUX_DEBUG
   A FAIRE : introduire les masque de debug avec la valeur par défaut
#endif   
}

/**
 * @brief Ajouter dans un registre système un paramètre décrit dans une chaîne
 * de caractères.
 *
 * La chaîne doit être sous la forme "a.b.c=v"
 * Attention, elle va être modifiée !
 */
void registreSystemeAjouterC(char * chaine)
{
  registreAjouterC(registreSysteme, chaine);
}

/**
 * @brief Ajouter au registre système un paramètre dont le nom est
 * donné comme une liste de paramètres.
 * 
 * Exemple
 *    registreSystemeAjouterParametre("192.168.10.2/24", &netif,
 *    netIpAddr, "net", "ip", "addr", NULL);
 */
void registreSystemeAffecterParametre(char * valeur,
		                      void * prive,
                                      registreMiseAJour miseAJour, 
                                      ...)
{
   va_list   argList;

   va_start(argList, miseAJour);
   registreAffecterParametreT(registreSysteme, valeur, prive, miseAJour, argList);
   va_end(argList);    
}

/**
 * @brief Affichage du registre système
 */
void registreSystemeAfficher(void)
{
    registreAfficher(registreSysteme, 0);   
}
