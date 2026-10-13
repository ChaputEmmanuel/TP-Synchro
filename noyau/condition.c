/**
 * @file condition.c
 * @brief Implantation des opérations atomiques de ManuX.
 *
 *                                                     (C) Manu Chaput 2000-2024
 */
#include <manux/condition.h>
#include <manux/memoire.h>   // NULL
#include <manux/debug.h>     // assert
#if defined(MANUX_CONDITION_AUDIT) && defined(MANUX_KMALLOC)
#   include <manux/kmalloc.h>
#   include <manux/listetaches.h>
#endif

#if defined(MANUX_CONDITION_AUDIT) && defined(MANUX_KMALLOC)

/**
 * @brief Définition des listes de conditions
 */
typedef struct _CelluleCondition {
   Condition                * condition;
   struct _CelluleCondition * suivant;
} CelluleCondition;
  
typedef struct _ListeConditions {
   CelluleCondition * tete;
   CelluleCondition * queue;
} ListeConditions;

/**
 * @brief création d'une liste de conditions
 */
static
ListeConditions * listeConditionsCreer(void)
{
   ListeConditions * result;

   result = (ListeConditions *) kmalloc(sizeof(ListeConditions));

   result->tete = NULL;
   result->queue = NULL;

   return result;
}

/**
 * @brief Insertion d'une condition dans une liste (au début)
 *
 */
static
void listeConditionsInserer(ListeConditions * l, Condition * c)
{
   CelluleCondition * cell = (CelluleCondition *) kmalloc(sizeof(CelluleCondition));
   
   assert(l != NULL);
   assert(c != NULL);

   if (cell != NULL) {
      cell->condition = c;
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
 * @brief  Liste des conditions définies sur le système
 */
static ListeConditions * listeConditions = NULL;

/**
 * @brief Affichage de l'état des variables condition
 */
void conditionsAfficherEtat(void)
{
   CelluleCondition * cell;
   CelluleTache * cellT;

   printk("\nEtat des variables de condition sur le systeme :\n\n");
   printk("Cond    |    s |    d | Attente\n");
   printk("--------+------+------+--------\n");

   if (listeConditions != NULL) {
      for (cell = listeConditions->tete; cell != NULL; cell = cell->suivant){
         printk("0x%x | %4d | %4d | ",
		cell->condition,
		cell->condition->nbSignaler,
		cell->condition->nbDiffuser);
         for (cellT = cell->condition->tachesEnAttente.tete ;
	      cellT != NULL; cellT = cellT->suivant){
            printk("%d ", cellT->tache->numero);
         }
         printk("\n");
      }
   }
   printk("--------+------+------+--------\n");
}

#endif //  defined(MANUX_CONDITION_AUDIT) && defined(MANUX_KMALLOC)


/**
 * @brief Initialisation d'une condition
 */
void conditionInitialiser(Condition * cond)
{
   initialiserListeTache((&cond->tachesEnAttente));

#if defined(MANUX_CONDITION_AUDIT)
   cond->nbSignaler = 0;
   cond->nbDiffuser = 0;

   if (listeConditions == NULL) {
      listeConditions = listeConditionsCreer();
   }
   if (listeConditions != NULL) {
      listeConditionsInserer(listeConditions, cond);
   }
#endif
}

/**
 * @brief Attente de la prochaine occurence d'une condition
 *
 * La tâche appelante doit être dans l'exclusion mutuelle
 * (qui sera automatiquement libérée le temps de l'attente puis
 * reprise avant que cette fonction ne rende la main)
 */
void conditionAttendre(Condition * cond, ExclusionMutuelle * em)
{
   // On se place dans la liste des tâches en attente de la condition
   // que l'on peut manipuler sans risque car on est protégé par
   // l'exclusion mutuelle 
   insererCelluleTache(&(cond->tachesEnAttente),
                       tacheEnCours,
                       (CelluleTache*)tacheEnCours+sizeof(Tache));
   
   // On doit libérer l'exclusion mutuelle et se placer dans l'état
   // bloqué, on va se rendre non préemptible pour faire ça afin de ne
   // pas se retrouvée bloquée après avoir été préemptée par une tâche
   // qui signale la condition !
   tacheEnCoursInterdirePreemption();
   
   exclusionMutuelleSortir(em);
   tacheEnCours->etat = Tache_Bloquee;
   
   tacheEnCoursAutoriserPreemption();

   // On est bloquée et préemptible, on peut rendre la main
   ordonnanceur();

   // Il nous faut ré-acquérir l'exclusion mutuelle
   exclusionMutuelleEntrer(em);
}

/**
 * @brief Signaler une occurence d'une condition
 *
 * Attention, doit être utilisée sous la protection de l'exclusion
 * mutuelle détenue par les tâches en attente.
 * Elle est donc inutilisable dans un handler d'interruption.
 */
void conditionSignaler(Condition * cond)
{
   Tache * ta;

   // Si on trouve une tâche en attente de cette condition
   // on la réveille.
   if ((ta = extraireTache(&(cond->tachesEnAttente))) != NULL) {
      ta->etat = Tache_Prete;
      insererCelluleTache(&listeTachesPretes,
                          ta,
                          (CelluleTache*)ta+sizeof(Tache));
   }
#if defined(MANUX_CONDITION_AUDIT)
   cond->nbSignaler++;
#endif
}

/**
 * @brief Signaler une occurence d'une condition à toutes les tâches
 * en attente
 *
 * Attention, doit être utilisée sous la protection de l'exclusion
 * mutuelle détenue par les tâches en attente.
 * Ell est donc inutilisable dans un handler d'interruption.
 */
void conditionDiffuser(Condition * cond)
{
   Tache * ta;

   // Si on trouve une tâche en attente de cette condition
   // on la réveille.
   while ((ta = extraireTache(&(cond->tachesEnAttente))) != NULL) {
      ta->etat = Tache_Prete;
      insererCelluleTache(&listeTachesPretes,
                          ta,
                          (CelluleTache*)ta+sizeof(Tache));
   }
#if defined(MANUX_CONDITION_AUDIT)
   cond->nbDiffuser++;
#endif
}

