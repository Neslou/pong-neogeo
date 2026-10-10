/*
 * Pong pour Neo Geo - etape pong-4
 * Les raquettes restent dans l'ecran, la balle part au hasard,
 * et le jeu a deux etats : DEBUT et JEU (bouton START).
 * Serie "Apprendre a coder sur Neo Geo en C", episode 05.
 */

#include <ngdevkit/neogeo.h>
#include <ngdevkit/bios-calls.h>
#include <ngdevkit/ng-fix.h>
#include <ngdevkit/ng-video.h>
#include <ngdevkit/bios-ram.h>

// --- Memoire video des sprites -------------------------------------------
// Leurs adresses (ADDR_SCB1 a ADDR_SCB4) sont deja definies par
// ngdevkit, dans <ngdevkit/registers.h>.
#define NB_SPRITES 381

#define TUILE_RAQUETTE  256
#define TUILE_BALLE     258
#define SPR_RAQUETTE1   1
#define SPR_RAQUETTE2   2
#define SPR_BALLE       3
#define PAL_BLANC       0
#define PAL_SPRITES    16

// --- Dimensions -----------------------------------------------------------
#define ECRAN_L       320
#define ECRAN_H       224
#define RAQUETTE_L      5
#define RAQUETTE_H     20
#define BALLE_T         4

// --- Virgule fixe 8.8 ------------------------------------------------------
#define FIXE(px)  ((s32)(px) << 8)
#define PIXELS(f) ((s16)((f) >> 8))
#define VITESSE_RAQUETTE  797   // 3,11 px par image

// --- Les etats du jeu --------------------------------------------------
#define ETAT_DEBUT  0
#define ETAT_JEU    1

static s32 raquette1_y, raquette2_y;     // positions des raquettes (8.8)
static s32 balle_x, balle_y;             // position de la balle (8.8)
static s16 balle_dx, balle_dy;           // vitesse de la balle (8.8, par image)
static u8 etat = ETAT_DEBUT;

// --- Le hasard sans horloge --------------------------------------------
// Un generateur "xorshift" sur 16 bits : chaque appel melange les bits.
// La graine est choisie au premier appui sur START (le moment exact,
// a l'image pres, est imprevisible).
static u16 graine = 1;

static u16 hasard(void) {
    graine ^= graine << 7;
    graine ^= graine >> 9;
    graine ^= graine << 8;
    return graine;
}

// Un nombre entre min et max (inclus).
static s16 hasard_entre(s16 min, s16 max) {
    return min + (s16)(hasard() % (u16)(max - min + 1));
}

// --- Sprites -------------------------------------------------------------
static void init_palettes(void) {
    MMAP_PALBANK1[PAL_BLANC * 16 + 1] = 0x0fff;
    MMAP_PALBANK1[PAL_BLANC * 16 + 2] = 0x0555;
    MMAP_PALBANK1[PAL_SPRITES * 16 + 1] = 0x0fff;
    MMAP_PALBANK1[4095] = 0x6223;
}

static void cacher_tous_les_sprites(void) {
    *REG_VRAMADDR = ADDR_SCB3;
    *REG_VRAMMOD = 1;
    for (u16 i = 0; i < NB_SPRITES; i++) {
        *REG_VRAMRW = 0;
    }
}

static void sprite_tuiles(u16 sprite, u16 premiere_tuile, u8 nb_tuiles, u8 palette) {
    *REG_VRAMADDR = ADDR_SCB1 + sprite * 64;
    *REG_VRAMMOD = 1;
    for (u8 i = 0; i < nb_tuiles; i++) {
        *REG_VRAMRW = premiere_tuile + i;
        *REG_VRAMRW = palette << 8;
    }
    *REG_VRAMADDR = ADDR_SCB2 + sprite;
    *REG_VRAMRW = 0x0fff;
}

static void sprite_placer(u16 sprite, s16 x, s16 y, u8 nb_tuiles) {
    *REG_VRAMADDR = ADDR_SCB3 + sprite;
    *REG_VRAMMOD = 0x200;
    *REG_VRAMRW = ((496 - y) << 7) | nb_tuiles;
    *REG_VRAMRW = x << 7;
}

// --- La balle ------------------------------------------------------------
// Au centre, avec une vitesse au hasard.
// Cours d'origine : 100 px/s a gauche ou a droite, -50 a 50 px/s en hauteur.
// Converti par image et a notre echelle : 1,25 px (320) et -0,78 a 0,78 px (-199 a 199).
static void balle_au_centre(void) {
    balle_x = FIXE((ECRAN_L - BALLE_T) / 2);
    balle_y = FIXE((ECRAN_H - BALLE_T) / 2);
    balle_dx = (hasard() & 1) ? 320 : -320;
    balle_dy = hasard_entre(-199, 199);
}

// --- Bloquer une raquette dans l'ecran ----------------------------------
static s32 bloquer(s32 y) {
    if (y < 0) return 0;
    if (y > FIXE(ECRAN_H - RAQUETTE_H)) return FIXE(ECRAN_H - RAQUETTE_H);
    return y;
}

static void afficher_etat(void) {
    if (etat == ETAT_DEBUT) {
        ng_center_text(3, PAL_BLANC, "ETAT : DEBUT  ");
    } else {
        ng_center_text(3, PAL_BLANC, "ETAT : JEU    ");
    }
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

    sprite_tuiles(SPR_RAQUETTE1, TUILE_RAQUETTE, 2, PAL_SPRITES);
    sprite_tuiles(SPR_RAQUETTE2, TUILE_RAQUETTE, 2, PAL_SPRITES);
    sprite_tuiles(SPR_BALLE, TUILE_BALLE, 1, PAL_SPRITES);

    raquette1_y = FIXE(28);
    raquette2_y = FIXE(ECRAN_H - 28 - RAQUETTE_H);
    balle_au_centre();
    afficher_etat();

    u8 graine_choisie = 0;
    u16 images = 0;   // nombre de tours de boucle (= d'images) depuis le debut

    for (;;) {
        // START (appui unique) : on passe d'un etat a l'autre.
        if (start_vient_d_etre_appuye()) {
            if (!graine_choisie) {
                // la graine = l'instant (en images) ou le joueur appuie
                graine = images | 1;
                graine_choisie = 1;
            }
            // Dans les deux cas, la balle repart du centre avec une
            // nouvelle vitesse au hasard (la graine vient d'etre choisie).
            etat = (etat == ETAT_DEBUT) ? ETAT_JEU : ETAT_DEBUT;
            balle_au_centre();
            afficher_etat();
        }

        // Les raquettes, toujours, mais bloquees aux bords.
        if (bios_p1current & CNT_UP)   raquette1_y = bloquer(raquette1_y - VITESSE_RAQUETTE);
        if (bios_p1current & CNT_DOWN) raquette1_y = bloquer(raquette1_y + VITESSE_RAQUETTE);
        if (bios_p2current & CNT_UP)   raquette2_y = bloquer(raquette2_y - VITESSE_RAQUETTE);
        if (bios_p2current & CNT_DOWN) raquette2_y = bloquer(raquette2_y + VITESSE_RAQUETTE);

        // La balle ne bouge que pendant le jeu.
        if (etat == ETAT_JEU) {
            balle_x += balle_dx;
            balle_y += balle_dy;
        }

        ng_wait_vblank();
        images++;

        sprite_placer(SPR_RAQUETTE1, 8, PIXELS(raquette1_y), 2);
        sprite_placer(SPR_RAQUETTE2, ECRAN_L - 8 - RAQUETTE_L, PIXELS(raquette2_y), 2);
        sprite_placer(SPR_BALLE, PIXELS(balle_x), PIXELS(balle_y), 1);
    }
    return 0;
}
