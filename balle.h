/*
 * Pong pour Neo Geo - la balle.
 */
#ifndef FICHIER_BALLE_H
#define FICHIER_BALLE_H

#include "pong.h"

typedef struct {
    s32 x, y;       // coin haut gauche, en virgule fixe 8.8
    s16 dx, dy;     // vitesse, en virgule fixe 8.8 par image
    u16 sprite;
} Balle;

void balle_init(Balle *b, u16 sprite);
void balle_au_centre(Balle *b);
void balle_mettre_a_jour(Balle *b);
void balle_afficher(const Balle *b);

#endif
