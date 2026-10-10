/*
 * Pong pour Neo Geo - une raquette.
 * Le "plan" (la struct) est dans raquette.h ; ici, ce qu'elle sait faire.
 */
#include "raquette.h"
#include "sprites.h"

void raquette_init(Raquette *r, u16 sprite, s16 x, s16 y) {
    r->x = x;
    r->y = FIXE(y);
    r->dy = 0;
    r->sprite = sprite;
    sprite_tuiles(sprite, TUILE_RAQUETTE, 2, PAL_SPRITES);
}

// Avance d'une image, en restant dans l'ecran.
void raquette_mettre_a_jour(Raquette *r) {
    r->y += r->dy;
    if (r->y < 0) {
        r->y = 0;
    }
    if (r->y > FIXE(ECRAN_H - RAQUETTE_H)) {
        r->y = FIXE(ECRAN_H - RAQUETTE_H);
    }
}

void raquette_afficher(const Raquette *r) {
    sprite_placer(r->sprite, r->x, PIXELS(r->y), 2);
}
