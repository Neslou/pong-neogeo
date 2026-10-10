/*
 * Pong pour Neo Geo - le score en grands chiffres.
 *
 * Les chiffres 0 a 9 sont dessines dans pong-sprites.gif (tuiles 260 a 269).
 * Chaque joueur a deux sprites : les dizaines et les unites.
 * On ne redessine un score que lorsqu'il change : c'est le patron
 * "Dirty Flag" (drapeau sale) de Game Programming Patterns.
 */
#include "pong.h"
#include "sprites.h"
#include "score.h"

// Ou s'affichent les scores (cours d'origine : 50 px a gauche et 30 px
// a droite du milieu, au tiers de la hauteur ; a notre echelle).
#define SCORE1_X   123
#define SCORE2_X   182
#define SCORE_Y     75
#define ECART_CHIFFRES 12

void score_init(void) {
    sprite_tuiles(SPR_SCORE1_D, TUILE_CHIFFRE0, 1, PAL_SPRITES);
    sprite_tuiles(SPR_SCORE1_U, TUILE_CHIFFRE0, 1, PAL_SPRITES);
    sprite_tuiles(SPR_SCORE2_D, TUILE_CHIFFRE0, 1, PAL_SPRITES);
    sprite_tuiles(SPR_SCORE2_U, TUILE_CHIFFRE0, 1, PAL_SPRITES);
    score_afficher(1, 0);
    score_afficher(2, 0);
}

void score_afficher(u8 joueur, u8 valeur) {
    u16 spr_dizaines = (joueur == 1) ? SPR_SCORE1_D : SPR_SCORE2_D;
    u16 spr_unites   = (joueur == 1) ? SPR_SCORE1_U : SPR_SCORE2_U;
    s16 x            = (joueur == 1) ? SCORE1_X : SCORE2_X;

    u8 dizaines = valeur / 10;
    u8 unites = valeur % 10;

    if (dizaines == 0) {
        // un seul chiffre : on cache le sprite des dizaines (hauteur 0)
        sprite_placer(spr_dizaines, 0, 0, 0);
        sprite_tuiles(spr_unites, TUILE_CHIFFRE0 + unites, 1, PAL_SPRITES);
        sprite_placer(spr_unites, x, SCORE_Y, 1);
    } else {
        sprite_tuiles(spr_dizaines, TUILE_CHIFFRE0 + dizaines, 1, PAL_SPRITES);
        sprite_tuiles(spr_unites, TUILE_CHIFFRE0 + unites, 1, PAL_SPRITES);
        sprite_placer(spr_dizaines, x, SCORE_Y, 1);
        sprite_placer(spr_unites, x + ECART_CHIFFRES, SCORE_Y, 1);
    }
}
