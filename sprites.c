/*
 * Pong pour Neo Geo - petites fonctions pour piloter les sprites.
 *
 * Chaque sprite est decrit dans 4 tables de la memoire video (VRAM) :
 *   SCB1 : la liste de ses tuiles (64 mots par sprite)
 *   SCB2 : son facteur de reduction (zoom)
 *   SCB3 : sa position verticale et sa hauteur en tuiles
 *   SCB4 : sa position horizontale
 */
#include "sprites.h"

// Leurs adresses (ADDR_SCB1 a ADDR_SCB4) sont deja definies par
// ngdevkit, dans <ngdevkit/registers.h>.
#define NB_SPRITES 381

// Au demarrage, la VRAM contient n'importe quoi : un sprite de hauteur 0
// n'est pas affiche, donc on met toutes les hauteurs a 0.
void cacher_tous_les_sprites(void) {
    *REG_VRAMADDR = ADDR_SCB3;
    *REG_VRAMMOD = 1;
    for (u16 i = 0; i < NB_SPRITES; i++) {
        *REG_VRAMRW = 0;
    }
}

// Donne a un sprite sa colonne de tuiles (consecutives dans la ROM C),
// sa palette, et sa taille normale (pas de reduction).
void sprite_tuiles(u16 sprite, u16 premiere_tuile, u8 nb_tuiles, u8 palette) {
    *REG_VRAMADDR = ADDR_SCB1 + sprite * 64;
    *REG_VRAMMOD = 1;
    for (u8 i = 0; i < nb_tuiles; i++) {
        *REG_VRAMRW = premiere_tuile + i;
        *REG_VRAMRW = palette << 8;
    }
    *REG_VRAMADDR = ADDR_SCB2 + sprite;
    *REG_VRAMRW = 0x0fff;
}

// Place un sprite a l'ecran. x, y : coin haut gauche, en pixels.
// La Neo Geo compte Y a l'envers : 496 - y.
void sprite_placer(u16 sprite, s16 x, s16 y, u8 nb_tuiles) {
    *REG_VRAMADDR = ADDR_SCB3 + sprite;
    *REG_VRAMMOD = 0x200;
    *REG_VRAMRW = ((496 - y) << 7) | nb_tuiles;
    *REG_VRAMRW = x << 7;
}
