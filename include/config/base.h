/**
 * @file config/base.h
 * @brief Eléments de base de la configuration
 *
 * Il s'agit de quelques macros et constantes de base sans conséquence
 * majeure sur la compilation du noyau. Ce ne sont pas des paramètres
 * qui permettent de sélectionner ou même paramétrer les
 * sous-systèmes.
 *
 *                                                    (C) E. Chaput 2022-2023 */

/**
 * @brief la signature d'un chargement par init-manux
 */
#ifndef MANUX_INIT_MAGIC
#   define MANUX_INIT_MAGIC 0x01c0ffee
#endif

/**
 * @brief le nom du fichier qui contient _startManuX()
 */
#ifndef MANUX_FICHIER_MAIN
#   define MANUX_FICHIER_MAIN main
#endif

/**
 * @brief La fréquence du timer
 */
#ifndef MANUX_FREQUENCE_HORLOGE
#   define MANUX_FREQUENCE_HORLOGE 100
#endif

/**
 * @brief Durée du calibrage des attentes actives en nombre de cycles
 * du timer.
 * 
 * La valeur MANUX_FREQUENCE_HORLOGE conduit donc à un calibrage sur
 * une seconde.
 * Attention, on fait une boucle sur un uint32_t donc il faut
 * s'assurer de ne pas dépasser 2^32 !
 */
#ifndef MANUX_NB_CYCLES_CALIBRAGE
#   define MANUX_NB_CYCLES_CALIBRAGE MANUX_FREQUENCE_HORLOGE
#endif

/**
 * @brief Quelle architecture ?
 *
 * Une seule possible pour le moment, mais on peut rêver non ?
 */
#ifndef MANUX_ARCH
#  define MANUX_ARCH i386
#endif

/**
 * @brief Quelques macros me permettant de tenir compte du compilateur
 */
#ifdef __clang__
# define NO_OPTIMIZE __attribute__((optnone))
#elif defined(__GNUC__)
# define NO_OPTIMIZE __attribute__((optimize("O0")))
#else
# define NO_OPTINO_OPTIMIZE Attention
#endif

