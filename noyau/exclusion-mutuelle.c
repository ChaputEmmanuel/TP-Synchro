/**
 * @file exclusion-mutuelle.c
 * @brief Implantation des exclusions mutuelles.
 *
 *                                                    (C) Manu Chaput 2000-2024
 */
#include <manux/exclusion-mutuelle.h>
#include <manux/stddef.h>   // NULL
#include <manux/debug.h>    // assert, ...
#include <manux/kmalloc.h>    // kmalloc, ...

#if defined(MANUX_EXCLUSION_MUTUELLE_AUDIT)

/**
 * @brief Définition des listes d'exclusions mutuelles pour pouvoir
 * lister celles qui sont définies sur le systèmes
 */
typedef struct _CelluleExclusionMutuelle {
   ExclusionMutuelle                * exclusionMutuelle;
   struct _CelluleExclusionMutuelle * suivant;
} CelluleExclusionMutuelle;
  
typedef struct _ListeExclusionsMutuelles {
   CelluleExclusionMutuelle * tete;
   CelluleExclusionMutuelle * queue;
} ListeExclusionsMutuelles;

/**
 * @brief Création d'une liste d'exclusions mutuelles
 */
static
ListeExclusionsMutuelles * listeExclusionsMutuellesCreer(void)
{
   ListeExclusionsMutuelles * result;

   result = (ListeExclusionsMutuelles *) kmalloc(sizeof(ListeExclusionsMutuelles));

   if (result == NULL) {
      paniqueNoyau("retour de kmalloc NULL\n");
   }
   result->tete = NULL;
   result->queue = NULL;

   return result;
}

/**
 * @brief Insertion d'une exclusionMutuelle dans une liste (au début)
 *
 */
static
void listeExclusionsMutuellesInserer(ListeExclusionsMutuelles * l, ExclusionMutuelle * c)
{
   CelluleExclusionMutuelle * cell = (CelluleExclusionMutuelle *) kmalloc(sizeof(CelluleExclusionMutuelle));
   
   assert(l != NULL);
   assert(c != NULL);

   if (cell != NULL) {
      cell->exclusionMutuelle = c;
      cell->suivant = NULL;
      if (l->queue != NULL) {
         l->queue->suivant = cell;
      }
      l->queue = cell;
      if (l->tete  == NULL) {
	l->tete = cell;
      }
   }
}

/** 
 * @brief  Liste des exclusionMutuelles définies sur le système
 */
static ListeExclusionsMutuelles * listeExclusionsMutuelles = NULL;

/**
 * @brief Affichage de l'état des variables d'exclusion mutuelle
 */
void exclusionsMutuellesAfficherEtat(void)
{
   CelluleExclusionMutuelle * cell;
   CelluleTache * cellT;

   printk("\nEtat des variables d'exclusion mutuelle sur le systeme :\n\n");
   printk("Excl M  |   en |   so | Attente\n");
   printk("--------+------+------+--------\n");
   if (listeExclusionsMutuelles != NULL) {
      for (cell = listeExclusionsMutuelles->tete; cell != NULL; cell = cell->suivant){
         printk("0x%x | %4d | %4d | ",
	    cell->exclusionMutuelle,
	    cell->exclusionMutuelle->nbEntrees,
	    cell->exclusionMutuelle->nbSorties);
         for (cellT = cell->exclusionMutuelle->tachesEnAttente.tete ;
	      cellT != NULL; cellT = cellT->suivant){
            printk("%d ", cellT->tache->numero);
          }
          printk("\n");
       }
   }
   printk("--------+------+------+--------\n");
}

#endif // defined(MANUX_EXCLUSION_MUTUELLE_AUDIT)

/**
 * @brief Initialisation d'une exclusion mutuelle
 */
void exclusionMutuelleInitialiser(ExclusionMutuelle * em)
{
   atomiqueInit(&(em->verrou), 0);
   initialiserListeTache(&(em->tachesEnAttente));

#if defined(MANUX_EXCLUSION_MUTUELLE_AUDIT)
   em->nbEntrees = 0;
   em->nbSorties = 0;

   if (listeExclusionsMutuelles == NULL) {
      listeExclusionsMutuelles = listeExclusionsMutuellesCreer();
   }
   if (listeExclusionsMutuelles != NULL) {
      listeExclusionsMutuellesInserer(listeExclusionsMutuelles, em);
   }
#endif
};

/**
 * @brief Entrée en exclusion mutuelle.
 * 
 * La tâche appelante est éventuellement mise en attente dans une
 * file spécifique. Elle n'en sera extraite que par la sortie d'une
 * tâche qui est dans la zone d'exclusion.
 */
void exclusionMutuelleEntrer(ExclusionMutuelle * em)
{
   // Tant que je n'acquière pas le verrou ...
   while (compareEtEchange(&(em->verrou), 0, tacheEnCours->numero)) {

      // ... je me mets en attente. Pour cela, j'interdis la
      // préeemption de sorte à ne pas me retrouvée dans l'état
      // bloquées mais référencée dans aucune liste d'attente !
      tacheEnCoursInterdirePreemption();  
      
      tacheEnCours->etat = Tache_Bloquee;
      insererCelluleTache(&(em->tachesEnAttente),
                          tacheEnCours,
                          (CelluleTache*)tacheEnCours+sizeof(Tache));
      
      tacheEnCoursAutoriserPreemption();

      // Puisque je suis bloquée, je rends la main
      ordonnanceur();   
   };

   // Je peux tranquilement modifier le compteur !  
#if defined(MANUX_EXCLUSION_MUTUELLE_AUDIT)
   em->nbEntrees++;
#endif
}

void exclusionMutuelleSortir(ExclusionMutuelle * em)
{
   Tache * ta;

   // Si on trouve une tâche en attente de cette exclusion mutuelle,
   // on la réveille.
   if ((ta = extraireTache(&(em->tachesEnAttente))) != NULL) {
      ta->etat = Tache_Prete;
      insererCelluleTache(&listeTachesPretes,
                          ta,
                          (CelluleTache*)ta+sizeof(Tache));
   }
   // Le verrou est encore à nous, on peut modifier les compteurs sans
   // crainte 
#if defined(MANUX_EXCLUSION_MUTUELLE_AUDIT)
   (em)->nbSorties++ ;
#endif
   // On déverouille
   atomiqueInit(&(em)->verrou, 0);
}
