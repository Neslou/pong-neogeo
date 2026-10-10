/*
 * Pong pour Neo Geo - petites fonctions pour piloter les sprites.
 */
#ifndef FICHIER_SPRITES_H
#define FICHIER_SPRITES_H

#include <ngdevkit/neogeo.h>

void cacher_tous_les_sprites(void);
void sprite_tuiles(u16 sprite, u16 premiere_tuile, u8 nb_tuiles, u8 palette);
void sprite_placer(u16 sprite, s16 x, s16 y, u8 nb_tuiles);

#endif
