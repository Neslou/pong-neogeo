/*
 * Pong pour Neo Geo - etape pong-9
 * Le service : celui qui vient d'encaisser le point sert.
 * Serie "Apprendre a coder sur Neo Geo en C", episode 07.
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
#include "debug.h"
#include "vblank.h"
#include "score.h"

// Les etats du jeu (patron "State") :
//   DEBUT --START--> SERVICE --START--> JEU --point--> SERVICE ...
#define ETAT_DEBUT    0
#define ETAT_SERVICE  1
#define ETAT_JEU      2

static Raquette joueur1, joueur2;
static Balle balle;
static u8 etat = ETAT_DEBUT;
static u8 joueur_au_service = 1;
static u8 score1 = 0, score2 = 0;
static u8 score_change = 0;   // "drapeau sale" : 1 = le score est a redessiner

static void init_palettes(void) {
    MMAP_PALBANK1[PAL_BLANC * 16 + 1] = 0x0fff;
    MMAP_PALBANK1[PAL_BLANC * 16 + 2] = 0x0555;
    MMAP_PALBANK1[PAL_SPRITES * 16 + 1] = 0x0fff;
    MMAP_PALBANK1[4095] = 0x6223;
}

// Efface une ligne de la couche fix, puis y centre un texte.
static void message(u8 ligne, const char *texte) {
    ng_text(0, ligne, PAL_BLANC, "                                        ");
    ng_center_text(ligne, PAL_BLANC, texte);
}

static void afficher_etat(void) {
    if (etat == ETAT_DEBUT) {
        message(3, "BIENVENUE DANS PONG !");
        message(5, "START POUR COMMENCER");
    } else if (etat == ETAT_SERVICE) {
        message(3, joueur_au_service == 1 ? "SERVICE : JOUEUR 1" : "SERVICE : JOUEUR 2");
        message(5, "START POUR SERVIR");
    } else {
        message(3, "");
        message(5, "");
    }
}

static s16 vitesse_selon(u8 manette) {
    if (manette & CNT_UP)   return -VITESSE_RAQUETTE;
    if (manette & CNT_DOWN) return VITESSE_RAQUETTE;
    return 0;
}

// Un point est marque : le perdant du point servira.
static void point_pour(u8 joueur) {
    if (joueur == 1) {
        score1++;
        joueur_au_service = 2;
    } else {
        score2++;
        joueur_au_service = 1;
    }
    score_change = 1;
    balle_au_centre(&balle);
    etat = ETAT_SERVICE;
    afficher_etat();
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
    debug_init();
    score_init();

    u8 graine_choisie = 0;
    u16 images = 0;   // nombre de tours de boucle (= d'images) depuis le debut

    for (;;) {
        if (start_vient_d_etre_appuye()) {
            if (!graine_choisie) {
                hasard_graine(images);   // le hasard = l'instant ou le joueur appuie
                graine_choisie = 1;
            }
            if (etat == ETAT_DEBUT) {
                etat = ETAT_SERVICE;
                afficher_etat();
            } else if (etat == ETAT_SERVICE) {
                balle_servir(&balle, joueur_au_service);
                etat = ETAT_JEU;
                afficher_etat();
            }
        }

        joueur1.dy = vitesse_selon(bios_p1current);
        joueur2.dy = vitesse_selon(bios_p2current);
        raquette_mettre_a_jour(&joueur1);
        raquette_mettre_a_jour(&joueur2);

        if (etat == ETAT_JEU) {
            balle_mettre_a_jour(&balle);
            if (balle_touche(&balle, &joueur1)) {
                balle_renvoyer(&balle, &joueur1);
            }
            if (balle_touche(&balle, &joueur2)) {
                balle_renvoyer(&balle, &joueur2);
            }
            balle_rebondir_murs(&balle);

            // Un point quand la balle est sortie EN ENTIER de l'ecran.
            if (PIXELS(balle.x) + BALLE_T <= 0) {
                point_pour(2);
            } else if (PIXELS(balle.x) >= ECRAN_L) {
                point_pour(1);
            }
        }

        if (bios_p1current & CNT_A) {
            for (volatile u16 attente = 0; attente < 4000; attente++) {}
        }

        attendre_vblank();
        images++;
        debug_apres_vblank();

        raquette_afficher(&joueur1);
        raquette_afficher(&joueur2);
        balle_afficher(&balle);
        if (score_change) {
            score_afficher(1, score1);
            score_afficher(2, score2);
            score_change = 0;
        }
    }
    return 0;
}
