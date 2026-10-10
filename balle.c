/*
 * Pong pour Neo Geo - la balle.
 */
#include "balle.h"
#include "sprites.h"
#include "hasard.h"

void balle_init(Balle *b, u16 sprite) {
    b->sprite = sprite;
    sprite_tuiles(sprite, TUILE_BALLE, 1, PAL_SPRITES);
    balle_au_centre(b);
}

// Au centre, avec une vitesse au hasard.
// Cours d'origine : 100 px/s a gauche ou a droite, -50 a 50 px/s en hauteur.
// Par image et a notre echelle : 1,25 px (320) et -0,78 a 0,78 px (-199 a 199).
void balle_au_centre(Balle *b) {
    b->x = FIXE((ECRAN_L - BALLE_T) / 2);
    b->y = FIXE((ECRAN_H - BALLE_T) / 2);
    b->dx = (hasard() & 1) ? 320 : -320;
    b->dy = hasard_entre(-199, 199);
}

void balle_mettre_a_jour(Balle *b) {
    b->x += b->dx;
    b->y += b->dy;
}

void balle_afficher(const Balle *b) {
    sprite_placer(b->sprite, PIXELS(b->x), PIXELS(b->y), 1);
}
