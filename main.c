/*
 * Pong pour Neo Geo - etape pong-0
 * La plus petite ROM possible : le mot PONG au milieu de l'ecran.
 * Serie "Apprendre a coder sur Neo Geo en C", episode 03.
 */

#include <ngdevkit/neogeo.h>      // registres et types de la Neo Geo (u8, u16...)
#include <ngdevkit/bios-calls.h>  // fonctions du BIOS, dont bios_fix_clear()
#include <ngdevkit/ng-fix.h>      // ng_text() : ecrire sur la couche fix

int main(void) {
    // 1. Effacer la couche fix : au demarrage, elle contient encore
    //    des caracteres laisses par l'ecran d'accueil.
    bios_fix_clear();

    // 2. Choisir les couleurs de la palette 0.
    //    La couleur 0 est toujours transparente : on commence a 1.
    MMAP_PALBANK1[1] = 0x0fff;   // couleur 1 : blanc, le corps des lettres
    MMAP_PALBANK1[2] = 0x0555;   // couleur 2 : gris, l'ombre des lettres

    // 3. Ecrire PONG au centre.
    //    Colonne : (40 colonnes - 4 lettres) / 2 = 18
    //    Ligne   : 15, le milieu de la partie visible (lignes 2 a 29)
    //    Palette : 0
    ng_text(18, 15, 0, "PONG");

    // 4. Une console n'a pas de systeme d'exploitation ou "revenir" :
    //    le programme ne doit jamais se terminer. On tourne en rond.
    for (;;) {}

    return 0;   // jamais atteint, mais le C l'attend
}
