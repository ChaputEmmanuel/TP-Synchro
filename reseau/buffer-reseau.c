/**
 * @file
 * @brief
 *                                                                  (C) Manu Chaput 2025
 */
#include <manux/buffer-reseau.h>
#include <manux/debug.h>
#include <manux/string.h>    // memcopy
#include <manux/kmalloc.h>

typedef struct _CelluleBufferReseau {
   BufferReseau                * br;
   struct _CelluleBufferReseau * suivant;
} CelluleBufferReseau;
  
typedef struct _ListeBufferReseau {
   CelluleBufferReseau * premier;
   CelluleBufferReseau * dernier;
} ListeBufferReseau;

/**
 * @brief Création (allocation et initialisation des champs de base)
 * d'un buffer réseau.
 *
 * On va ici créer une structure et y recopier les données. A voir :
 * ne peut-on pas plutôt utiliser les données telles qu'elles arrivent
 * (pour virtio par exemple) ? Ca éviterait une copie, ou ça nous
 * permettrait au moins de la faire plus tard, dans la partie basse.
 */
BufferReseau * bufferReseauCreer(void * donnees, int taille)
{
   BufferReseau * result = (BufferReseau *)kmalloc(sizeof(BufferReseau));

   if (result) {
      printk_debug(DBG_KERNEL_NET, "Creation d'un buffer reseau de taille %d\n", taille);
      result->donnees = kmalloc(taille);
      if (result->donnees) {
         memcpy(result->donnees, donnees, taille);
         result->taille = taille;
      } else {
         kfree(result);
         result = NULL;
      }
   }
   
   return result;
}

/**
 * @brief Création d'une liste vide de buffers réseau
 */
ListeBufferReseau * listeBufferReseauCreer(void)
{
   ListeBufferReseau * result = (ListeBufferReseau *)kmalloc(sizeof(ListeBufferReseau));

   if (result) {
      printk_debug(DBG_KERNEL_NET, "Creation d'une liste de buffers reseau\n");
      result->premier = NULL;
      result->dernier = NULL;
   };
   
   return result;
}

/**
 * @brief Une liste de buffers est-elle vide ?
 */
int listeBufferReseauVide(ListeBufferReseau * l)
{
   return ((l == NULL) || (l->premier == NULL));
}

/**
 * @brief Insertion d'un buffer réseau à la fin d'une liste
 */
int listeBufferReseauInserer(ListeBufferReseau * l, BufferReseau * b)
{ 
   printk_debug(DBG_KERNEL_NET, "Insertion d'un nouveau buffer\n");

   assert (l != NULL);
   
   CelluleBufferReseau * c = (CelluleBufferReseau *)kmalloc(sizeof(CelluleBufferReseau));

   if (c) {
      c->br = b;
      c->suivant = l->premier;
      l->premier = c;
      if (l->dernier == NULL) {
         l->dernier = c;
      }
      return 1;
   } else {
      return 0;
   }
}

/**
 * @brief Extraction d'un buffer réseau d'une liste
 *
 * NULL si liste inexistante ou vide
 */
BufferReseau * listeBufferReseauExtraire(ListeBufferReseau * l)
{
   CelluleBufferReseau * cellResult = NULL;

   printk_debug(DBG_KERNEL_NET, "Extraction d'un buffer\n");
   
   if ((l != NULL) && (l->premier != NULL)) {
      cellResult = l->premier;
      l->premier = cellResult->suivant;
      if (l->dernier == cellResult) {
         l->dernier = NULL;
      }
      return cellResult->br;
   } else {
      return NULL;
   }
}

/**
 * @brief Affichage de la liste de buffers
 */
void listeBufferReseauAfficher(ListeBufferReseau * l)
{
   int n = 0;
   CelluleBufferReseau * cell;
   uint8_t * data;
   
   if (!listeBufferReseauVide(l)) {
      for (cell = l->premier; cell != NULL; cell = cell->suivant) {
         n++;
      }
      printkc("Il y a %d paquet(s) dans la file :\n", n);
      for (cell = l->premier; cell != NULL; cell = cell->suivant) {
	data = (uint8_t*)cell->br->donnees;
	printkc("l=%d : 0x%x 0x%x 0x%x 0x%x\n", cell->br->taille,
		data[0],
		data[1],
		data[2],
		data[3]);
      }
   } else {
      printkc("File reseau pas encore initialisee ...\n");
   }
}

