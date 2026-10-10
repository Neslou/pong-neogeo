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

// Collision "boite contre boite" (AABB) : deux rectangles se touchent
// s'il n'y a de trou ni a l'horizontale, ni a la verticale.
u8 balle_touche(const Balle *b, const Raquette *r) {
    s16 bx = PIXELS(b->x), by = PIXELS(b->y);
    s16 ry = PIXELS(r->y);
    // un trou a gauche ou a droite ?
    if (bx > r->x + RAQUETTE_L || r->x > bx + BALLE_T) {
        return 0;
    }
    // un trou en haut ou en bas ?
    if (by > ry + RAQUETTE_H || ry > by + BALLE_T) {
        return 0;
    }
    return 1;
}

// La balle repart dans l'autre sens, un peu plus vite (x 1,03125),
// avec un angle au hasard, et sort de la raquette pour ne pas
// rebondir en boucle a l'image suivante.
void balle_renvoyer(Balle *b, const Raquette *r) {
    s16 vitesse = b->dx < 0 ? -b->dx : b->dx;   // la vitesse, sans le sens
    vitesse += vitesse >> 5;                    // + 1/32 : pas de multiplication
    if (vitesse > VITESSE_MAX_BALLE) {
        vitesse = VITESSE_MAX_BALLE;
    }
    if (r->x < ECRAN_L / 2) {
        // raquette de gauche : la balle repart vers la droite
        b->dx = vitesse;
        b->x = FIXE(r->x + RAQUETTE_L);
    } else {
        // raquette de droite : la balle repart vers la gauche
        b->dx = -vitesse;
        b->x = FIXE(r->x - BALLE_T);
    }
    // Cours d'origine : 10 a 150 px/s en hauteur, dans le meme sens.
    // Par image et a notre echelle : 40 a 598.
    if (b->dy < 0) {
        b->dy = -hasard_entre(40, 598);
    } else {
        b->dy = hasard_entre(40, 598);
    }
}

// Rebond sur les murs du haut et du bas. Renvoie 1 s'il y a eu rebond.
u8 balle_rebondir_murs(Balle *b) {
    if (b->y <= 0) {
        b->y = 0;
        b->dy = -b->dy;
        return 1;
    }
    if (b->y >= FIXE(ECRAN_H - BALLE_T)) {
        b->y = FIXE(ECRAN_H - BALLE_T);
        b->dy = -b->dy;
        return 1;
    }
    return 0;
}
