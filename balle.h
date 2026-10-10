/*
 * Pong pour Neo Geo - la balle.
 */
#ifndef FICHIER_BALLE_H
#define FICHIER_BALLE_H

#include "pong.h"
#include "raquette.h"

typedef struct {
    s32 x, y;       // coin haut gauche, en virgule fixe 8.8
    s16 dx, dy;     // vitesse, en virgule fixe 8.8 par image
    u16 sprite;
} Balle;

void balle_init(Balle *b, u16 sprite);
void balle_au_centre(Balle *b);
void balle_servir(Balle *b, u8 joueur);
void balle_mettre_a_jour(Balle *b);
void balle_afficher(const Balle *b);
u8 balle_touche(const Balle *b, const Raquette *r);
void balle_renvoyer(Balle *b, const Raquette *r);
u8 balle_rebondir_murs(Balle *b);

#endif
