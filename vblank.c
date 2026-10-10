/*
 * Pong pour Neo Geo - notre propre attente de VBlank.
 *
 * A chaque fin d'image (59,19 fois par seconde), le 68000 recoit une
 * interruption "VBlank". Le runtime de ngdevkit appelle alors la
 * fonction rom_callback_VBlank() si le jeu en fournit une.
 * ng_wait_vblank() de ngdevkit fait exactement ca en coulisse ; ici on
 * l'ecrit nous-memes pour COMPTER les images : c'est ce compteur qui
 * permet de voir si notre boucle a rate une image.
 */
#include "vblank.h"

volatile u32 vblank_compteur = 0;

// Appelee par l'interruption VBlank : on ne fait que compter.
void rom_callback_VBlank(void) {
    vblank_compteur++;
}

// Attend la prochaine interruption VBlank.
void attendre_vblank(void) {
    u32 depart = vblank_compteur;
    while (vblank_compteur == depart) {
        // on attend : "volatile" oblige le compilateur a relire la variable
    }
}
