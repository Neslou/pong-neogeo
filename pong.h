/*
 * Pong pour Neo Geo - constantes communes a tout le jeu.
 */
#ifndef FICHIER_PONG_H
#define FICHIER_PONG_H

#include <ngdevkit/neogeo.h>

// --- Dimensions (en pixels) ---------------------------------------------
#define ECRAN_L       320
#define ECRAN_H       224
#define RAQUETTE_L      5
#define RAQUETTE_H     20
#define BALLE_T         4

// --- Virgule fixe 8.8 : la valeur x 256 -----------------------------------
#define FIXE(px)  ((s32)(px) << 8)
#define PIXELS(f) ((s16)((f) >> 8))

// --- Vitesses (8.8, par image) -------------------------------------------
#define VITESSE_RAQUETTE  797   // 3,11 px par image

// --- Tuiles, sprites et palettes ------------------------------------------
#define TUILE_RAQUETTE  256
#define TUILE_BALLE     258
#define SPR_RAQUETTE1   1
#define SPR_RAQUETTE2   2
#define SPR_BALLE       3
#define PAL_BLANC       0
#define PAL_SPRITES    16

#endif
