/*
 * Pong pour Neo Geo - le compteur de debogage.
 */
#ifndef FICHIER_DEBUG_H
#define FICHIER_DEBUG_H

#include <ngdevkit/neogeo.h>

void debug_init(void);
void debug_apres_vblank(void);
void ecrire_nombre(u8 x, u8 y, u8 palette, u32 nombre, u8 chiffres);

#endif
