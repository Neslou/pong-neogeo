/*
 * Pong pour Neo Geo - une raquette.
 */
#ifndef FICHIER_RAQUETTE_H
#define FICHIER_RAQUETTE_H

#include "pong.h"

typedef struct {
    s16 x;          // colonne, en pixels (ne change jamais)
    s32 y;          // ligne du haut, en virgule fixe 8.8
    s16 dy;         // vitesse verticale, en virgule fixe 8.8 par image
    u16 sprite;     // numero du sprite qui l'affiche
} Raquette;

void raquette_init(Raquette *r, u16 sprite, s16 x, s16 y);
void raquette_mettre_a_jour(Raquette *r);
void raquette_afficher(const Raquette *r);

#endif
