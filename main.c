/*
 * Pong pour Neo Geo - bonus pong-2-neogeo (branche bonus-2-neogeo)
 * Le plus Neo Geo de l'episode 04 : une palette par joueur, et des
 * raquettes qui se "deplient" grace a la reduction materielle (SCB2).
 * Serie "Apprendre a coder sur Neo Geo en C", episode 04.
 *
 * La Neo Geo ne sait pas "dessiner un rectangle" : tout ce qui bouge
 * est un sprite, une colonne de tuiles de 16 x 16 pixels rangees
 * dans la ROM C. Nos tuiles sont dessinees dans assets/pong-sprites.gif.
 */

#include <ngdevkit/neogeo.h>
#include <ngdevkit/bios-calls.h>
#include <ngdevkit/ng-fix.h>
#include <ngdevkit/ng-video.h>   // ng_wait_vblank()

// --- La memoire video des sprites (VRAM) -------------------------------
// Chaque sprite est decrit dans 4 tables :
//   SCB1 : la liste de ses tuiles (64 mots par sprite)
//   SCB2 : son facteur de reduction (zoom)
//   SCB3 : sa position verticale et sa hauteur en tuiles
//   SCB4 : sa position horizontale
// Leurs adresses (ADDR_SCB1 a ADDR_SCB4) sont deja definies par
// ngdevkit, dans <ngdevkit/registers.h>.
#define NB_SPRITES 381

// --- Nos tuiles dans la ROM C ------------------------------------------
// Les 256 premieres tuiles sont celles du modele ngdevkit (le logo).
// pong-sprites.gif est decoupe en tuiles de 16 x 16, ligne par ligne :
#define TUILE_RAQUETTE  256   // haut de la raquette (la tuile 257 = le bas)
#define TUILE_BALLE     258

// --- Les numeros de sprites ---------------------------------------------
#define SPR_RAQUETTE1   1
#define SPR_RAQUETTE2   2
#define SPR_BALLE       3

// --- Palettes -----------------------------------------------------------
#define PAL_BLANC       0     // texte de la couche fix
#define PAL_SPRITES    16     // les sprites peuvent utiliser les 256 palettes
#define PAL_JOUEUR1    17     // bonus : une palette par joueur
#define PAL_JOUEUR2    18

// --- Dimensions (en pixels) ---------------------------------------------
#define ECRAN_L       320
#define ECRAN_H       224
#define RAQUETTE_L      5
#define RAQUETTE_H     20
#define BALLE_T         4

static void init_palettes(void) {
    MMAP_PALBANK1[PAL_BLANC * 16 + 1] = 0x0fff;      // texte blanc
    MMAP_PALBANK1[PAL_BLANC * 16 + 2] = 0x0555;      // ombre grise
    MMAP_PALBANK1[PAL_SPRITES * 16 + 1] = 0x0fff;    // raquettes et balle : blanc
    MMAP_PALBANK1[PAL_JOUEUR1 * 16 + 1] = 0x08ff;    // joueur 1 : cyan
    MMAP_PALBANK1[PAL_JOUEUR2 * 16 + 1] = 0x0f80;    // joueur 2 : orange
    MMAP_PALBANK1[4095] = 0x6223;                    // fond de l'ecran : gris-bleu
}

// Cache les 381 sprites : au demarrage, la VRAM contient n'importe quoi.
// Un sprite de hauteur 0 n'est pas affiche.
static void cacher_tous_les_sprites(void) {
    *REG_VRAMADDR = ADDR_SCB3;
    *REG_VRAMMOD = 1;
    for (u16 i = 0; i < NB_SPRITES; i++) {
        *REG_VRAMRW = 0;
    }
}

// Donne a un sprite sa colonne de tuiles (consecutives dans la ROM C),
// sa palette, et sa taille normale (pas de reduction).
static void sprite_tuiles(u16 sprite, u16 premiere_tuile, u8 nb_tuiles, u8 palette) {
    *REG_VRAMADDR = ADDR_SCB1 + sprite * 64;
    *REG_VRAMMOD = 1;
    for (u8 i = 0; i < nb_tuiles; i++) {
        *REG_VRAMRW = premiere_tuile + i;   // numero de la tuile
        *REG_VRAMRW = palette << 8;         // attributs : palette, pas de miroir
    }
    *REG_VRAMADDR = ADDR_SCB2 + sprite;
    *REG_VRAMRW = 0x0fff;                   // 0x0fff = taille normale
}

// Place un sprite a l'ecran. x, y : coin haut gauche, en pixels.
// La Neo Geo compte Y a l'envers : 496 - y.
static void sprite_placer(u16 sprite, s16 x, s16 y, u8 nb_tuiles) {
    *REG_VRAMADDR = ADDR_SCB3 + sprite;
    *REG_VRAMMOD = 0x200;                   // SCB3 -> SCB4 : 0x200 plus loin
    *REG_VRAMRW = ((496 - y) << 7) | nb_tuiles;
    *REG_VRAMRW = x << 7;
}

// Bonus : la reduction materielle. Le mot SCB2 d'un sprite vaut
// (h << 8) | v : h = largeur de 0 a 15, v = hauteur de 0 a 255.
// 0x0fff = taille normale. La puce video ne fait que REDUIRE.
static void sprite_reduire(u16 sprite, u8 h, u8 v) {
    *REG_VRAMADDR = ADDR_SCB2 + sprite;
    *REG_VRAMRW = (h << 8) | v;
}

// Piege de la reduction : pour un sprite reduit, la puce video va
// chercher des lignes dans TOUTE la colonne de 32 tuiles du sprite
// (SCB1), pas seulement dans nos 2 tuiles. Les 30 autres contiennent
// n'importe quoi : on les remplit avec une tuile vide (259).
#define TUILE_VIDE 259

static void sprite_vider_suite(u16 sprite, u8 depuis) {
    *REG_VRAMADDR = ADDR_SCB1 + sprite * 64 + depuis * 2;
    *REG_VRAMMOD = 1;
    for (u8 i = depuis; i < 32; i++) {
        *REG_VRAMRW = TUILE_VIDE;
        *REG_VRAMRW = 0;
    }
}

// Les deux raquettes passent de la hauteur v1 a la hauteur v2,
// un petit pas a chaque image.
static void raquettes_deplier(s16 v1, s16 v2, s16 pas) {
    for (s16 v = v1; (pas > 0) ? (v <= v2) : (v >= v2); v += pas) {
        ng_wait_vblank();
        sprite_reduire(SPR_RAQUETTE1, 15, v);
        sprite_reduire(SPR_RAQUETTE2, 15, v);
    }
}

static void attendre(u16 images) {
    while (images--) ng_wait_vblank();
}

int main(void) {
    bios_fix_clear();
    init_palettes();
    cacher_tous_les_sprites();

    ng_center_text(3, PAL_BLANC, "HELLO PONG!");

    // Les deux raquettes : la meme paire de tuiles, deux sprites.
    sprite_tuiles(SPR_RAQUETTE1, TUILE_RAQUETTE, 2, PAL_JOUEUR1);
    sprite_tuiles(SPR_RAQUETTE2, TUILE_RAQUETTE, 2, PAL_JOUEUR2);
    sprite_vider_suite(SPR_RAQUETTE1, 2);
    sprite_vider_suite(SPR_RAQUETTE2, 2);
    sprite_tuiles(SPR_BALLE, TUILE_BALLE, 1, PAL_SPRITES);

    // Raquette gauche : 8 pixels du bord. Raquette droite : 8 pixels du bord
    // droit, sans oublier sa largeur (320 - 8 - 5 = 307).
    sprite_placer(SPR_RAQUETTE1, 8, 28, 2);
    sprite_placer(SPR_RAQUETTE2, ECRAN_L - 8 - RAQUETTE_L, ECRAN_H - 28 - RAQUETTE_H, 2);
    // La balle au centre : (320 - 4) / 2 = 158 et (224 - 4) / 2 = 110
    sprite_placer(SPR_BALLE, (ECRAN_L - BALLE_T) / 2, (ECRAN_H - BALLE_T) / 2, 1);

    // Bonus : les raquettes se replient et se deplient, pour toujours.
    for (;;) {
        raquettes_deplier(255, 0, -5);   // repliees en 52 images
        attendre(30);
        raquettes_deplier(0, 255, 5);    // depliees en 52 images
        attendre(90);
    }
    return 0;
}
