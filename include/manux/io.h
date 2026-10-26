/*----------------------------------------------------------------------------*/
/*       Manipulation des entrées/sorties.                                    */
/*                                                  (C) Manu Chaput 2000-2022 */
/*----------------------------------------------------------------------------*/
#ifndef IO_DEF
#define IO_DEF

// WARNING les "Nd" étaient des "N", ...

/**
 * @brief Emission d'un octet sur un port
 */
#define outb(port, octet) \
  asm volatile ("outb %%al, %1":: "a"(octet), "Nd"((uint16_t)port))

/**
 * @brief Lecture d'un octet depuis un port
 */
#define inb(port, adresseOctet) \
   asm volatile ("inb %1, %%al": "=a"(adresseOctet): "Nd"((uint16_t)port))

/**
 * @brief Emission d'un mot sur un port
 */
#define outw(port, mot) \
   asm volatile ("outw %%ax, %1":: "a"(mot), "Nd"((uint16_t)port))

/**
 * @brief Lecture d'un mot depuis un port
 */
#define inw(port, adresseMot) \
   asm volatile ("inw %1, %%ax": "=a"(adresseMot): "Nd"((uint16_t)port))

/**
 * @brief Emission d'un long sur un port
 */
#define outl(port, lng) \
   asm volatile ("outl %%eax, %1":: "a"(lng), "Nd"((uint16_t)port))

/**
 * @brief Lecture d'un long depuis un port
 */
#define inl(port, adresseLong) \
   asm volatile("inl %%dx, %%eax": "=a"(adresseLong): "d"((uint16_t)port))

#endif
