/*
 * Pong pour Neo Geo - le compteur de debogage, en vert en bas de l'ecran.
 *
 * vblank_compteur compte les images affichees (59,19 par seconde,
 * voir vblank.c). Notre boucle, elle, passe une fois par image... si elle a
 * fini son travail a temps. Sinon, une image est "perdue" : le jeu
 * ralentit. On compte ces images perdues.
 */
#include <ngdevkit/ng-fix.h>
#include "debug.h"
#include "vblank.h"

#define PAL_VERT     2
#define LIGNE_DEBUG 28

static u32 image_precedente;
static u32 images_perdues;

// Ecrit un nombre en decimal, sur "chiffres" caracteres (avec des 0 devant).
void ecrire_nombre(u8 x, u8 y, u8 palette, u32 nombre, u8 chiffres) {
    char texte[11];
    texte[chiffres] = 0;
    for (s8 i = chiffres - 1; i >= 0; i--) {
        texte[i] = '0' + (nombre % 10);
        nombre /= 10;
    }
    ng_text(x, y, palette, texte);
}

void debug_init(void) {
    MMAP_PALBANK1[PAL_VERT * 16 + 1] = 0x00f0;   // vert vif
    MMAP_PALBANK1[PAL_VERT * 16 + 2] = 0x0030;   // vert sombre (ombre)
    ng_text(1, LIGNE_DEBUG, PAL_VERT, "IMG");
    ng_text(13, LIGNE_DEBUG, PAL_VERT, "PERDUES");
    image_precedente = vblank_compteur;
    images_perdues = 0;
}

// A appeler juste apres attendre_vblank(), une fois par tour de boucle.
void debug_apres_vblank(void) {
    u32 maintenant = vblank_compteur;
    // Plus d'une image depuis le tour precedent : on en a rate.
    if (maintenant - image_precedente > 1) {
        images_perdues += maintenant - image_precedente - 1;
    }
    image_precedente = maintenant;
    ecrire_nombre(5, LIGNE_DEBUG, PAL_VERT, maintenant, 7);
    ecrire_nombre(21, LIGNE_DEBUG, PAL_VERT, images_perdues, 5);
}
