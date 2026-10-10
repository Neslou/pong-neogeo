/*
 * Pong pour Neo Geo - etape pong-1-neogeo (bonus)
 * L'ecran titre, avec une couleur de fond qui "respire" pendant le VBlank.
 * Serie "Apprendre a coder sur Neo Geo en C", episode 03.
 */

#include <ngdevkit/neogeo.h>
#include <ngdevkit/bios-calls.h>
#include <ngdevkit/ng-fix.h>
#include <ngdevkit/ng-video.h>   // ng_wait_vblank() : attendre la fin de l'image

// Zone sure de la couche fix : visible sur toutes les teles.
// Grille : 40 colonnes (0 a 39) x 32 lignes (0 a 31), lignes 2 a 29 affichees.
#define COL_GAUCHE    1
#define COL_DROITE   38
#define LIGNE_HAUT    3
#define LIGNE_BAS    28

// Numeros de palette utilises par la couche fix
#define PAL_BLANC     0
#define PAL_JAUNE     1

// Prepare les couleurs des deux palettes.
// Couleur 1 = corps des lettres, couleur 2 = ombre (la couleur 0 est transparente).
static void init_palettes(void) {
    MMAP_PALBANK1[PAL_BLANC * 16 + 1] = 0x0fff;   // blanc
    MMAP_PALBANK1[PAL_BLANC * 16 + 2] = 0x0555;   // gris
    MMAP_PALBANK1[PAL_JAUNE * 16 + 1] = 0x0fd2;   // jaune
    MMAP_PALBANK1[PAL_JAUNE * 16 + 2] = 0x0530;   // brun
}

// Un "+" dans chaque coin de la zone sure, pour verifier ce que montre l'ecran.
static void reperes_zone_sure(void) {
    ng_text(COL_GAUCHE, LIGNE_HAUT, PAL_BLANC, "+");
    ng_text(COL_DROITE, LIGNE_HAUT, PAL_BLANC, "+");
    ng_text(COL_GAUCHE, LIGNE_BAS,  PAL_BLANC, "+");
    ng_text(COL_DROITE, LIGNE_BAS,  PAL_BLANC, "+");
}

// Un bleu nuit plus ou moins clair, pour niveau = 0 (noir) a 12.
// Rouge = niveau / 4, vert = niveau / 2, bleu = niveau.
static u16 bleu_nuit(u8 niveau) {
    u16 rouge = niveau >> 2;
    u16 vert  = niveau >> 1;
    u16 bleu  = niveau;
    return (rouge << 8) | (vert << 4) | bleu;
}

int main(void) {
    bios_fix_clear();
    init_palettes();
    reperes_zone_sure();

    // ng_center_text calcule la colonne : (40 - longueur) / 2
    // La version "tall" dessine des lettres deux fois plus hautes,
    // sur la ligne indiquee et la suivante (ici 9 et 10).
    ng_center_text_tall(9, PAL_BLANC, "PONG");
    ng_center_text(13, PAL_BLANC, "UN PONG POUR NEO GEO");
    ng_center_text(21, PAL_JAUNE, "PRESS START");
    ng_center_text(27, PAL_BLANC, "(C) 2026 NESLOU");

    // La boucle de jeu, enfin vivante : une fois par image.
    u8 niveau = 0;       // de 0 a 12
    s8 sens = 1;         // +1 : on eclaircit, -1 : on assombrit
    u8 compteur = 0;     // compte les images

    for (;;) {
        // Attendre le VBlank : le faisceau remonte en haut de l'ecran.
        // C'est le seul moment ou changer une couleur ne laisse pas de trace.
        ng_wait_vblank();

        // La case 4095 = palette 255, couleur 15 = le fond de l'ecran.
        MMAP_PALBANK1[4095] = bleu_nuit(niveau);

        // Toutes les 6 images (environ 10 fois par seconde), un cran de plus.
        compteur++;
        if (compteur == 6) {
            compteur = 0;
            niveau += sens;
            if (niveau == 12 || niveau == 0) {
                sens = -sens;   // demi-tour aux deux extremites
            }
        }
    }

    return 0;
}
