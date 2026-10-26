/**
 * @file bootloader.h
 * @brief Interfaçage avec les bootloader
 *                                                                          
 *                                                (C) Manu Chaput 2000 - 2024 
 */
#ifndef BOOTLOADER_CONFIG
#define BOOTLOADER_CONFIG

#include <manux/types.h>     // uint...
#include <manux/string.h>    // memcpy

/**
 * @brief la signature d'un chargement par multiboot (legacy)
 */
#define MULTIBOOT_SIGNATURE 0x2BADB002

/**
 * @brief Valeur passée via %eax par le bootloader pour s'identifier
 */ 
extern uint32_t signatureBootloader;

/**
 * @brief la signature d'un chargement par init-manux
 */
#ifndef MANUX_INIT_SIGNATURE
#   define MANUX_INIT_SIGNATURE 0x01c0ffee
#endif

/**
 * @brief les diverses informations que peut fournir le bootloader
 */
#define MULTIBOOT_PAGE_ALIGN   0x00000001
#define MULTIBOOT_MEMORY_INFO  0x00000002
#define MULTIBOOT_INFO_CMDLINE 0x00000004

/**
 * @brief Structure passée en paramètre par la phase d'init
 * (cf init-manux.nasm)
 */
typedef struct _InfoSysteme {
   uint32_t flags;           // Pour compatibilité avec multiboot
   uint32_t memoireDeBase;   // En Ko
   uint32_t memoireEtendue;  // En Ko
   uint32_t peripheriqueDemarrage ;
   char *   ligneCommande;
} InfoSysteme;


/**
 * @brief Stockage des informations décrivant le système fournies par
 * le bootloader.
 */
extern InfoSysteme infoSysteme;

/**
 * @brief Adresse de la zone mémoire dans laquelle le bootloader nous
 * fournit les informations sur le système.
 */
extern InfoSysteme * _infoSysteme;

/**
 * @brief Lecture des informations fournies par le bootloader
 *
 * Cette fonction doit être invoquée relativement tôt, car elle a la
 * charge de recopier des informations fournies par le bootloader. Ces
 * informations sont dans une zole mémoire que l'on risque ensuite
 * d'utiliser.
 */
void bootloaderInitialiser(void);

/**
 * @brief Analyse de la ligne de commande et modification de certains
 * paramètres. Cette fonction estparticulièrement utile avec un
 * regsitre. 
 */
void bootloaderLireLigneCmd(void);

#endif
 
