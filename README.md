# Objectif général

   Mettre en place les outils de synchronisation permettant d'assurer le bon fonctionnement
des tubes de communication (équivalent du pipe Unix) dans "ManuX"

   L'implantation des tubes est fournies.

   On implantera, une version bloquante et une version non bloquante.

# Compilation de ManuX

   ```
   make
   make run
   ```


   Le code source est réparti dans quelques répertoires.

   L'implantation des tubes est dans le répertoire `sf`.

   L'implantation des "programmes utilisateurs" est dans `usr`,
c'est ici le fichier `usr/init-acces-concurrent.c` (fourni) qui sera utilisé.

# Les tubes

## Implantation

   Le fichier `sf/tubes.c` (et `include/manux/tubes.h`) implante (et définit)
l'objet tube.

   Les fonctions les plus importantes ici sont les suivantes

```
   tubeOuvir()
   tubeFermer()
   tubeLire()
   tubeEcrire()
```

## Utilisation

   Côté utilisateur (`usr/`) :

   . Un tube est créé avec

```
int fd[2];
int r;
r = tube(fd); // ESUCCES ou ENOMEM
```

   Il est directement ouvert et ce qui est écrit (sur `fd[1]`)
peut ensuite être lu (sur `fd[0]`) dans l'ordre.

   . Un tube est manipulé avec `read()` et `write()`

retour : le nombre d'octets lus ou écrits (si >=0) ou `-EINVAL` en
cas d'erreur

# Userland

   Le fichier `usr/init-acces-concurrent.c` implante des tâches dont
certaines écrivent et d'autres lisent dans le tube.

# Configuration du noyau

   Dans le fichier include/config/base.h on peut définir la macro `MANUX_REENTRANT`
sans laquelle un gros verrou empêche l'accès au noyau par deux tâches "simultanément".

##  Noyau non ré-entrant

   Si la macro `MANUX_REENTRANT` n'est pas définie, il n'y a donc qu'une tâche max
dans le noyau à un instant t, il n'y a donc pas trop de soucis avec le code du tube
qui va fonctionner (mais qui est non bloquant).

## Noyau ré-entrant

  Dans ce cas, il est important de protéger les fonctions par des outils de synchro
afin d'assurer la cohérence des données.

# Les outils de synchro

   Ils sont définis dans `include/manux/atomique.h` et implantés dans `noyau/atomique.c` et
nous utiliserons les types suivants

```
   exclusionMutuelle
   condition
```
