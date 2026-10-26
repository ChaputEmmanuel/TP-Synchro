/**
 * @file
 * @brief Définition d'une tâche minimaliste qui est là pour afficher
 * quelques éléments à la demande de l’utilisateur.
 *
 * Elle peut être lancée par le scheduler (si MANUX_TACHES est
 * défini), mais elle peut aussi être utilisée comme une fonction
 * quelconque.
 *                                                     (C) Manu Chaput 2000-2026
 */
#ifndef DUMMYTASK_DEF
#define DUMMYTASK_DEF

/**
 * @brief Le corps d'une tâche à exécuter lorsqu'on n'a que ça à faire, ...
 *
 * Cette tâche ne fait rien de très important, si ce n'est récupérer
 * les événements clavier sur la console noyau. Grâce à ça c'est elle
 * qui permet d'afficher quelques informations sur le système
 */
void aDummyKernelTask(void);


#endif // DUMMYTASK_DEF
