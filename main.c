/*
 * Pong pour Neo Geo - etape pong-5
 * Le code est range : une struct et un fichier par objet.
 * Le jeu se comporte exactement comme a l'etape pong-4.
 * Serie "Apprendre a coder sur Neo Geo en C", episode 06.
 */

#include <ngdevkit/neogeo.h>
#include <ngdevkit/bios-calls.h>
#include <ngdevkit/ng-fix.h>
#include <ngdevkit/ng-video.h>
#include <ngdevkit/bios-ram.h>

#include "pong.h"
#include "sprites.h"
#include "hasard.h"
#include "raquette.h"
#include "balle.h"

#define ETAT_DEBUT  0
#define ETAT_JEU    1

static Raquette joueur1, joueur2;
static Balle balle;
static u8 etat = ETAT_DEBUT;

static void init_palettes(void) {
    MMAP_PALBANK1[PAL_BLANC * 16 + 1] = 0x0fff;
    MMAP_PALBANK1[PAL_BLANC * 16 + 2] = 0x0555;
    MMAP_PALBANK1[PAL_SPRITES * 16 + 1] = 0x0fff;
    MMAP_PALBANK1[4095] = 0x6223;
}

static void afficher_etat(void) {
    if (etat == ETAT_DEBUT) {
        ng_center_text(3, PAL_BLANC, "ETAT : DEBUT  ");
    } else {
        ng_center_text(3, PAL_BLANC, "ETAT : JEU    ");
    }
}

// La vitesse d'une raquette selon sa manette : haut, bas, ou rien.
static s16 vitesse_selon(u8 manette) {
    if (manette & CNT_UP)   return -VITESSE_RAQUETTE;
    if (manette & CNT_DOWN) return VITESSE_RAQUETTE;
    return 0;
}

// START "vient d'etre appuye" : vrai UNE seule image, quand le bouton
// passe de relache a enfonce (un "front montant"). C'est l'equivalent
// de love.keypressed en Lua, alors que bios_p1current ressemble a
// love.keyboard.isDown. On le calcule nous-memes a partir de l'etat
// courant (bios_statcurnt) : ca marche avec tous les BIOS.
static u8 start_avant = 0;
static u8 start_vient_d_etre_appuye(void) {
    u8 start = bios_statcurnt & CNT_START1;
    u8 resultat = start && !start_avant;
    start_avant = start;
    return resultat;
}

int main(void) {
    bios_fix_clear();
    init_palettes();
    cacher_tous_les_sprites();

    raquette_init(&joueur1, SPR_RAQUETTE1, 8, 28);
    raquette_init(&joueur2, SPR_RAQUETTE2, ECRAN_L - 8 - RAQUETTE_L, ECRAN_H - 28 - RAQUETTE_H);
    balle_init(&balle, SPR_BALLE);
    afficher_etat();

    u8 graine_choisie = 0;
    u16 images = 0;   // nombre de tours de boucle (= d'images) depuis le debut

    for (;;) {
        if (start_vient_d_etre_appuye()) {
            if (!graine_choisie) {
                hasard_graine(images);   // le hasard = l'instant ou le joueur appuie
                graine_choisie = 1;
            }
            etat = (etat == ETAT_DEBUT) ? ETAT_JEU : ETAT_DEBUT;
            balle_au_centre(&balle);
            afficher_etat();
        }

        joueur1.dy = vitesse_selon(bios_p1current);
        joueur2.dy = vitesse_selon(bios_p2current);
        raquette_mettre_a_jour(&joueur1);
        raquette_mettre_a_jour(&joueur2);

        if (etat == ETAT_JEU) {
            balle_mettre_a_jour(&balle);
        }

        ng_wait_vblank();
        images++;

        raquette_afficher(&joueur1);
        raquette_afficher(&joueur2);
        balle_afficher(&balle);
    }
    return 0;
}
